/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

void declare_pads(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Gamepad", ideatek_gamepad_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:GamepadState",
        ideatek_gamepad_state_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:GamepadEvent",
        ideatek_gamepad_event_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:GamepadRequest",
        ideatek_gamepad_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:FingerEvent", ideatek_finger_event_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WindowState", ideatek_window_state_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek_input_sdl3:Settings",
        input_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}
