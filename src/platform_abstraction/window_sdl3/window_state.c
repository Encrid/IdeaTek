/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static void read_hdr(ideatek_window_state_t *s)
{
    SDL_PropertiesID props = SDL_GetWindowProperties(window_module.window);

    s->hdr = SDL_GetBooleanProperty(props, SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN,
        false);
    s->sdr_white_level = s->hdr ? SDL_GetFloatProperty(props,
        SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT, 1.0F) : 1.0F;
    s->hdr_headroom = s->hdr ? SDL_GetFloatProperty(props,
        SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT, 1.0F) : 1.0F;
}

static void read_mode(ideatek_window_state_t *s, SDL_WindowFlags flags)
{
    SDL_Window *w = window_module.window;
    const SDL_DisplayMode *exclusive = SDL_GetWindowFullscreenMode(w);
    window_display_t *display = find_display(SDL_GetDisplayForWindow(w), 0);

    s->display.id = display ? display->entity : 0;
    s->mode = IDEATEK_WINDOW_MODE_WINDOWED;
    if ((flags & SDL_WINDOW_FULLSCREEN) && exclusive)
        s->mode = IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN;
    else if (flags & SDL_WINDOW_FULLSCREEN)
        s->mode = IDEATEK_WINDOW_MODE_FULLSCREEN;
    s->video_mode.id = 0;
    if (display && s->mode == IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN)
        s->video_mode.id = find_mode_entity(display, exclusive);
}

static void build_state(ideatek_window_state_t *s)
{
    SDL_Window *w = window_module.window;
    SDL_WindowFlags flags = SDL_GetWindowFlags(w);
    int size[4] = {};

    read_mode(s, flags);
    SDL_GetWindowSize(w, &size[0], &size[1]);
    SDL_GetWindowSizeInPixels(w, &size[2], &size[3]);
    s->width = (uint32_t)size[0];
    s->height = (uint32_t)size[1];
    s->pixel_width = (uint32_t)size[2];
    s->pixel_height = (uint32_t)size[3];
    s->display_scale = SDL_GetWindowDisplayScale(w);
    s->drop_x = window_module.drop_x;
    s->drop_y = window_module.drop_y;
    s->focused = flags & SDL_WINDOW_INPUT_FOCUS;
    s->minimized = flags & SDL_WINDOW_MINIMIZED;
    s->close_asked = window_module.close_asked;
    s->drop_over = window_module.drop_over;
    read_hdr(s);
}

void attach_state(const bridge_api_t *api)
{
    ideatek_window_state_t next;

    if (!window_module.window || !window_module.dirty || !has_room(api, 0, 104))
        return;
    next = window_module.state;
    build_state(&next);
    if (memcmp(&next, &window_module.state, sizeof(next))) {
        BRIDGE_NATIVE_ATTACH(api, window_module.entity, STATE, next);
        window_module.state = next;
    }
    window_module.dirty = false;
}
