/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_BENCH_H
    #define TEST_SDL3_GAME_BENCH_H

    #include "game_space.h"

    bool game_bench_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_bench_play(const bridge_api_t *api);
    void game_bench_end(const bridge_api_t *api);
#endif
