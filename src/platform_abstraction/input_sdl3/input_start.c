/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "input_inside.h"

static void add_mappings(const bridge_api_t *api, bridge_text_t text)
{
    char path[4096] = "";
    uint32_t len = text.len < sizeof(path) - 1 ? text.len
        : (uint32_t)sizeof(path) - 1;

    memcpy(path, api->shared_base + text.at, len);
    if (!strcmp(path, "none"))
        return;
    if (text.len >= sizeof(path) || SDL_AddGamepadMappingsFromFile(path) < 0)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no mappings %s: %s", path,
            SDL_GetError());
}

bool start_input(const bridge_api_t *api)
{
    const input_settings_t *s = bridge_native_get_single(api, SETTINGS);

    if (s->round_entities < 1 || s->round_entities > INPUT_EVENTS) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "round_entities: %u is outside 1 to 65535", s->round_entities);
        return (false);
    }
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, s->touch_mouse_events ? "1"
        : "0");
    add_mappings(api, s->mappings);
    input_module.started = true;
    return (true);
}
