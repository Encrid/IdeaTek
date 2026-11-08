/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "frame_inside.h"

static const char *const SOURCES[2] = { "os", "fixed" };
static const char *const SLEEPERS[2] = { "clock", "events" };
static const char *const SMOOTHINGS[2] = { "none", "refresh" };

static bool text_is(const bridge_api_t *api, bridge_text_t text,
    const char *word)
{
    size_t len = strlen(word);
    bool same = text.len == len;

    return (same & !memcmp(api->shared_base + text.at, word, same * len));
}

static int pick_choice(const bridge_api_t *api, const char *name,
    bridge_text_t text, const char *const words[2])
{
    bool a = text_is(api, text, words[0]);
    bool b = text_is(api, text, words[1]);
    bool over = text.len > FRAME_TEXT;
    int shown = (int)(over * FRAME_TEXT + !over * text.len);

    if (!(a | b))
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %.*s is outside %s or %s",
            name, shown, (const char *)api->shared_base + text.at, words[0],
            words[1]);
    return (b - !(a | b));
}

static bool check_count(const bridge_api_t *api, const char *name,
    uint32_t value, uint32_t most)
{
    bool ok = (value >= 1) & (value <= most);

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside 1 to %u",
            name, value, most);
    return (ok);
}

static bool check_time(const bridge_api_t *api, const char *name,
    double value)
{
    bool ok = value > 0;

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "%s: %g is outside more than 0", name, value);
    return (ok);
}

bool start_clock(const bridge_api_t *api)
{
    const frame_settings_t *s = bridge_native_get_single(api, SETTINGS);
    bool steps_ok = check_count(api, "most_steps", s->most_steps, 100);
    bool round_ok = check_time(api, "fixed_round", s->fixed_round);
    int source = pick_choice(api, "time_source", s->time_source, SOURCES);
    int sleeper = pick_choice(api, "sleeper", s->sleeper, SLEEPERS);
    int smoothing = pick_choice(api, "smoothing", s->smoothing, SMOOTHINGS);

    frame_module.started = true;
    frame_module.broken = !(steps_ok & round_ok & (source >= 0)
        & (sleeper >= 0) & (smoothing >= 0));
    frame_module.most_steps = s->most_steps;
    frame_module.rule.most_steps = s->most_steps;
    frame_module.fixed_round = s->fixed_round;
    frame_module.fixed = source == 1;
    frame_module.events = sleeper == 1;
    frame_module.refresh = smoothing == 1;
    return (!frame_module.broken);
}
