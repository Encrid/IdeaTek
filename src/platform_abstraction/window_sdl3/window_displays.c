/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

static bool is_listed(SDL_DisplayID id, const SDL_DisplayID *ids, int count)
{
    for (int i = 0; i < count; i++)
        if (ids[i] == id)
            return (true);
    return (false);
}

static bool add_display(const bridge_api_t *api, SDL_DisplayID id)
{
    window_display_t *display;
    uint32_t bytes;

    if (window_module.display_count >= WINDOW_DISPLAYS) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "more than 16 displays: the display %u gets no entity", id);
        return (true);
    }
    display = &window_module.displays[window_module.display_count];
    *display = (window_display_t){ .id = id };
    display->mode_count = read_modes(api, id, display->modes,
        &display->listed);
    bytes = CHANGE_BYTES(ideatek_display_t)
        + CHANGE_BYTES(ideatek_video_mode_t) * display->mode_count;
    if (!has_room(api, 1 + display->mode_count, bytes))
        return (false);
    display->entity = spawn_entity(api);
    spawn_modes(api, display);
    window_module.display_count++;
    return (true);
}

static bool remove_display(const bridge_api_t *api, uint32_t at)
{
    window_display_t *display = &window_module.displays[at];

    if (!has_room(api, 0, CHANGE_HEAD * (1 + display->mode_count)))
        return (false);
    if (window_module.applied.target == IDEATEK_DISPLAY_TARGET_CHOSEN
        && window_module.applied.display.id == display->entity)
        window_module.moved = true;
    destroy_modes(api, display);
    api->destroy(api, display->entity);
    *display = window_module.displays[--window_module.display_count];
    return (true);
}

static bool match_list(const bridge_api_t *api, const SDL_DisplayID *ids,
    int count)
{
    bool done = true;

    for (uint32_t i = window_module.display_count; i > 0; i--)
        if (!is_listed(window_module.displays[i - 1].id, ids, count))
            done &= remove_display(api, i - 1);
    for (int i = 0; i < count; i++)
        if (!find_display(ids[i], 0))
            done &= add_display(api, ids[i]);
    return (done);
}

void sync_displays(const bridge_api_t *api)
{
    int count = 0;
    SDL_DisplayID *ids = SDL_GetDisplays(&count);
    SDL_DisplayID primary = SDL_GetPrimaryDisplay();
    bool done;

    if (!ids) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no list of the displays: %s",
            SDL_GetError());
        return;
    }
    done = match_list(api, ids, count);
    for (uint32_t i = 0; i < window_module.display_count; i++)
        done &= update_display(api, &window_module.displays[i], primary);
    window_module.displays_dirty = !done;
    SDL_free(ids);
}
