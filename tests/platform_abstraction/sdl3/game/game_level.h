/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_LEVEL_H
    #define TEST_SDL3_GAME_LEVEL_H

    #include "game.h"

    bool game_level_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_level_play(const bridge_api_t *api);
    void game_watch_level(const bridge_api_t *api);
    void game_level_end(const bridge_api_t *api);
#endif
