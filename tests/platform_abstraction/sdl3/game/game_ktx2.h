/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_KTX2_H
    #define TEST_SDL3_GAME_KTX2_H

    #include "game.h"

    #if defined(GAME_KTX2)

    #include "ideatek_rendering.h"

    typedef struct {
        uint32_t dimension;
        uint32_t filter;
        uint32_t wrap;
        uint32_t texture;
        uint32_t sampler;
    } game_ktx2_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_ktx2_slots_t;

    #endif

    extern game_ktx2_slots_t game_ktx2_slots;

    bool game_ktx2_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_ktx2(const bridge_api_t *api);
    void game_watch_ktx2(const bridge_api_t *api);
#endif
