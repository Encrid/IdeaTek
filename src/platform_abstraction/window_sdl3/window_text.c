/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "window_inside.h"

char *copy_arena_text(const bridge_api_t *api, bridge_text_t text)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    char *copy;

    if (text.at < h->arena_lo || text.at > h->arena_hi
        || text.len > h->arena_hi - text.at) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the text at %u with %u bytes is not in the persistent arena",
            text.at, text.len);
        return (nullptr);
    }
    copy = malloc((size_t)text.len + 1);
    if (!copy) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "no memory for a text of %u bytes", text.len + 1);
        return (nullptr);
    }
    memcpy(copy, api->shared_base + text.at, text.len);
    copy[text.len] = '\0';
    return (copy);
}
