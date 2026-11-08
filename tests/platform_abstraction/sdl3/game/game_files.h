/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_FILES_H
    #define TEST_SDL3_GAME_FILES_H

    #include "game.h"

    #if defined(GAME_FILES)

    #include "ideatek_files.h"

    typedef struct {
        uint32_t request;
        uint32_t result;
        uint32_t change;
    } game_files_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_files_slots_t;

    #endif

    extern game_files_slots_t game_files_slots;

    bool game_files_step(const bridge_api_t *api, const char *verb, char *args);
    void game_declare_files(const bridge_api_t *api);
    void game_watch_files(const bridge_api_t *api);
#endif
