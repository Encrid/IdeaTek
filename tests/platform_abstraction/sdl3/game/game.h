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
    #include "ideatek_input.h"
    #include "ideatek_input_gamepad.h"
    #include "ideatek_input_keyboard.h"
    #include "ideatek_input_mouse.h"
    #include "ideatek_input_text.h"
    #include "ideatek_input_touch.h"

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
    uint32_t input_event;
    uint32_t keyboard;
    uint32_t key_event;
    uint32_t text_request;
    uint32_t text_event;
    uint32_t mouse;
    uint32_t mouse_motion;
    uint32_t mouse_button;
    uint32_t mouse_wheel;
    uint32_t mouse_request;
    uint32_t gamepad;
    uint32_t gamepad_state;
    uint32_t gamepad_event;
    uint32_t gamepad_request;
    uint32_t finger_event;
    uint32_t lists[13];
    uint32_t query;
} game_input_t;

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
    game_input_t input;
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
    uint64_t input_entity;
    uint64_t pad_entity;
    SDL_JoystickID pad_ids[20];
    SDL_Joystick *pads[20];
    uint32_t pad_count;
    float rumble_low;
    float rumble_high;
    uint32_t rumble_ms;
} game_t;

extern game_t game;

typedef void (*game_show_t)(const bridge_api_t *, uint64_t, const void *);

const void *game_cell(const bridge_api_t *api, uint64_t id, uint32_t slot);
uint32_t game_rows(const bridge_api_t *api, uint32_t query, uint32_t slot,
    game_show_t show);
uint64_t game_find_display(const char *name);
void game_start(const bridge_api_t *api);
void game_play(const bridge_api_t *api);
void game_declare_input(const bridge_api_t *api);
void game_watch_input(const bridge_api_t *api);
bool game_input_step(const bridge_api_t *api, const char *verb, char *args);

#endif
