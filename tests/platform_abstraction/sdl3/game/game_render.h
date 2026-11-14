/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_RENDER_H
    #define TEST_SDL3_GAME_RENDER_H

    #include "game.h"

    #if defined(GAME_RENDER)

    #include "ideatek_rendering.h"

    typedef struct {
        uint32_t camera;
        uint32_t projection;
        uint32_t request;
        uint32_t capture;
        uint32_t stats;
    } game_render_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_render_slots_t;

    #endif

    extern game_render_slots_t game_render_slots;

    bool game_render_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_render(const bridge_api_t *api);
    void game_watch_render(const bridge_api_t *api);
#endif
