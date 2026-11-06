/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef WINDOW_INSIDE_H
    #define WINDOW_INSIDE_H

    #include <SDL3/SDL.h>

    #include "bridge_native.h"
    #include "ideatek_window.h"
    #include "ideatek_window_clipboard.h"
    #include "sdl3_start.h"

constexpr uint32_t REQUEST = 5;
constexpr uint32_t STATE = 6;
constexpr uint32_t DISPLAY = 7;
constexpr uint32_t VIDEO_MODE = 8;
constexpr uint32_t DROP = 9;
constexpr uint32_t CLIPBOARD = 10;
constexpr uint32_t PASTE = 11;
constexpr uint32_t EVENT_WAIT = 12;
constexpr uint32_t SETTINGS = 13;

constexpr uint32_t WINDOW_SPAWNS = 256;
constexpr uint32_t WINDOW_DISPLAYS = 16;
constexpr uint32_t WINDOW_MODES = 253;
constexpr uint32_t WINDOW_PEEKS = 64;
constexpr uint32_t WINDOW_REASON = 256;

    #define WINDOW_SETTINGS_FIELDS { \
        bridge_text_t icon; \
        uint32_t drop_bytes; \
        uint32_t clipboard_bytes; \
        bool close_on_request; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(window_settings_t, WINDOW_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(window_settings_t, WINDOW_SETTINGS_FIELDS);

typedef struct {
    uint32_t at;
    uint32_t size;
    uint32_t used;
    uint32_t side;
} window_half_t;

typedef struct {
    SDL_DisplayMode mode;
    uint64_t entity;
} window_mode_t;

typedef struct {
    SDL_DisplayID id;
    uint64_t entity;
    window_mode_t modes[WINDOW_MODES + 2];
    uint32_t mode_count;
    uint32_t listed;
    ideatek_display_t attached;
} window_display_t;

typedef struct {
    const char *name;
    ideatek_surface_t surface;
    SDL_WindowFlags flags;
    SDL_Keymod paste;
    const char *display_key;
    const char *window_key;
} window_driver_t;

typedef struct {
    SDL_Window *window;
    SDL_MetalView view;
    const window_driver_t *driver;

    uint64_t entity;
    ideatek_window_request_t applied;
    ideatek_window_state_t state;

    window_display_t displays[WINDOW_DISPLAYS];

    window_half_t drops;
    window_half_t pastes;
    uint64_t items[WINDOW_SPAWNS];

    uint64_t taken;
    uint32_t request_list;
    uint32_t clipboard_list;
    uint32_t display_count;
    uint32_t item_count;
    uint32_t spawned;
    uint32_t set;
    uint32_t index;
    uint32_t paste_count;

    float drop_x;
    float drop_y;

    bool pending;
    bool moved;
    bool dirty;
    bool close_asked;
    bool drop_over;
    bool displays_dirty;
} window_module_t;

extern window_module_t window_module;

static inline bool is_same_mode(const SDL_DisplayMode *a,
    const SDL_DisplayMode *b)
{
    return (a->w == b->w && a->h == b->h
        && a->pixel_density == b->pixel_density
        && a->refresh_rate_numerator == b->refresh_rate_numerator
        && a->refresh_rate_denominator == b->refresh_rate_denominator);
}

void declare_types(const bridge_api_t *api);

void enable_events(void);

void apply_request(const bridge_api_t *api);
window_display_t *get_target(const ideatek_window_request_t *request);

bool open_window(const bridge_api_t *api,
    const ideatek_window_request_t *request);
void set_mode(const bridge_api_t *api, const ideatek_window_request_t *request,
    const window_display_t *display);

void close_window(void);

const window_driver_t *find_driver(void);

void read_handles(const bridge_api_t *api);

void attach_state(const bridge_api_t *api);

void sync_displays(const bridge_api_t *api);

bool update_display(const bridge_api_t *api, window_display_t *display,
    SDL_DisplayID primary);
window_display_t *find_display(SDL_DisplayID id, uint64_t entity);

uint32_t read_modes(const bridge_api_t *api, SDL_DisplayID id,
    window_mode_t *modes, uint32_t *listed);
void spawn_modes(const bridge_api_t *api, window_display_t *display);
void destroy_modes(const bridge_api_t *api, window_display_t *display);

uint64_t find_mode_entity(const window_display_t *display,
    const SDL_DisplayMode *mode);
const SDL_DisplayMode *find_mode(const window_display_t *display,
    uint64_t entity);

void take_drops(const bridge_api_t *api);

void count_pastes(void);
void spawn_pastes(const bridge_api_t *api);

bool start_items(const bridge_api_t *api);
void turn_items(const bridge_api_t *api);
bridge_text_t put_bytes(const bridge_api_t *api, window_half_t *half,
    const char *bytes);
void spawn_item(const bridge_api_t *api, uint32_t slot, const void *value,
    uint32_t size);

char *copy_arena_text(const bridge_api_t *api, bridge_text_t text);

uint64_t spawn_entity(const bridge_api_t *api);
bool has_room(const bridge_api_t *api, uint32_t spawns, uint32_t bytes);

#endif
