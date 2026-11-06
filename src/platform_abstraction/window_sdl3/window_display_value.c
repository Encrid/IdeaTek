/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static bool has_new_modes(const window_display_t *display,
    const window_mode_t *modes, uint32_t count)
{
    if (count != display->mode_count)
        return (true);
    for (uint32_t i = 0; i < count; i++)
        if (!find_mode_entity(display, &modes[i].mode))
            return (true);
    return (false);
}

static bool renew_modes(const bridge_api_t *api, window_display_t *display)
{
    static window_mode_t modes[WINDOW_MODES + 2];
    uint32_t listed = 0;
    uint32_t count = read_modes(api, display->id, modes, &listed);

    if (!has_new_modes(display, modes, count))
        return (true);
    if (!has_room(api, count, 24 * display->mode_count + 56 * count))
        return (false);
    destroy_modes(api, display);
    memcpy(display->modes, modes, count * sizeof(modes[0]));
    display->mode_count = count;
    display->listed = listed;
    spawn_modes(api, display);
    return (true);
}

static ideatek_display_t read_display(const bridge_api_t *api,
    const window_display_t *display)
{
    ideatek_display_t value = {};
    SDL_Rect bounds = {};
    const char *name = SDL_GetDisplayName(display->id);
    size_t len = name ? SDL_strlen(name) : 0;

    if (SDL_utf8strlcpy(value.name, name ? name : "", sizeof(value.name)) < len)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the name of the display %u is longer than 127 bytes",
            display->id);
    value.desktop_mode.id = find_mode_entity(display,
        SDL_GetDesktopDisplayMode(display->id));
    value.current_mode.id = find_mode_entity(display,
        SDL_GetCurrentDisplayMode(display->id));
    SDL_GetDisplayBounds(display->id, &bounds);
    value.width = (uint32_t)bounds.w;
    value.height = (uint32_t)bounds.h;
    value.content_scale = SDL_GetDisplayContentScale(display->id);
    value.hdr = SDL_GetBooleanProperty(SDL_GetDisplayProperties(display->id),
        SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN, false);
    return (value);
}

bool update_display(const bridge_api_t *api, window_display_t *display,
    SDL_DisplayID primary)
{
    ideatek_display_t value;

    if (!renew_modes(api, display))
        return (false);
    value = read_display(api, display);
    value.is_primary = display->id == primary;
    if (!memcmp(&value, &display->attached, sizeof(value)))
        return (true);
    if (!has_room(api, 0, 184))
        return (false);
    BRIDGE_NATIVE_ATTACH(api, display->entity, DISPLAY, value);
    display->attached = value;
    return (true);
}

window_display_t *find_display(SDL_DisplayID id, uint64_t entity)
{
    for (uint32_t i = 0; i < window_module.display_count; i++)
        if ((id && window_module.displays[i].id == id)
            || (entity && window_module.displays[i].entity == entity))
            return (&window_module.displays[i]);
    return (nullptr);
}
