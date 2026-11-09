/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef AUDIO_INSIDE_H
    #define AUDIO_INSIDE_H

    #include <SDL3/SDL.h>

    #include "audio_values.h"
    #include "bridge_native.h"
    #include "ideatek_audio.h"
    #include "sdl3_start.h"

    #define AUDIO_SETTINGS_FIELDS { \
        uint32_t device_frames; \
        bridge_text_t stream_name; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(audio_settings_t, AUDIO_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(audio_settings_t, AUDIO_SETTINGS_FIELDS);

    #define AUDIO_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t DEVICE = 4;
constexpr uint32_t REQUEST = 5;
constexpr uint32_t FRAMES = 6;
constexpr uint32_t RECORDED = 7;
constexpr uint32_t STATE = 8;
constexpr uint32_t SETTINGS = 9;

constexpr uint32_t AUDIO_DEVICES = 32;
constexpr uint32_t AUDIO_NAME = 128;
constexpr uint32_t AUDIO_REASON = 256;
constexpr uint32_t AUDIO_HALF_FRAMES = 65536;
constexpr uint32_t AUDIO_HALF_BYTES = 2097152;
constexpr uint32_t AUDIO_TEXT = 256;

constexpr uint32_t CHANGE_HEAD = 24;

    #define CHANGE_SIZE(n) (CHANGE_HEAD + (((n) + 7u) & ~7u))
    #define CHANGE_BYTES(T) CHANGE_SIZE(sizeof(T))

typedef struct {
    SDL_AudioDeviceID id;
    uint64_t entity;
    bool recording;
    bool is_default;
} audio_device_t;

typedef struct {
    SDL_AudioStream *stream;
    uint64_t entity;
    ideatek_audio_request_t applied;
    ideatek_audio_state_t last;
    uint64_t count;
    uint64_t dropped;
    SDL_AudioDeviceID device;
    ideatek_audio_status_t status;
    bool pending;
    bool lost;
} audio_stream_t;

typedef struct {
    bool started;
    bool broken;
    audio_device_t devices[AUDIO_DEVICES];
    uint32_t device_count;
    SDL_AudioDeviceID defaults[2];
    audio_stream_t streams[2];
    uint32_t half;
    uint32_t side;
    char name[AUDIO_TEXT];
    uint32_t request_list;
    uint32_t frames_list;
} audio_module_t;

extern audio_module_t audio_module;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);

bool start_audio(const bridge_api_t *api);

uint32_t find_device(SDL_AudioDeviceID id);
void add_device(const bridge_api_t *api, SDL_AudioDeviceID id,
    bool recording);
void remove_device(const bridge_api_t *api, SDL_AudioDeviceID id);
void update_defaults(const bridge_api_t *api);

bool read_request(const bridge_api_t *api, audio_stream_t *stream,
    ideatek_audio_request_t *r);

void close_stream(audio_stream_t *stream);
void apply_stream(const bridge_api_t *api, audio_stream_t *stream);

void take_recorded(const bridge_api_t *api);

void attach_state(const bridge_api_t *api, audio_stream_t *stream);

#endif
