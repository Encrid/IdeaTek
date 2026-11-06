/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "input_inside.h"

BRIDGE_NATIVE(events)
{
    if (!input_module.started && !start_input(api))
        return (1);
    turn_items(api);
    if (input_module.focused && !input_module.was_focused)
        spawn_focus(api, IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED);
    take_events(api);
    if (!input_module.was_focused || input_module.focused || release_all(api))
        input_module.was_focused = input_module.focused;
    attach_states(api);
    return (0);
}

BRIDGE_NATIVE(window)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        input_module.window_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);
    uint32_t at;

    for (uint32_t i = 0; i < list->count; i++) {
        at = bridge_native_find_value(api, t[i].id, WINDOW_STATE);
        input_module.focused = ((const ideatek_window_state_t *)(
            api->shared_base + at))->focused;
    }
    return (0);
}

static bool take_change(const bridge_api_t *api, void *last,
    const void *next, uint32_t size)
{
    if (!memcmp(last, next, size) || !has_room(api, 0, 24 + size))
        return (false);
    memcpy(last, next, size);
    return (true);
}

void attach_states(const bridge_api_t *api)
{
    input_module_t *m = &input_module;
    input_pad_t *p;

    if (take_change(api, &m->keyboard, &m->next_keyboard, sizeof(m->keyboard)))
        BRIDGE_NATIVE_ATTACH(api, m->entity, KEYBOARD, m->keyboard);
    if (take_change(api, &m->mouse, &m->next_mouse, sizeof(m->mouse)))
        BRIDGE_NATIVE_ATTACH(api, m->entity, MOUSE, m->mouse);
    for (uint32_t i = 0; i < m->pad_count; i++) {
        p = &m->pads[i];
        if (take_change(api, &p->last, &p->next, sizeof(p->last)))
            BRIDGE_NATIVE_ATTACH(api, p->entity, GAMEPAD_STATE, p->last);
    }
}
