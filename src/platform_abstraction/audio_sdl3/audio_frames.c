/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_inside.h"

static void put_frames(const bridge_api_t *api, uint64_t id)
{
    audio_stream_t *stream = &audio_module.streams[0];
    uint32_t at = bridge_native_find_value(api, id, FRAMES);
    const ideatek_audio_frames_t *f = (const void *)(api->shared_base + at);
    uint32_t bytes = f->frames * stream->applied.channels * 4;
    bool fits = f->data.len >= bytes;

    if (id != stream->entity || !stream->stream)
        return;
    if (f->at != stream->count || !fits) {
        stream->dropped += f->frames;
        return;
    }
    SDL_PutAudioStreamData(stream->stream, api->shared_base + f->data.at,
        (int)bytes);
    stream->count += f->frames;
}

BRIDGE_NATIVE(frames)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        audio_module.frames_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        put_frames(api, t[i].id);
    return (0);
}

static uint32_t drop_oldest(const bridge_api_t *api, audio_stream_t *stream,
    uint32_t frames, uint32_t frame_bytes)
{
    uint32_t drop = frames_over(frames, stream->applied.ahead);
    uint32_t left = drop;
    uint32_t chunk;
    bool over;
    uint8_t *half = api->shared_base + audio_module.half
        + audio_module.side * AUDIO_HALF_BYTES;

    while (left) {
        over = left > AUDIO_HALF_FRAMES;
        chunk = over * AUDIO_HALF_FRAMES + !over * left;
        SDL_GetAudioStreamData(stream->stream, half,
            (int)(chunk * frame_bytes));
        left -= chunk;
    }
    stream->dropped += drop;
    return (drop);
}

static void attach_recorded(const bridge_api_t *api, audio_stream_t *stream,
    uint32_t frames)
{
    uint32_t at = audio_module.half + audio_module.side * AUDIO_HALF_BYTES;
    ideatek_audio_frames_t batch = {};

    batch.at = stream->count;
    batch.frames = frames;
    batch.data.at = at;
    batch.data.len = frames * stream->applied.channels * 4;
    stream->count += frames;
    if (!frames)
        return;
    SDL_GetAudioStreamData(stream->stream, api->shared_base + at,
        (int)batch.data.len);
    BRIDGE_NATIVE_ATTACH(api, stream->entity, RECORDED, batch);
}

void take_recorded(const bridge_api_t *api)
{
    audio_stream_t *stream = &audio_module.streams[1];
    uint32_t frame_bytes = stream->applied.channels * 4;
    uint32_t frames;
    uint32_t dropped;

    if (!stream->stream)
        return;
    frames = (uint32_t)SDL_GetAudioStreamAvailable(stream->stream)
        / frame_bytes;
    dropped = drop_oldest(api, stream, frames, frame_bytes);
    stream->count += dropped;
    attach_recorded(api, stream, frames - dropped);
}
