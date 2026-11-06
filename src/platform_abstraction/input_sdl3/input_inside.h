/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef INPUT_INSIDE_H
    #define INPUT_INSIDE_H

    #include <SDL3/SDL.h>

    #include "bridge_native.h"
    #include "ideatek_input.h"
    #include "ideatek_input_gamepad.h"
    #include "ideatek_input_keyboard.h"
    #include "ideatek_input_mouse.h"
    #include "ideatek_input_text.h"
    #include "ideatek_input_touch.h"
    #include "ideatek_window.h"
    #include "sdl3_start.h"

constexpr uint32_t INPUT_EVENT = 9;
constexpr uint32_t KEYBOARD = 10;
constexpr uint32_t KEY_EVENT = 11;
constexpr uint32_t TEXT_REQUEST = 12;
constexpr uint32_t TEXT_EVENT = 13;
constexpr uint32_t MOUSE = 14;
constexpr uint32_t MOUSE_MOTION = 15;
constexpr uint32_t MOUSE_BUTTON = 16;
constexpr uint32_t MOUSE_WHEEL = 17;
constexpr uint32_t MOUSE_REQUEST = 18;
constexpr uint32_t GAMEPAD = 19;
constexpr uint32_t GAMEPAD_STATE = 20;
constexpr uint32_t GAMEPAD_EVENT = 21;
constexpr uint32_t GAMEPAD_REQUEST = 22;
constexpr uint32_t FINGER_EVENT = 23;
constexpr uint32_t WINDOW_STATE = 24;
constexpr uint32_t SETTINGS = 25;

constexpr uint32_t INPUT_GAMEPADS = 16;
constexpr uint32_t INPUT_EVENTS = 65535;
constexpr uint32_t INPUT_FINGERS = 16;
constexpr uint32_t INPUT_REASON = 256;
constexpr uint32_t INPUT_CURSORS = 11;

    #define INPUT_SETTINGS_FIELDS { \
        bridge_text_t mappings; \
        uint32_t round_entities; \
        bool merge_motion; \
        bool touch_mouse_events; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(input_settings_t, INPUT_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(input_settings_t, INPUT_SETTINGS_FIELDS);

typedef struct {
    SDL_JoystickID id;
    SDL_Gamepad *gamepad;
    uint64_t entity;
    ideatek_gamepad_state_t last;
    ideatek_gamepad_state_t next;
} input_pad_t;

typedef struct {
    SDL_FingerID id;
    float x;
    float y;
} input_finger_t;

typedef struct {
    uint32_t slot;
    uint64_t key;
    uint64_t entity;
    uint32_t size;
    union {
        ideatek_mouse_motion_t motion;
        ideatek_finger_event_t finger;
    } value;
    bool pending;
} input_run_t;

typedef void (*input_take_t)(const bridge_api_t *api, const SDL_Event *e);

typedef struct {
    uint32_t first;
    uint32_t last;
    uint32_t need;
    uint32_t bytes;
    input_take_t take;
} input_family_t;

typedef struct {
    uint64_t entity;

    ideatek_keyboard_t keyboard;
    ideatek_keyboard_t next_keyboard;
    ideatek_mouse_t mouse;
    ideatek_mouse_t next_mouse;

    input_pad_t pads[INPUT_GAMEPADS];
    input_finger_t fingers[INPUT_FINGERS];
    SDL_Cursor *cursors[INPUT_CURSORS];

    uint64_t items[INPUT_EVENTS];
    input_run_t run;

    uint32_t item_count;
    uint32_t order;
    uint32_t spawned;
    uint32_t pad_count;
    uint32_t finger_count;
    uint32_t window_list;
    uint32_t text_list;
    uint32_t mouse_list;
    uint32_t pad_list;

    bool focused;
    bool was_focused;
    bool started;
    bool text_on;
    bool screen_keyboard;
} input_module_t;

extern input_module_t input_module;

void declare_types(const bridge_api_t *api);
void declare_pads(const bridge_api_t *api);

bool start_input(const bridge_api_t *api);

void attach_states(const bridge_api_t *api);

void take_events(const bridge_api_t *api);

bool has_room(const bridge_api_t *api, uint32_t spawns, uint32_t bytes);
uint32_t next_order(void);
void spawn_event(const bridge_api_t *api, uint32_t slot, const void *value,
    uint32_t size);
void turn_items(const bridge_api_t *api);
uint64_t spawn_entity(const bridge_api_t *api);

bool joins_run(const bridge_api_t *api, uint32_t slot, uint64_t key);
void start_run(const bridge_api_t *api, uint32_t slot, uint64_t key,
    uint32_t size);
void flush_run(const bridge_api_t *api);

uint32_t usage_of_scancode(SDL_Scancode scancode);
uint32_t usage_of_keycode(SDL_Keycode keycode);
uint32_t character_of(SDL_Scancode scancode, SDL_Keymod mod);

ideatek_modifier_t modifiers_of(SDL_Keymod mod);
void take_key(const bridge_api_t *api, const SDL_Event *e);
void release_keys(const bridge_api_t *api);

float to_pixels(SDL_WindowID id, float place);
void take_motion(const bridge_api_t *api, const SDL_Event *e);
void take_button(const bridge_api_t *api, const SDL_Event *e);
void take_wheel(const bridge_api_t *api, const SDL_Event *e);

uint32_t count_points(const char *text);
void take_text(const bridge_api_t *api, const SDL_Event *e);

input_pad_t *find_pad(SDL_JoystickID id, uint64_t entity);
void add_pad(const bridge_api_t *api, const SDL_Event *e);
void remove_pad(const bridge_api_t *api, const SDL_Event *e);

void take_axis(const bridge_api_t *api, const SDL_Event *e);
void take_pad_button(const bridge_api_t *api, const SDL_Event *e);
void release_pads(const bridge_api_t *api);

void take_finger(const bridge_api_t *api, const SDL_Event *e);
void cancel_fingers(const bridge_api_t *api);

void spawn_focus(const bridge_api_t *api, ideatek_input_event_kind_t kind);
bool release_all(const bridge_api_t *api);

SDL_Window *first_window(void);

#endif
