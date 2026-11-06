/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

uint64_t find_mode_entity(const window_display_t *display,
    const SDL_DisplayMode *mode)
{
    for (uint32_t i = 0; mode && i < display->mode_count; i++)
        if (is_same_mode(&display->modes[i].mode, mode))
            return (display->modes[i].entity);
    return (0);
}

const SDL_DisplayMode *find_mode(const window_display_t *display,
    uint64_t entity)
{
    for (uint32_t i = 0; i < display->listed; i++)
        if (display->modes[i].entity == entity)
            return (&display->modes[i].mode);
    return (nullptr);
}
