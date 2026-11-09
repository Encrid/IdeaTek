/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

void turn_half(const bridge_api_t *api)
{
    while (sockets_module.gone_count)
        api->destroy(api, sockets_module.gone[--sockets_module.gone_count]);
    memcpy(sockets_module.gone, sockets_module.later,
        sockets_module.later_count * sizeof(uint64_t));
    sockets_module.gone_count = sockets_module.later_count;
    sockets_module.later_count = 0;
    sockets_module.half.side = 1 - sockets_module.half.side;
    sockets_module.half.used = 0;
}

void drop_later(uint64_t entity)
{
    sockets_module.later[sockets_module.later_count++] = entity;
}

bridge_text_t put_bytes(const bridge_api_t *api, const void *bytes,
    uint32_t len)
{
    sockets_half_t *half = &sockets_module.half;
    bridge_text_t text = { .len = len };

    text.at = half->at + half->side * half->size + half->used;
    memcpy(api->shared_base + text.at, bytes, len);
    half->used += len;
    return (text);
}

uint32_t room_left(void)
{
    return (sockets_module.half.size - sockets_module.half.used);
}
