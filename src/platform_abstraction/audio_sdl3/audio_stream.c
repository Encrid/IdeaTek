/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_inside.h"

static const char *const DIRECTIONS[2] = { "playback", "recording" };
static const SDL_AudioDeviceID DEFAULTS[2] = {
    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, SDL_AUDIO_DEVICE_DEFAULT_RECORDING };

static SDL_AudioDeviceID pick_device(audio_stream_t *stream)
{
    const ideatek_audio_request_t *r = &stream->applied;
    bool recording = r->direction == IDEATEK_AUDIO_DIRECTION_RECORDING;
    bool chosen = r->target == IDEATEK_AUDIO_TARGET_CHOSEN;
    const audio_device_t *d = audio_module.devices;
    SDL_AudioDeviceID id = DEFAULTS[recording];
    bool found = false;
    bool hit;

    for (uint32_t i = 0; i < audio_module.device_count; i++) {
        hit = chosen & (d[i].entity == r->device.id)
            & (d[i].recording == recording);
        found |= hit;
        id = hit * d[i].id + !hit * id;
    }
    stream->status = (chosen & !found) ? IDEATEK_AUDIO_STATUS_FELL_BACK
        : IDEATEK_AUDIO_STATUS_RUNNING;
    return (id);
}

static bool open_stream(const bridge_api_t *api, audio_stream_t *stream)
{
    const ideatek_audio_request_t *r = &stream->applied;
    bool recording = r->direction == IDEATEK_AUDIO_DIRECTION_RECORDING;
    SDL_AudioSpec spec = { SDL_AUDIO_F32, r->channels, (int)r->rate };
    unsigned long long id = stream->entity;

    stream->device = pick_device(stream);
    if (stream->status == IDEATEK_AUDIO_STATUS_FELL_BACK)
        bridge_native_say(api, BRIDGE_LOG_WARN, "the device of %llu is not "
            "there, so the stream uses the default", id);
    stream->stream = SDL_OpenAudioDeviceStream(stream->device, &spec, nullptr,
        nullptr);
    if (!stream->stream) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no %s stream: %s",
            DIRECTIONS[recording], SDL_GetError());
        stream->status = recording ? IDEATEK_AUDIO_STATUS_DENIED
            : IDEATEK_AUDIO_STATUS_NO_DEVICE;
        return (false);
    }
    SDL_ResumeAudioStreamDevice(stream->stream);
    return (true);
}

void close_stream(audio_stream_t *stream)
{
    if (stream->stream)
        SDL_DestroyAudioStream(stream->stream);
    stream->stream = nullptr;
    stream->lost = false;
}

static bool needs_open(const audio_stream_t *stream,
    const ideatek_audio_request_t *r)
{
    const ideatek_audio_request_t *a = &stream->applied;

    return (!stream->stream || stream->lost || r->target != a->target
        || r->device.id != a->device.id);
}

void apply_stream(const bridge_api_t *api, audio_stream_t *stream)
{
    ideatek_audio_request_t r = stream->applied;
    bool again;
    bool format;

    if (!stream->pending && !stream->lost)
        return;
    if (stream->pending && !read_request(api, stream, &r))
        return;
    again = needs_open(stream, &r);
    format = r.rate != stream->applied.rate
        || r.channels != stream->applied.channels;
    stream->applied = r;
    stream->pending = false;
    if (again)
        close_stream(stream);
    if (again && !open_stream(api, stream))
        return;
    if (!again && format)
        SDL_SetAudioStreamFormat(stream->stream, &(SDL_AudioSpec){
            SDL_AUDIO_F32, r.channels, (int)r.rate }, nullptr);
}
