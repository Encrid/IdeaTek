/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "files_inside.h"

bool has_room(const bridge_api_t *api, uint32_t bytes)
{
    return (bridge_native_get_header(api)->changes_left >= bytes);
}

void take_halves(const bridge_api_t *api, uint32_t bytes)
{
    files_module.half.at = api->alloc(api, bytes);
    files_module.half.size = bytes / 2;
}

void turn_results(const bridge_api_t *api)
{
    while (files_module.gone_count)
        api->destroy(api, files_module.gone[--files_module.gone_count]);
    files_module.half.side = 1 - files_module.half.side;
    files_module.half.used = 0;
}

bridge_text_t put_bytes(const bridge_api_t *api, const char *bytes,
    uint32_t len)
{
    files_half_t *half = &files_module.half;
    bridge_text_t text = { .len = len };

    text.at = half->at + half->side * half->size + half->used;
    memcpy(api->shared_base + text.at, bytes, len);
    half->used += len;
    return (text);
}
