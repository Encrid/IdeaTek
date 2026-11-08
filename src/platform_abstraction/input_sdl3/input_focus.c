/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

void spawn_focus(const bridge_api_t *api, ideatek_input_event_kind_t kind)
{
    ideatek_input_event_t event = { .kind = kind };

    event.order = next_order();
    spawn_event(api, INPUT_EVENT, &event, sizeof(event));
}

static void release_buttons(const bridge_api_t *api)
{
    ideatek_mouse_button_t event = { .x = input_module.next_mouse.x,
        .y = input_module.next_mouse.y };

    for (uint32_t b = 0; b < 32; b++) {
        if (!(input_module.next_mouse.buttons >> b & 1u))
            continue;
        event.button = (uint8_t)(b + 1);
        event.order = next_order();
        spawn_event(api, MOUSE_BUTTON, &event, sizeof(event));
    }
    input_module.next_mouse.buttons = 0;
}

static uint32_t count_bits(uint64_t bits)
{
    uint32_t count = 0;

    for (; bits; bits &= bits - 1)
        count++;
    return (count);
}

static uint32_t count_downs(void)
{
    const uint64_t *keys = input_module.next_keyboard.keys;
    uint32_t count = 1 + input_module.finger_count
        + count_bits(input_module.next_mouse.buttons);

    for (uint32_t i = 0; i < 4; i++)
        count += count_bits(keys[i]);
    for (uint32_t i = 0; i < input_module.pad_count; i++)
        count += count_bits(input_module.pads[i].next.buttons);
    return (count);
}

bool release_all(const bridge_api_t *api)
{
    uint32_t count = count_downs();

    if (!has_room(api, count, count * RELEASE_BYTES))
        return (false);
    release_keys(api);
    release_buttons(api);
    cancel_fingers(api);
    release_pads(api);
    spawn_focus(api, IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST);
    return (true);
}
