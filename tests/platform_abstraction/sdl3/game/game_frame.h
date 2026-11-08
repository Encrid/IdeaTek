/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_FRAME_H
    #define TEST_SDL3_GAME_FRAME_H

    #include "game.h"

    #if defined(GAME_FRAME)

    #include "ideatek_frame.h"

    typedef struct {
        uint32_t frame;
        uint32_t tick;
        uint32_t request;
    } game_frame_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_frame_slots_t;

    #endif

    extern game_frame_slots_t game_frame_slots;

    bool game_frame_step(const bridge_api_t *api, const char *verb, char *args);
    void game_declare_frame(const bridge_api_t *api);
    void game_watch_frame(const bridge_api_t *api);
#endif
