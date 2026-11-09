/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "audio_inside.h"

static uint64_t device_entity(const audio_stream_t *stream)
{
    bool recording = stream->applied.direction
        == IDEATEK_AUDIO_DIRECTION_RECORDING;
    bool chosen = stream->status == IDEATEK_AUDIO_STATUS_RUNNING
        && stream->applied.target == IDEATEK_AUDIO_TARGET_CHOSEN;
    SDL_AudioDeviceID id = chosen ? stream->device
        : audio_module.defaults[recording];
    uint32_t at = find_device(id);

    return ((at != AUDIO_DEVICES)
        * audio_module.devices[at % AUDIO_DEVICES].entity);
}

static ideatek_audio_state_t fill_state(const audio_stream_t *stream)
{
    ideatek_audio_state_t state = { .dropped = stream->dropped,
        .status = stream->status };
    SDL_AudioSpec spec = {};
    bool playback = stream->applied.direction
        == IDEATEK_AUDIO_DIRECTION_PLAYBACK;
    uint32_t frame_bytes = stream->applied.channels * 4;
    int queued;

    if (!stream->stream)
        return (state);
    state.device.id = device_entity(stream);
    if (SDL_GetAudioDeviceFormat(stream->device, &spec, nullptr)) {
        state.rate = (uint32_t)spec.freq;
        state.channels = (uint8_t)spec.channels;
    }
    queued = playback ? SDL_GetAudioStreamQueued(stream->stream) : 0;
    state.queued = (uint32_t)(queued * (queued > 0)) / frame_bytes;
    state.wanted = playback * frames_wanted(stream->applied.ahead,
        state.queued);
    return (state);
}

void attach_state(const bridge_api_t *api, audio_stream_t *stream)
{
    ideatek_audio_state_t state;

    if (!stream->entity)
        return;
    state = fill_state(stream);
    if (!memcmp(&state, &stream->last, sizeof(state)))
        return;
    stream->last = state;
    BRIDGE_NATIVE_ATTACH(api, stream->entity, STATE, state);
}
