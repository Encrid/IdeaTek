/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "audio_inside.h"

static const char FRAMES_RANGE[] = "0, or 64 to 65536";
static const char NAME_RANGE[] = "1 to 255 bytes";

static bool copy_name(const bridge_api_t *api, bridge_text_t text)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    bool inside = (text.at >= h->arena_lo) & (text.at <= h->arena_hi)
        & (text.len <= h->arena_hi - text.at);
    bool ok = inside & (text.len >= 1) & (text.len < AUDIO_TEXT);
    uint32_t len = ok * text.len;

    memcpy(audio_module.name, api->shared_base + text.at, len);
    audio_module.name[len] = '\0';
    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "%s: a text of %u bytes is outside %s", "stream_name", text.len,
            NAME_RANGE);
    return (ok);
}

static void list_direction(const bridge_api_t *api, bool recording)
{
    int count = 0;
    SDL_AudioDeviceID *ids = recording ? SDL_GetAudioRecordingDevices(&count)
        : SDL_GetAudioPlaybackDevices(&count);

    if (!ids) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no device list: %s",
            SDL_GetError());
        return;
    }
    for (int i = 0; i < count; i++)
        add_device(api, ids[i], recording);
    SDL_free(ids);
}

static void set_hints(uint32_t device_frames)
{
    char frames[16];

    SDL_snprintf(frames, sizeof(frames), "%u", device_frames);
    if (device_frames)
        SDL_SetHintWithPriority(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, frames,
            SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_AUDIO_DEVICE_STREAM_NAME,
        audio_module.name, SDL_HINT_OVERRIDE);
}

bool start_audio(const bridge_api_t *api)
{
    const audio_settings_t *s = bridge_native_get_single(api, SETTINGS);
    bool ok = check_frames(s->device_frames);

    audio_module.started = true;
    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %s",
            "device_frames", s->device_frames, FRAMES_RANGE);
    ok &= copy_name(api, s->stream_name);
    audio_module.broken = !ok;
    if (!ok)
        return (false);
    set_hints(s->device_frames);
    audio_module.half = api->alloc(api, 2 * AUDIO_HALF_BYTES);
    audio_module.defaults[0] = SDL_GetDefaultAudioDeviceID(false);
    audio_module.defaults[1] = SDL_GetDefaultAudioDeviceID(true);
    list_direction(api, false);
    list_direction(api, true);
    return (true);
}
