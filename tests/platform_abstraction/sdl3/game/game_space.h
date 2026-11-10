/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_SPACE_H
    #define TEST_SDL3_GAME_SPACE_H

    #include "game.h"

    #if defined(GAME_SPACE)

    #include "ideatek_space.h"

    typedef struct {
        uint32_t transform;
        uint32_t parent;
        uint32_t world;
        uint32_t children;
        uint32_t sibling;
        uint32_t previous;
        uint32_t interpolated;
        uint32_t teleported;
    } game_space_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_space_slots_t;

    #endif

    extern game_space_slots_t game_space_slots;

    bool game_space_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_space(const bridge_api_t *api);
    void game_watch_space(const bridge_api_t *api);
#endif
