/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

static uint32_t add_extras(SDL_DisplayID id, window_mode_t *modes,
    uint32_t count)
{
    const SDL_DisplayMode *extra[2] = { SDL_GetDesktopDisplayMode(id),
        SDL_GetCurrentDisplayMode(id) };
    bool found;

    for (uint32_t e = 0; e < 2; e++) {
        found = !extra[e] || count >= WINDOW_MODES + 2;
        for (uint32_t i = 0; !found && i < count; i++)
            found = is_same_mode(&modes[i].mode, extra[e]);
        if (!found)
            modes[count++] = (window_mode_t){ .mode = *extra[e] };
    }
    return (count);
}

uint32_t read_modes(const bridge_api_t *api, SDL_DisplayID id,
    window_mode_t *modes, uint32_t *listed)
{
    int got = 0;
    SDL_DisplayMode **list = SDL_GetFullscreenDisplayModes(id, &got);
    uint32_t count = 0;

    if (!list)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no video modes of the display %u: %s", id, SDL_GetError());
    while (list && count < (uint32_t)got && count < WINDOW_MODES) {
        modes[count] = (window_mode_t){ .mode = *list[count] };
        count++;
    }
    if (list && (uint32_t)got > WINDOW_MODES)
        bridge_native_say(api, BRIDGE_LOG_WARN, "the display %u has %d video "
            "modes, and the module keeps the first 255", id, got);
    *listed = count;
    SDL_free(list);
    return (add_extras(id, modes, count));
}

void spawn_modes(const bridge_api_t *api, window_display_t *display)
{
    const SDL_DisplayMode *m;
    ideatek_video_mode_t value = { .display = { display->entity } };

    for (uint32_t i = 0; i < display->mode_count; i++) {
        m = &display->modes[i].mode;
        value.width = (uint32_t)m->w;
        value.height = (uint32_t)m->h;
        value.pixel_density = m->pixel_density;
        value.refresh_numerator = m->refresh_rate_numerator;
        value.refresh_denominator = m->refresh_rate_numerator
            ? m->refresh_rate_denominator : 1;
        value.order = i;
        display->modes[i].entity = spawn_entity(api);
        BRIDGE_NATIVE_ATTACH(api, display->modes[i].entity, VIDEO_MODE, value);
    }
}

void destroy_modes(const bridge_api_t *api, window_display_t *display)
{
    for (uint32_t i = 0; i < display->mode_count; i++)
        api->destroy(api, display->modes[i].entity);
    display->mode_count = 0;
    display->listed = 0;
}
