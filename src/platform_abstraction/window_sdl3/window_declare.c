/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ideatek_entity_t", ideatek_entity_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_window_mode_t",
        ideatek_window_mode_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_display_target_t",
        ideatek_display_target_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_surface_t",
        ideatek_surface_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_drop_kind_t",
        ideatek_drop_kind_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
}

static void declare_window(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WindowRequest",
        ideatek_window_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WindowState",
        ideatek_window_state_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Display", ideatek_display_t,
        BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:VideoMode", ideatek_video_mode_t,
        BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
}

static void declare_items(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Drop", ideatek_drop_t,
        BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ClipboardRequest",
        ideatek_clipboard_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Paste", ideatek_paste_t,
        BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:EventWait", ideatek_event_wait_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek_window_sdl3:Settings",
        window_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    declare_window(api);
    declare_items(api);
}
