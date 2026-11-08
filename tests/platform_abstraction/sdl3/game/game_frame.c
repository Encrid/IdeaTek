/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_frame.h"

#if defined(GAME_FRAME)

#define SINGLE(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, name, T, \
    BRIDGE_STRIP, BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE)

game_frame_slots_t game_frame_slots;

static void show_frame(const bridge_api_t *api)
{
    const ideatek_frame_t *f = bridge_native_get_single(api,
        game_frame_slots.frame);

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u frame %llu delta %.6f elapsed %.6f real %.6f %.6f scale %.2f"
        " paused %u", game.round, (unsigned long long)f->index, f->delta,
        f->elapsed, f->real_delta, f->real_elapsed, f->scale, f->paused);
}

static void show_tick(const bridge_api_t *api)
{
    const ideatek_tick_t *t = bridge_native_get_single(api,
        game_frame_slots.tick);

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u tick %llu elapsed %.6f step %.6f steps %u fraction %.4f",
        game.round, (unsigned long long)t->tick, t->elapsed, t->step,
        t->steps, t->fraction);
}

static void ask_time(const bridge_api_t *api, char *args)
{
    ideatek_time_request_t r = {};
    char *left = nullptr;

    r.scale = atof(strtok_r(args, ",", &left));
    r.rate = (uint32_t)strtoul(strtok_r(nullptr, ",", &left), nullptr, 0);
    r.max_rate = (uint32_t)strtoul(strtok_r(nullptr, ",", &left), nullptr, 0);
    r.paused = (uint8_t)strtoul(strtok_r(nullptr, ",", &left), nullptr, 0);
    BRIDGE_NATIVE_ATTACH(api, 0, game_frame_slots.request, r);
}

bool game_frame_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "timereq"))
        ask_time(api, args);
    else if (!strcmp(verb, "show_frame"))
        show_frame(api);
    else if (!strcmp(verb, "show_tick"))
        show_tick(api);
    else
        done = false;
    return (done);
}

void game_declare_frame(const bridge_api_t *api)
{
    game_frame_slots.frame = SINGLE(api, "ideatek:Frame", ideatek_frame_t);
    game_frame_slots.tick = SINGLE(api, "ideatek:Tick", ideatek_tick_t);
    game_frame_slots.request = SINGLE(api, "ideatek:TimeRequest",
        ideatek_time_request_t);
}

BRIDGE_NATIVE(step)
{
    const ideatek_tick_t *t = bridge_native_get_single(api,
        game_frame_slots.tick);

    if (t->steps)
        bridge_native_say(api, BRIDGE_LOG_INFO, "step %llu %.6f",
            (unsigned long long)t->tick, t->elapsed);
    return (0);
}

void game_watch_frame(const bridge_api_t *api)
{
    BRIDGE_NATIVE_ADD_SYSTEM(api, "step", "fixed", nullptr, 0, nullptr, 0,
        nullptr, 0);
}

#endif
