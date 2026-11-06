/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "input_inside.h"

float to_pixels(SDL_WindowID id, float place)
{
    SDL_Window *window = id ? SDL_GetWindowFromID(id) : nullptr;

    if (!window)
        return (place);
    return (place * SDL_GetWindowPixelDensity(window));
}

void take_motion(const bridge_api_t *api, const SDL_Event *e)
{
    const SDL_MouseMotionEvent *m = &e->motion;
    ideatek_mouse_motion_t *v = &input_module.run.value.motion;

    if (!joins_run(api, MOUSE_MOTION, m->state)) {
        start_run(api, MOUSE_MOTION, m->state, sizeof(*v));
        memset(v, 0, sizeof(*v));
        v->buttons = m->state;
        v->order = next_order();
    }
    v->x = to_pixels(m->windowID, m->x);
    v->y = to_pixels(m->windowID, m->y);
    v->rel_x += to_pixels(m->windowID, m->xrel);
    v->rel_y += to_pixels(m->windowID, m->yrel);
    input_module.next_mouse.x = v->x;
    input_module.next_mouse.y = v->y;
    input_module.next_mouse.buttons = m->state;
}

void take_button(const bridge_api_t *api, const SDL_Event *e)
{
    const SDL_MouseButtonEvent *b = &e->button;
    ideatek_mouse_button_t event = {};
    uint32_t bit = 1u << ((b->button - 1u) & 31u);

    event.x = to_pixels(b->windowID, b->x);
    event.y = to_pixels(b->windowID, b->y);
    event.button = b->button;
    event.down = b->down;
    event.clicks = b->clicks;
    event.order = next_order();
    spawn_event(api, MOUSE_BUTTON, &event, sizeof(event));
    if (b->down)
        input_module.next_mouse.buttons |= bit;
    else
        input_module.next_mouse.buttons &= ~bit;
    input_module.next_mouse.x = event.x;
    input_module.next_mouse.y = event.y;
}

void take_wheel(const bridge_api_t *api, const SDL_Event *e)
{
    const SDL_MouseWheelEvent *w = &e->wheel;
    float sign = w->direction == SDL_MOUSEWHEEL_FLIPPED ? -1.0F : 1.0F;
    ideatek_mouse_wheel_t event = { .ticks_x = w->x * sign,
        .ticks_y = w->y * sign };

    event.order = next_order();
    spawn_event(api, MOUSE_WHEEL, &event, sizeof(event));
}
