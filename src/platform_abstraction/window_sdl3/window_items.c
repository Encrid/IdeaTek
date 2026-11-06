/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static const char RANGE[] = "2 to 1073741824, an even number";

static bool check_bytes(const bridge_api_t *api, const char *name,
    uint32_t value)
{
    if (value >= 2 && value <= 1073741824 && value % 2 == 0)
        return (true);
    bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %s", name, value,
        RANGE);
    return (false);
}

bool start_items(const bridge_api_t *api)
{
    const window_settings_t *settings = bridge_native_get_single(api,
        SETTINGS);
    bool drops = check_bytes(api, "drop_bytes", settings->drop_bytes);
    bool pastes = check_bytes(api, "clipboard_bytes",
        settings->clipboard_bytes);

    if (!drops || !pastes)
        return (false);
    window_module.drops.at = api->alloc(api, settings->drop_bytes);
    window_module.drops.size = settings->drop_bytes / 2;
    window_module.pastes.at = api->alloc(api, settings->clipboard_bytes);
    window_module.pastes.size = settings->clipboard_bytes / 2;
    return (true);
}

void turn_items(const bridge_api_t *api)
{
    for (uint32_t i = 0; i < window_module.item_count; i++)
        api->destroy(api, window_module.items[i]);
    window_module.item_count = 0;
    window_module.spawned = 0;
    window_module.drops.side = 1 - window_module.drops.side;
    window_module.drops.used = 0;
    window_module.pastes.side = 1 - window_module.pastes.side;
    window_module.pastes.used = 0;
}

bridge_text_t put_bytes(const bridge_api_t *api, window_half_t *half,
    const char *bytes)
{
    bridge_text_t text = {};

    text.at = half->at + half->side * half->size + half->used;
    text.len = bytes ? (uint32_t)strlen(bytes) : 0;
    memcpy(api->shared_base + text.at, bytes ? bytes : "", text.len);
    half->used += text.len;
    return (text);
}

void spawn_item(const bridge_api_t *api, uint32_t slot, const void *value,
    uint32_t size)
{
    uint64_t id = spawn_entity(api);

    api->attach(api, id, slot, bridge_native_lend(api, value, size));
    window_module.items[window_module.item_count++] = id;
}
