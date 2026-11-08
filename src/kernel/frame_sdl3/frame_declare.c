/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "frame_inside.h"

void declare_types(const bridge_api_t *api)
{
    FRAME_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    FRAME_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_window_mode_t",
        ideatek_window_mode_t);
    FRAME_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_surface_t",
        ideatek_surface_t);
    FRAME_TYPE(api, BRIDGE_STRUCT, "ideatek:WindowState",
        ideatek_window_state_t);
    FRAME_TYPE(api, BRIDGE_STRUCT, "ideatek:EventWait", ideatek_event_wait_t);
    FRAME_SINGLE(api, "ideatek:Frame", ideatek_frame_t);
    FRAME_SINGLE(api, "ideatek:Tick", ideatek_tick_t);
    FRAME_SINGLE(api, "ideatek:TimeRequest", ideatek_time_request_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek_frame_sdl3:Settings",
        frame_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { FRAME, TICK, EVENT_WAIT };
    bridge_term_t term[1] = {{ WINDOW_STATE, BRIDGE_READ, BRIDGE_AND, 0 }};
    uint32_t events = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH | BRIDGE_ON_LEAVE;

    BRIDGE_NATIVE_ADD_SYSTEM(api, "round", "first", nullptr, 0, changes, 3,
        nullptr, 0);
    BRIDGE_NATIVE_ADD_SYSTEM(api, "step", "fixed", nullptr, 0, changes + 1, 1,
        nullptr, 0);
    frame_module.window_list = BRIDGE_NATIVE_OBSERVER(api, "window", events,
        term, 1, nullptr, 0, nullptr, 0);
}
