/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"
#include "ideatek_input_mapping.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_action_type_t", ideatek_action_type_t);
    ENUM(api, "ideatek:ideatek_accumulate_t", ideatek_accumulate_t);
    ENUM(api, "ideatek:ideatek_action_state_t", ideatek_action_state_t);
    MASK(api, "ideatek:ideatek_action_edges_t", ideatek_action_edges_t);
    ENUM(api, "ideatek:ideatek_input_source_t", ideatek_input_source_t);
    ENUM(api, "ideatek:ideatek_dead_zone_kind_t", ideatek_dead_zone_kind_t);
    MASK(api, "ideatek:ideatek_axes_t", ideatek_axes_t);
    ENUM(api, "ideatek:ideatek_swizzle_t", ideatek_swizzle_t);
    MASK(api, "ideatek:ideatek_hold_flags_t", ideatek_hold_flags_t);
    MASK(api, "ideatek:ideatek_pulse_flags_t", ideatek_pulse_flags_t);
    MASK(api, "ideatek:ideatek_chord_flags_t", ideatek_chord_flags_t);
    MASK(api, "ideatek:ideatek_context_flags_t", ideatek_context_flags_t);
    MASK(api, "ideatek:ideatek_capture_sources_t", ideatek_capture_sources_t);
    ENUM(api, "ideatek:ideatek_device_kind_t", ideatek_device_kind_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:InputAction", ideatek_input_action_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:InputAction", ideatek_input_action_t);
    STRUCT(api, "ideatek:InputActionState", ideatek_input_action_state_t);
    STRUCT(api, "ideatek:InputBinding", ideatek_input_binding_t);
    STRUCT(api, "ideatek:InputDeadZone", ideatek_input_dead_zone_t);
    STRUCT(api, "ideatek:InputCurve", ideatek_input_curve_t);
    STRUCT(api, "ideatek:InputScale", ideatek_input_scale_t);
    STRUCT(api, "ideatek:InputNegate", ideatek_input_negate_t);
    STRUCT(api, "ideatek:InputSwizzle", ideatek_input_swizzle_t);
    STRUCT(api, "ideatek:InputPressed", ideatek_input_pressed_t);
    STRUCT(api, "ideatek:InputReleased", ideatek_input_released_t);
    STRUCT(api, "ideatek:InputHold", ideatek_input_hold_t);
    STRUCT(api, "ideatek:InputTap", ideatek_input_tap_t);
    STRUCT(api, "ideatek:InputPulse", ideatek_input_pulse_t);
    STRUCT(api, "ideatek:InputChord", ideatek_input_chord_t);
    STRUCT(api, "ideatek:InputContext", ideatek_input_context_t);
    STRUCT(api, "ideatek:InputPlayer", ideatek_input_player_t);
    STRUCT(api, "ideatek:InputDevice", ideatek_input_device_t);
    STRUCT(api, "ideatek:InputCapture", ideatek_input_capture_t);
    STRUCT(api, "ideatek:InputCaptured", ideatek_input_captured_t);
    STRUCT(api, "ideatek:InputLastDevice", ideatek_input_last_device_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_action_type_t", ideatek_action_type_t);
    ENUM(api, "ideatek:ideatek_accumulate_t", ideatek_accumulate_t);
    ENUM(api, "ideatek:ideatek_input_source_t", ideatek_input_source_t);
    ENUM(api, "ideatek:ideatek_dead_zone_kind_t", ideatek_dead_zone_kind_t);
    MASK(api, "ideatek:ideatek_axes_t", ideatek_axes_t);
    ENUM(api, "ideatek:ideatek_swizzle_t", ideatek_swizzle_t);
    MASK(api, "ideatek:ideatek_hold_flags_t", ideatek_hold_flags_t);
    MASK(api, "ideatek:ideatek_pulse_flags_t", ideatek_pulse_flags_t);
    MASK(api, "ideatek:ideatek_chord_flags_t", ideatek_chord_flags_t);
    MASK(api, "ideatek:ideatek_context_flags_t", ideatek_context_flags_t);
    MASK(api, "ideatek:ideatek_capture_sources_t", ideatek_capture_sources_t);
    api->attach(api, id, STRUCT(api, "ideatek:InputAction",
        ideatek_input_action_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputBinding",
        ideatek_input_binding_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputDeadZone",
        ideatek_input_dead_zone_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputCurve",
        ideatek_input_curve_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputScale",
        ideatek_input_scale_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputNegate",
        ideatek_input_negate_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputSwizzle",
        ideatek_input_swizzle_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputPressed",
        ideatek_input_pressed_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputReleased",
        ideatek_input_released_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputHold", ideatek_input_hold_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputTap", ideatek_input_tap_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputPulse",
        ideatek_input_pulse_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputChord",
        ideatek_input_chord_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputContext",
        ideatek_input_context_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputPlayer",
        ideatek_input_player_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputDevice",
        ideatek_input_device_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputCapture",
        ideatek_input_capture_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_mapper(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_action_type_t", ideatek_action_type_t);
    ENUM(api, "ideatek:ideatek_accumulate_t", ideatek_accumulate_t);
    ENUM(api, "ideatek:ideatek_action_state_t", ideatek_action_state_t);
    MASK(api, "ideatek:ideatek_action_edges_t", ideatek_action_edges_t);
    ENUM(api, "ideatek:ideatek_input_source_t", ideatek_input_source_t);
    ENUM(api, "ideatek:ideatek_dead_zone_kind_t", ideatek_dead_zone_kind_t);
    MASK(api, "ideatek:ideatek_axes_t", ideatek_axes_t);
    ENUM(api, "ideatek:ideatek_swizzle_t", ideatek_swizzle_t);
    MASK(api, "ideatek:ideatek_hold_flags_t", ideatek_hold_flags_t);
    MASK(api, "ideatek:ideatek_pulse_flags_t", ideatek_pulse_flags_t);
    MASK(api, "ideatek:ideatek_chord_flags_t", ideatek_chord_flags_t);
    MASK(api, "ideatek:ideatek_context_flags_t", ideatek_context_flags_t);
    MASK(api, "ideatek:ideatek_capture_sources_t", ideatek_capture_sources_t);
    ENUM(api, "ideatek:ideatek_device_kind_t", ideatek_device_kind_t);
    STRUCT(api, "ideatek:InputAction", ideatek_input_action_t);
    STRUCT(api, "ideatek:InputBinding", ideatek_input_binding_t);
    STRUCT(api, "ideatek:InputDeadZone", ideatek_input_dead_zone_t);
    STRUCT(api, "ideatek:InputCurve", ideatek_input_curve_t);
    STRUCT(api, "ideatek:InputScale", ideatek_input_scale_t);
    STRUCT(api, "ideatek:InputNegate", ideatek_input_negate_t);
    STRUCT(api, "ideatek:InputSwizzle", ideatek_input_swizzle_t);
    STRUCT(api, "ideatek:InputPressed", ideatek_input_pressed_t);
    STRUCT(api, "ideatek:InputReleased", ideatek_input_released_t);
    STRUCT(api, "ideatek:InputHold", ideatek_input_hold_t);
    STRUCT(api, "ideatek:InputTap", ideatek_input_tap_t);
    STRUCT(api, "ideatek:InputPulse", ideatek_input_pulse_t);
    STRUCT(api, "ideatek:InputChord", ideatek_input_chord_t);
    STRUCT(api, "ideatek:InputContext", ideatek_input_context_t);
    STRUCT(api, "ideatek:InputPlayer", ideatek_input_player_t);
    STRUCT(api, "ideatek:InputDevice", ideatek_input_device_t);
    STRUCT(api, "ideatek:InputCapture", ideatek_input_capture_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Tick", ideatek_tick_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:InputActionState",
        ideatek_input_action_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputCaptured",
        ideatek_input_captured_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InputLastDevice",
        ideatek_input_last_device_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_MAPPER)
    role_mapper(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
