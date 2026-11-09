/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_STATES_H
    #define TEST_SDL3_GAME_STATES_H

    #include "game.h"

    #if defined(GAME_STATES)

    #include "ideatek_states.h"

    typedef struct {
        uint32_t state;
        uint32_t request;
        uint32_t change;
        uint32_t scoped;
    } game_states_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_states_slots_t;

    #endif

    extern game_states_slots_t game_states_slots;

    bool game_states_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_states(const bridge_api_t *api);
    void game_watch_states(const bridge_api_t *api);
#endif
