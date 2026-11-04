/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_audio.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef struct {
    uint32_t device;
    uint32_t request;
    uint32_t frames;
    uint32_t recorded;
    uint32_t state;
} slots_t;

static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_audio_direction_t", ideatek_audio_direction_t);
    ENUM(api, "ideatek:ideatek_audio_target_t", ideatek_audio_target_t);
    ENUM(api, "ideatek:ideatek_audio_status_t", ideatek_audio_status_t);
}

static slots_t declare_structs(const bridge_api_t *api)
{
    return ((slots_t){
        .device = STRUCT(api, "ideatek:AudioDevice", ideatek_audio_device_t),
        .request = STRUCT(api, "ideatek:AudioRequest",
            ideatek_audio_request_t),
        .frames = STRUCT(api, "ideatek:AudioFrames", ideatek_audio_frames_t),
        .recorded = STRUCT(api, "ideatek:AudioRecorded",
            ideatek_audio_frames_t),
        .state = STRUCT(api, "ideatek:AudioState", ideatek_audio_state_t)
    });
}

[[maybe_unused]] static void ask(const bridge_api_t *api, slots_t slots)
{
    ideatek_audio_request_t request = { .rate = 48000, .ahead = 2048,
        .direction = IDEATEK_AUDIO_DIRECTION_PLAYBACK,
        .target = IDEATEK_AUDIO_TARGET_DEFAULT, .channels = 2 };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.request, request);
}

[[maybe_unused]] static void report(const bridge_api_t *api, slots_t slots)
{
    ideatek_audio_state_t state = {
        .status = IDEATEK_AUDIO_STATUS_NO_DEVICE };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.state, state);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_structs(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    declare_fields(api);
    ask(api, declare_structs(api));
#elif defined(ROLE_AUDIO)
    declare_fields(api);
    report(api, declare_structs(api));
#else
    declare_fields(api);
    declare_structs(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
