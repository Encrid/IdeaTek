/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_SDL3_START_H
    #define BL_IDEATEK_SDL3_START_H

    #include <stddef.h>

    #include <SDL3/SDL_init.h>

bool sdl3_start_subsystem(SDL_InitFlags flags, char *reason, size_t size);
void sdl3_stop_subsystem(SDL_InitFlags flags);

#endif
