/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static const input_family_t FAMILIES[] = {
    { SDL_EVENT_KEY_DOWN, SDL_EVENT_KEY_UP, 1, 44, take_key },
    { SDL_EVENT_TEXT_INPUT, SDL_EVENT_TEXT_INPUT, 1, 32, take_text },
    { SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_MOTION, 1, 48, take_motion },
    { SDL_EVENT_MOUSE_BUTTON_DOWN, SDL_EVENT_MOUSE_BUTTON_UP, 1, 40,
        take_button },
    { SDL_EVENT_MOUSE_WHEEL, SDL_EVENT_MOUSE_WHEEL, 1, 36, take_wheel },
    { SDL_EVENT_GAMEPAD_AXIS_MOTION, SDL_EVENT_GAMEPAD_AXIS_MOTION, 0, 0,
        take_axis },
    { SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_EVENT_GAMEPAD_BUTTON_UP, 1, 48,
        take_pad_button },
    { SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_ADDED, 1, 455, add_pad },
    { SDL_EVENT_GAMEPAD_REMOVED, SDL_EVENT_GAMEPAD_REMOVED, 0, 24,
        remove_pad },
    { SDL_EVENT_FINGER_DOWN, SDL_EVENT_FINGER_CANCELED, 1, 48, take_finger },
};

static const input_family_t *family_of(const SDL_Event *e)
{
    for (size_t i = 0; i < SDL_arraysize(FAMILIES); i++)
        if (e->type >= FAMILIES[i].first && e->type <= FAMILIES[i].last)
            return (&FAMILIES[i]);
    return (nullptr);
}

static bool is_motion(const bridge_api_t *api, const SDL_Event *e)
{
    if (e->type == SDL_EVENT_MOUSE_MOTION)
        return (joins_run(api, MOUSE_MOTION, e->motion.state));
    if (e->type == SDL_EVENT_FINGER_MOTION)
        return (joins_run(api, FINGER_EVENT, e->tfinger.fingerID));
    return (false);
}

static bool fits(const bridge_api_t *api, const SDL_Event *e,
    const input_family_t *f)
{
    uint32_t need;

    if (!f || is_motion(api, e))
        return (has_room(api, 0, 0));
    need = f->need;
    if (e->type == SDL_EVENT_TEXT_INPUT)
        need = count_points(e->text.text);
    return (has_room(api, need, f->bytes * SDL_max(need, 1u)));
}

void take_events(const bridge_api_t *api)
{
    SDL_Event e;
    const input_family_t *f;

    while (SDL_PeepEvents(&e, 1, SDL_PEEKEVENT, SDL_EVENT_KEY_DOWN,
        SDL_EVENT_FINGER_CANCELED) == 1) {
        f = family_of(&e);
        if (!fits(api, &e, f))
            break;
        SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EVENT_KEY_DOWN,
            SDL_EVENT_FINGER_CANCELED);
        if (!is_motion(api, &e))
            flush_run(api);
        if (f)
            f->take(api, &e);
    }
    flush_run(api);
}
