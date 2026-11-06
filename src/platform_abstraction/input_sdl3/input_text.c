/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static void apply_text(const bridge_api_t *api, uint64_t id)
{
    const ideatek_text_request_t *r = (const void *)(api->shared_base
        + bridge_native_find_value(api, id, TEXT_REQUEST));
    SDL_Window *w = first_window();
    float d = w ? SDL_GetWindowPixelDensity(w) : 1.0F;
    SDL_Rect area = { (int)(r->x / d), (int)(r->y / d), (int)(r->width / d),
        (int)(r->height / d) };

    SDL_SetHint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, r->screen_keyboard ? "1"
        : "0");
    if (input_module.text_on && r->screen_keyboard
        != input_module.screen_keyboard)
        input_module.text_on = !SDL_StopTextInput(w);
    input_module.screen_keyboard = r->screen_keyboard;
    if (w)
        SDL_SetTextInputArea(w, &area, (int)(r->cursor / d));
    if (!input_module.text_on && w && SDL_StartTextInput(w))
        input_module.text_on = true;
    else if (!input_module.text_on)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no text input: %s",
            SDL_GetError());
}

BRIDGE_NATIVE(text)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        input_module.text_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);
    SDL_Window *w = first_window();

    for (uint32_t i = 0; i < list->count; i++) {
        if (t[i].id != input_module.entity)
            continue;
        if (t[i].events != BRIDGE_ON_LEAVE)
            apply_text(api, t[i].id);
        else if (input_module.text_on)
            input_module.text_on = !SDL_StopTextInput(w);
    }
    return (0);
}

uint32_t count_points(const char *text)
{
    uint32_t count = 0;

    for (; *text; count++)
        SDL_StepUTF8(&text, nullptr);
    return (count);
}

void take_text(const bridge_api_t *api, const SDL_Event *e)
{
    const char *at = e->text.text;
    ideatek_text_event_t event = {};

    while (*at) {
        event.code_point = SDL_StepUTF8(&at, nullptr);
        event.order = next_order();
        spawn_event(api, TEXT_EVENT, &event, sizeof(event));
    }
}
