/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "audio_inside.h"

uint32_t find_device(SDL_AudioDeviceID id)
{
    uint32_t at = AUDIO_DEVICES;
    bool hit;

    for (uint32_t i = 0; i < audio_module.device_count; i++) {
        hit = audio_module.devices[i].id == id;
        at = hit * i + !hit * at;
    }
    return (at);
}

static void attach_device(const bridge_api_t *api, audio_device_t *d,
    const char *name)
{
    ideatek_audio_device_t value = {};

    d->is_default = d->id == audio_module.defaults[d->recording];
    value.direction = d->recording ? IDEATEK_AUDIO_DIRECTION_RECORDING
        : IDEATEK_AUDIO_DIRECTION_PLAYBACK;
    value.is_default = d->is_default;
    if (name && SDL_utf8strlcpy(value.name, name, sizeof(value.name))
        < strlen(name))
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the name of the device %u is longer than 127 bytes", d->id);
    BRIDGE_NATIVE_ATTACH(api, d->entity, DEVICE, value);
}

void add_device(const bridge_api_t *api, SDL_AudioDeviceID id, bool recording)
{
    audio_device_t *d = &audio_module.devices[audio_module.device_count
        % AUDIO_DEVICES];

    if (find_device(id) != AUDIO_DEVICES)
        return;
    if (audio_module.device_count == AUDIO_DEVICES) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "more than 32 devices: the device %u gets no entity", id);
        return;
    }
    d->id = id;
    d->recording = recording;
    d->entity = api->spawn(api);
    attach_device(api, d, SDL_GetAudioDeviceName(id));
    audio_module.device_count++;
}

void remove_device(const bridge_api_t *api, SDL_AudioDeviceID id)
{
    uint32_t at = find_device(id);
    audio_device_t *d = &audio_module.devices[at % AUDIO_DEVICES];
    bool lost;

    if (at == AUDIO_DEVICES)
        return;
    api->destroy(api, d->entity);
    for (uint32_t i = 0; i < 2; i++) {
        lost = audio_module.streams[i].stream
            && audio_module.streams[i].device == id;
        audio_module.streams[i].lost |= lost;
    }
    audio_module.device_count--;
    *d = audio_module.devices[audio_module.device_count];
}

void update_defaults(const bridge_api_t *api)
{
    SDL_AudioDeviceID id;
    uint32_t old;
    uint32_t new;

    for (uint32_t dir = 0; dir < 2; dir++) {
        id = SDL_GetDefaultAudioDeviceID(dir == 1);
        if (id == audio_module.defaults[dir])
            continue;
        old = find_device(audio_module.defaults[dir]);
        new = find_device(id);
        audio_module.defaults[dir] = id;
        if (old != AUDIO_DEVICES)
            attach_device(api, &audio_module.devices[old],
                SDL_GetAudioDeviceName(audio_module.devices[old].id));
        if (new != AUDIO_DEVICES)
            attach_device(api, &audio_module.devices[new],
                SDL_GetAudioDeviceName(id));
    }
}
