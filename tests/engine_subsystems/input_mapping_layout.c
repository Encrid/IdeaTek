/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_input_mapping.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_action_type(void)
{
    SIZE(ideatek_action_type_t);
}

static void show_accumulate(void)
{
    SIZE(ideatek_accumulate_t);
}

static void show_input_action(void)
{
    SIZE(ideatek_input_action_t);
    AT(ideatek_input_action_t, type);
    AT(ideatek_input_action_t, accumulate);
}

static void show_action_state(void)
{
    SIZE(ideatek_action_state_t);
}

static void show_action_edges(void)
{
    SIZE(ideatek_action_edges_t);
}

static void show_input_action_state(void)
{
    SIZE(ideatek_input_action_state_t);
    AT(ideatek_input_action_state_t, x);
    AT(ideatek_input_action_state_t, y);
    AT(ideatek_input_action_state_t, z);
    AT(ideatek_input_action_state_t, held);
    AT(ideatek_input_action_state_t, state);
    AT(ideatek_input_action_state_t, edges);
}

static void show_input_source(void)
{
    SIZE(ideatek_input_source_t);
}

static void show_input_binding(void)
{
    SIZE(ideatek_input_binding_t);
    AT(ideatek_input_binding_t, context);
    AT(ideatek_input_binding_t, action);
    AT(ideatek_input_binding_t, control);
    AT(ideatek_input_binding_t, source);
}

static void show_dead_zone_kind(void)
{
    SIZE(ideatek_dead_zone_kind_t);
}

static void show_input_dead_zone(void)
{
    SIZE(ideatek_input_dead_zone_t);
    AT(ideatek_input_dead_zone_t, lower);
    AT(ideatek_input_dead_zone_t, upper);
    AT(ideatek_input_dead_zone_t, kind);
}

static void show_input_curve(void)
{
    SIZE(ideatek_input_curve_t);
    AT(ideatek_input_curve_t, x);
    AT(ideatek_input_curve_t, y);
    AT(ideatek_input_curve_t, z);
}

static void show_input_scale(void)
{
    SIZE(ideatek_input_scale_t);
    AT(ideatek_input_scale_t, x);
    AT(ideatek_input_scale_t, y);
    AT(ideatek_input_scale_t, z);
}

static void show_axes(void)
{
    SIZE(ideatek_axes_t);
}

static void show_input_negate(void)
{
    SIZE(ideatek_input_negate_t);
    AT(ideatek_input_negate_t, axes);
}

static void show_swizzle(void)
{
    SIZE(ideatek_swizzle_t);
}

static void show_input_swizzle(void)
{
    SIZE(ideatek_input_swizzle_t);
    AT(ideatek_input_swizzle_t, order);
}

static void show_input_pressed(void)
{
    SIZE(ideatek_input_pressed_t);
    AT(ideatek_input_pressed_t, threshold);
}

static void show_input_released(void)
{
    SIZE(ideatek_input_released_t);
    AT(ideatek_input_released_t, threshold);
}

static void show_hold_flags(void)
{
    SIZE(ideatek_hold_flags_t);
}

static void show_input_hold(void)
{
    SIZE(ideatek_input_hold_t);
    AT(ideatek_input_hold_t, threshold);
    AT(ideatek_input_hold_t, time);
    AT(ideatek_input_hold_t, flags);
}

static void show_input_tap(void)
{
    SIZE(ideatek_input_tap_t);
    AT(ideatek_input_tap_t, threshold);
    AT(ideatek_input_tap_t, time);
}

static void show_pulse_flags(void)
{
    SIZE(ideatek_pulse_flags_t);
}

static void show_input_pulse(void)
{
    SIZE(ideatek_input_pulse_t);
    AT(ideatek_input_pulse_t, threshold);
    AT(ideatek_input_pulse_t, interval);
    AT(ideatek_input_pulse_t, limit);
    AT(ideatek_input_pulse_t, flags);
}

static void show_chord_flags(void)
{
    SIZE(ideatek_chord_flags_t);
}

static void show_input_chord(void)
{
    SIZE(ideatek_input_chord_t);
    AT(ideatek_input_chord_t, action);
    AT(ideatek_input_chord_t, flags);
}

static void show_context_flags(void)
{
    SIZE(ideatek_context_flags_t);
}

static void show_input_context(void)
{
    SIZE(ideatek_input_context_t);
    AT(ideatek_input_context_t, player);
    AT(ideatek_input_context_t, priority);
    AT(ideatek_input_context_t, flags);
}

static void show_input_player(void)
{
    SIZE(ideatek_input_player_t);
    AT(ideatek_input_player_t, flags);
}

static void show_input_device(void)
{
    SIZE(ideatek_input_device_t);
    AT(ideatek_input_device_t, player);
}

static void show_capture_sources(void)
{
    SIZE(ideatek_capture_sources_t);
}

static void show_input_capture(void)
{
    SIZE(ideatek_input_capture_t);
    AT(ideatek_input_capture_t, player);
    AT(ideatek_input_capture_t, sources);
}

static void show_input_captured(void)
{
    SIZE(ideatek_input_captured_t);
    AT(ideatek_input_captured_t, capture);
    AT(ideatek_input_captured_t, device);
    AT(ideatek_input_captured_t, control);
    AT(ideatek_input_captured_t, order);
    AT(ideatek_input_captured_t, source);
}

