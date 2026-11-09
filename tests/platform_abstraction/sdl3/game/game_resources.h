/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_RESOURCES_H
    #define TEST_SDL3_GAME_RESOURCES_H

    #include "game.h"

    #if defined(GAME_RESOURCES)

    #include "ideatek_resources.h"

    #define GAME_TEXT_FIELDS { \
        uint32_t bytes; \
    }
BRIDGE_SCHEMA_STRUCT_TYPE(game_text_t, GAME_TEXT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(game_text_t, GAME_TEXT_FIELDS);

    typedef struct {
        uint32_t load_state;
        uint32_t error;
        uint32_t load;
        uint32_t add;
        uint32_t handle;
        uint32_t resource;
        uint32_t loader;
        uint32_t bytes;
        uint32_t read;
        uint32_t outcome;
        uint32_t label;
        uint32_t raw;
        uint32_t source;
        uint32_t source_read;
        uint32_t source_result;
        uint32_t reload;
        uint32_t modified;
        uint32_t ready;
        uint32_t failed;
        uint32_t variable;
        uint32_t text;
    } game_resources_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_resources_slots_t;

    #endif

    extern game_resources_slots_t game_resources_slots;

    bool game_resources_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_resources(const bridge_api_t *api);
    void game_watch_resources(const bridge_api_t *api);
#endif
