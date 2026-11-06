/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_H
    #define TEST_SDL3_GAME_H

    #include <SDL3/SDL.h>

    #include "bridge_native.h"
    #include "ideatek_window.h"
    #include "ideatek_window_clipboard.h"

    #define GAME_SETTINGS_FIELDS { \
        bridge_text_t script; \
    }
BRIDGE_SCHEMA_STRUCT_TYPE(game_settings_t, GAME_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(game_settings_t, GAME_SETTINGS_FIELDS);

typedef struct {
    uint32_t request;
    uint32_t state;
    uint32_t display;
    uint32_t video_mode;
    uint32_t drop;
    uint32_t clipboard;
    uint32_t paste;
    uint32_t event_wait;
    uint32_t settings;
} game_slots_t;

typedef struct {
    uint64_t entity;
    char name[128];
} game_display_t;

typedef struct {
    uint64_t display;
    uint64_t entity;
    uint32_t order;
} game_mode_t;

typedef struct {
    game_slots_t slots;
    char script[65536];
    uint64_t window;
    ideatek_window_request_t request;
    game_display_t displays[32];
    uint32_t display_count;
    game_mode_t modes[1024];
    uint32_t mode_count;
    uint32_t round;
    uint64_t last;
    uint64_t second;
    uint32_t first_drop;
    uint32_t first_paste;
} game_t;

extern game_t game;

const void *game_cell(const bridge_api_t *api, uint64_t id, uint32_t slot);
uint64_t game_find_display(const char *name);
void game_start(const bridge_api_t *api);
void game_play(const bridge_api_t *api);

#endif
