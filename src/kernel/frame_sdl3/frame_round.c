/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "frame_inside.h"

static void wait_turn(void)
{
    uint32_t limit = frame_module.max_rate;
    uint64_t period = 1000000000ULL / (limit + !limit);
    bool waits = !frame_module.events & (limit > 0)
        & (frame_module.clock.index > 0);
    uint64_t due = frame_module.last_start + period;
    uint64_t now = SDL_GetTicksNS();
    uint64_t left = waits * (due > now) * (due - now);

    SDL_DelayPrecise(left);
}

static double read_real(uint64_t now)
{
    bool first = frame_module.clock.index == 0;
    double os = (double)(now - frame_module.last_start) / 1e9;
    double real = frame_module.fixed * frame_module.fixed_round
        + !frame_module.fixed * os;
    double smooth = smoothed(real);

    real = frame_module.refresh * smooth + !frame_module.refresh * real;
    return (real * !first);
}

static void write_wait(const bridge_api_t *api)
{
    uint32_t limit = frame_module.max_rate;
    ideatek_event_wait_t w = { .wait = (limit > 0) / (double)(limit + !limit) };
    bool writes = frame_module.events & (frame_module.window != 0);

    if (writes)
        BRIDGE_NATIVE_ATTACH(api, frame_module.window, EVENT_WAIT, w);
}

static void first_step(void)
{
    ideatek_tick_t *tick = &frame_module.values.tick;
    bool has = tick->steps > 0;

    tick->tick += has;
    tick->elapsed += has * tick->step;
    frame_module.run = 0;
}

BRIDGE_NATIVE(round)
{
    uint64_t now;
    double real;

    if (!frame_module.started)
        start_clock(api);
    if (frame_module.broken)
        return (1);
    apply_request(api);
    wait_turn();
    now = SDL_GetTicksNS();
    real = read_real(now);
    frame_module.last_start = now;
    start_round(&frame_module.clock, &frame_module.rule, real,
        &frame_module.values);
    first_step();
    BRIDGE_NATIVE_ATTACH(api, 0, FRAME, frame_module.values.frame);
    BRIDGE_NATIVE_ATTACH(api, 0, TICK, frame_module.values.tick);
    write_wait(api);
    return (0);
}
