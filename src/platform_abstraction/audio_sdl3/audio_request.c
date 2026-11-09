/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "audio_inside.h"

static const char *const DIRECTIONS[2] = { "playback", "recording" };

bool read_request(const bridge_api_t *api, audio_stream_t *stream,
    ideatek_audio_request_t *r)
{
    uint32_t at = bridge_native_find_value(api, stream->entity, REQUEST);
    audio_fault_t fault = {};

    memcpy(r, api->shared_base + at, sizeof(*r));
    if (check_request(r, &fault))
        return (true);
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the request of %llu: %s %llu is outside %s",
        (unsigned long long)stream->entity, fault.field,
        (unsigned long long)fault.value, fault.range);
    return (false);
}

static void leave_request(uint64_t id)
{
    audio_stream_t *stream;

    for (uint32_t dir = 0; dir < 2; dir++) {
        stream = &audio_module.streams[dir];
        if (stream->entity != id)
            continue;
        close_stream(stream);
        stream->entity = 0;
        stream->pending = false;
        stream->count = 0;
        stream->dropped = 0;
        stream->last = (ideatek_audio_state_t){};
    }
}

static void take_request(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = bridge_native_find_value(api, id, REQUEST);
    const ideatek_audio_request_t *r = (const void *)(api->shared_base + at);
    uint32_t dir = r->direction == IDEATEK_AUDIO_DIRECTION_RECORDING;
    audio_stream_t *stream = &audio_module.streams[dir];

    if (audio_module.streams[1 - dir].entity == id)
        leave_request(id);
    if (stream->entity && stream->entity != id) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the entity %llu asks for a second %s stream, and gets none",
            (unsigned long long)id, DIRECTIONS[dir]);
        return;
    }
    stream->entity = id;
    stream->pending = true;
}

BRIDGE_NATIVE(request)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        audio_module.request_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);

    for (uint32_t i = 0; i < list->count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE)
            leave_request(t[i].id);
        else
            take_request(api, t[i].id);
    }
    return (0);
}
