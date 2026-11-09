/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_AUDIO_H
    #define TEST_SDL3_GAME_AUDIO_H

    #include "game.h"

    #if defined(GAME_AUDIO)

    #include "ideatek_audio.h"

    typedef struct {
        uint32_t device;
        uint32_t request;
        uint32_t frames;
        uint32_t recorded;
        uint32_t state;
    } game_audio_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_audio_slots_t;

    #endif

    extern game_audio_slots_t game_audio_slots;

    bool game_audio_step(const bridge_api_t *api, const char *verb, char *args);
    void game_declare_audio(const bridge_api_t *api);
    void game_watch_audio(const bridge_api_t *api);
#endif