static void show_device_kind(void)
{
    SIZE(ideatek_device_kind_t);
}

static void show_input_last_device(void)
{
    SIZE(ideatek_input_last_device_t);
    AT(ideatek_input_last_device_t, device);
    AT(ideatek_input_last_device_t, kind);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_action_type_t_desc, ideatek_accumulate_t_desc,
        ideatek_input_action_t_desc, ideatek_action_state_t_desc,
        ideatek_action_edges_t_desc, ideatek_input_action_state_t_desc,
        ideatek_input_source_t_desc, ideatek_input_binding_t_desc,
        ideatek_dead_zone_kind_t_desc, ideatek_input_dead_zone_t_desc,
        ideatek_input_curve_t_desc, ideatek_input_scale_t_desc,
        ideatek_axes_t_desc, ideatek_input_negate_t_desc,
        ideatek_swizzle_t_desc, ideatek_input_swizzle_t_desc,
        ideatek_input_pressed_t_desc, ideatek_input_released_t_desc,
        ideatek_hold_flags_t_desc, ideatek_input_hold_t_desc,
        ideatek_input_tap_t_desc, ideatek_pulse_flags_t_desc,
        ideatek_input_pulse_t_desc, ideatek_chord_flags_t_desc,
        ideatek_input_chord_t_desc, ideatek_context_flags_t_desc,
        ideatek_input_context_t_desc, ideatek_input_player_t_desc,
        ideatek_input_device_t_desc, ideatek_capture_sources_t_desc,
        ideatek_input_capture_t_desc, ideatek_input_captured_t_desc,
        ideatek_device_kind_t_desc, ideatek_input_last_device_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_ACTION_TYPE_VALUES), TEXT_OF(IDEATEK_ACCUMULATE_VALUES),
        TEXT_OF(IDEATEK_INPUT_ACTION_FIELDS),
        TEXT_OF(IDEATEK_ACTION_STATE_VALUES),
        TEXT_OF(IDEATEK_ACTION_EDGES_VALUES),
        TEXT_OF(IDEATEK_INPUT_ACTION_STATE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_SOURCE_VALUES),
        TEXT_OF(IDEATEK_INPUT_BINDING_FIELDS),
        TEXT_OF(IDEATEK_DEAD_ZONE_KIND_VALUES),
        TEXT_OF(IDEATEK_INPUT_DEAD_ZONE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_CURVE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_SCALE_FIELDS), TEXT_OF(IDEATEK_AXES_VALUES),
        TEXT_OF(IDEATEK_INPUT_NEGATE_FIELDS), TEXT_OF(IDEATEK_SWIZZLE_VALUES),
        TEXT_OF(IDEATEK_INPUT_SWIZZLE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_PRESSED_FIELDS),
        TEXT_OF(IDEATEK_INPUT_RELEASED_FIELDS),
        TEXT_OF(IDEATEK_HOLD_FLAGS_VALUES), TEXT_OF(IDEATEK_INPUT_HOLD_FIELDS),
        TEXT_OF(IDEATEK_INPUT_TAP_FIELDS), TEXT_OF(IDEATEK_PULSE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_INPUT_PULSE_FIELDS),
        TEXT_OF(IDEATEK_CHORD_FLAGS_VALUES),
        TEXT_OF(IDEATEK_INPUT_CHORD_FIELDS),
        TEXT_OF(IDEATEK_CONTEXT_FLAGS_VALUES),
        TEXT_OF(IDEATEK_INPUT_CONTEXT_FIELDS),
        TEXT_OF(IDEATEK_INPUT_PLAYER_FIELDS),
        TEXT_OF(IDEATEK_INPUT_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_CAPTURE_SOURCES_VALUES),
        TEXT_OF(IDEATEK_INPUT_CAPTURE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_CAPTURED_FIELDS),
        TEXT_OF(IDEATEK_DEVICE_KIND_VALUES),
        TEXT_OF(IDEATEK_INPUT_LAST_DEVICE_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "action_type", show_action_type }, { "accumulate", show_accumulate },
        { "input_action", show_input_action },
        { "action_state", show_action_state },
        { "action_edges", show_action_edges },
        { "input_action_state", show_input_action_state },
        { "input_source", show_input_source },
        { "input_binding", show_input_binding },
        { "dead_zone_kind", show_dead_zone_kind },
        { "input_dead_zone", show_input_dead_zone },
        { "input_curve", show_input_curve },
        { "input_scale", show_input_scale }, { "axes", show_axes },
        { "input_negate", show_input_negate }, { "swizzle", show_swizzle },
        { "input_swizzle", show_input_swizzle },
        { "input_pressed", show_input_pressed },
        { "input_released", show_input_released },
        { "hold_flags", show_hold_flags }, { "input_hold", show_input_hold },
        { "input_tap", show_input_tap }, { "pulse_flags", show_pulse_flags },
        { "input_pulse", show_input_pulse },
        { "chord_flags", show_chord_flags },
        { "input_chord", show_input_chord },
        { "context_flags", show_context_flags },
        { "input_context", show_input_context },
        { "input_player", show_input_player },
        { "input_device", show_input_device },
        { "capture_sources", show_capture_sources },
        { "input_capture", show_input_capture },
        { "input_captured", show_input_captured },
        { "device_kind", show_device_kind },
        { "input_last_device", show_input_last_device },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
