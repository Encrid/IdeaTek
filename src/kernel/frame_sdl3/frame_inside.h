/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef FRAME_INSIDE_H
    #define FRAME_INSIDE_H

    #include <SDL3/SDL_timer.h>

    #include "bridge_native.h"
    #include "frame_values.h"
    #include "ideatek_frame.h"
    #include "ideatek_window.h"

    #define FRAME_SETTINGS_FIELDS { \
        uint32_t most_steps; \
        bridge_text_t time_source; \
        double fixed_round; \
        bridge_text_t sleeper; \
        bridge_text_t smoothing; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(frame_settings_t, FRAME_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(frame_settings_t, FRAME_SETTINGS_FIELDS);

    #define FRAME_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define FRAME_SINGLE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_SINGLETON, \
        BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t ENTITY = 0;
constexpr uint32_t WINDOW_MODE = 1;
constexpr uint32_t SURFACE = 2;
constexpr uint32_t WINDOW_STATE = 3;
constexpr uint32_t EVENT_WAIT = 4;
constexpr uint32_t FRAME = 5;
constexpr uint32_t TICK = 6;
constexpr uint32_t TIME_REQUEST = 7;
constexpr uint32_t SETTINGS = 8;

constexpr uint32_t FRAME_TEXT = 64;

typedef struct {
    bool started;
    bool broken;
    uint32_t most_steps;
    double fixed_round;
    bool fixed;
    bool events;
    bool refresh;
    frame_rule_t rule;
    uint32_t max_rate;
    ideatek_time_request_t seen;
    frame_clock_t clock;
    frame_values_t values;
    uint32_t run;
    uint64_t last_start;
    uint64_t window;
    uint32_t window_list;
} frame_module_t;

extern frame_module_t frame_module;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);
bool start_clock(const bridge_api_t *api);
void apply_request(const bridge_api_t *api);

#endif
