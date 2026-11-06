/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static const uint32_t BITS[] = {
    IDEATEK_GAMEPAD_BUTTON_SOUTH, IDEATEK_GAMEPAD_BUTTON_EAST,
    IDEATEK_GAMEPAD_BUTTON_WEST, IDEATEK_GAMEPAD_BUTTON_NORTH,
    IDEATEK_GAMEPAD_BUTTON_BACK, IDEATEK_GAMEPAD_BUTTON_GUIDE,
    IDEATEK_GAMEPAD_BUTTON_START, IDEATEK_GAMEPAD_BUTTON_LEFT_STICK,
    IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK, IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER,
    IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER, IDEATEK_GAMEPAD_BUTTON_DPAD_UP,
    IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN, IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT,
    IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT,
};

input_pad_t *find_pad(SDL_JoystickID id, uint64_t entity)
{
    for (uint32_t i = 0; i < input_module.pad_count; i++)
        if ((id && input_module.pads[i].id == id)
            || (entity && input_module.pads[i].entity == entity))
            return (&input_module.pads[i]);
    return (nullptr);
}

void take_axis([[maybe_unused]] const bridge_api_t *api, const SDL_Event *e)
{
    input_pad_t *p = find_pad(e->gaxis.which, 0);
    float value = (float)e->gaxis.value / 32767.0F;
    float *fields[SDL_GAMEPAD_AXIS_COUNT];

    if (!p || e->gaxis.axis >= SDL_GAMEPAD_AXIS_COUNT)
        return;
    fields[0] = &p->next.left_x;
    fields[1] = &p->next.left_y;
    fields[2] = &p->next.right_x;
    fields[3] = &p->next.right_y;
    fields[4] = &p->next.left_trigger;
    fields[5] = &p->next.right_trigger;
    *fields[e->gaxis.axis] = SDL_clamp(value, e->gaxis.axis
        >= SDL_GAMEPAD_AXIS_LEFT_TRIGGER ? 0.0F : -1.0F, 1.0F);
}

void take_pad_button(const bridge_api_t *api, const SDL_Event *e)
{
    input_pad_t *p = find_pad(e->gbutton.which, 0);
    ideatek_gamepad_event_t event = {};

    if (!p || e->gbutton.button >= SDL_arraysize(BITS))
        return;
    event.gamepad.id = p->entity;
    event.button = BITS[e->gbutton.button];
    event.down = e->gbutton.down;
    event.order = next_order();
    spawn_event(api, GAMEPAD_EVENT, &event, sizeof(event));
    if (event.down)
        p->next.buttons |= event.button;
    else
        p->next.buttons &= ~event.button;
}

static void release_pad(const bridge_api_t *api, input_pad_t *p)
{
    ideatek_gamepad_event_t event = {};

    for (uint32_t bit = 1; p->next.buttons; bit <<= 1) {
        if (!(p->next.buttons & bit))
            continue;
        event.gamepad.id = p->entity;
        event.button = bit;
        event.order = next_order();
        spawn_event(api, GAMEPAD_EVENT, &event, sizeof(event));
        p->next.buttons &= ~bit;
    }
}

void release_pads(const bridge_api_t *api)
{
    for (uint32_t i = 0; i < input_module.pad_count; i++)
        release_pad(api, &input_module.pads[i]);
}
