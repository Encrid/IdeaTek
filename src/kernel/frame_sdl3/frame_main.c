/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "frame_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

frame_module_t frame_module;

static const ideatek_time_request_t DEFAULTS = { .scale = 1, .rate = 64 };

BRIDGE_NATIVE(init)
{
    ideatek_frame_t frame = {};
    ideatek_tick_t tick = {};
    ideatek_time_request_t request = DEFAULTS;

    declare_types(api);
    add_runs(api);
    BRIDGE_NATIVE_ATTACH(api, 0, FRAME, frame);
    BRIDGE_NATIVE_ATTACH(api, 0, TICK, tick);
    BRIDGE_NATIVE_ATTACH(api, 0, TIME_REQUEST, request);
    frame_module.rule = (frame_rule_t){ .scale = 1, .rate = 64,
        .most_steps = 8 };
    frame_module.seen = DEFAULTS;
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}
