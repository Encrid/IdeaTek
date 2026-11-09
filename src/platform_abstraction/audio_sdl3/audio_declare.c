/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    AUDIO_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_audio_direction_t",
        ideatek_audio_direction_t);
    AUDIO_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_audio_target_t",
        ideatek_audio_target_t);
    AUDIO_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_audio_status_t",
        ideatek_audio_status_t);
}

static void declare_values(const bridge_api_t *api)
{
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:AudioDevice",
        ideatek_audio_device_t);
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:AudioRequest",
        ideatek_audio_request_t);
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:AudioFrames",
        ideatek_audio_frames_t);
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:AudioRecorded",
        ideatek_audio_frames_t);
    AUDIO_TYPE(api, BRIDGE_STRUCT, "ideatek:AudioState",
        ideatek_audio_state_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek_audio_sdl3:Settings",
        audio_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    declare_values(api);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { DEVICE, RECORDED, STATE };
    bridge_term_t request[1] = {{ REQUEST, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t frames[1] = {{ FRAMES, BRIDGE_READ, BRIDGE_AND, 0 }};

    BRIDGE_NATIVE_ADD_SYSTEM(api, "round", "first", nullptr, 0, changes, 3,
        nullptr, 0);
    audio_module.request_list = BRIDGE_NATIVE_OBSERVER(api, "request",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH | BRIDGE_ON_LEAVE, request, 1,
        nullptr, 0, nullptr, 0);
    audio_module.frames_list = BRIDGE_NATIVE_OBSERVER(api, "frames",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, frames, 1, nullptr, 0, nullptr,
        0);
}
