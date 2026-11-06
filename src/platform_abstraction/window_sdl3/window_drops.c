/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static uint32_t length(const char *text)
{
    return (text ? (uint32_t)strlen(text) : 0);
}

static void spawn_drop(const bridge_api_t *api, const SDL_DropEvent *e,
    ideatek_drop_kind_t kind)
{
    ideatek_drop_t drop = {};
    uint32_t need = length(e->data) + length(e->source);
    uint32_t room = window_module.drops.size - window_module.drops.used;

    drop.window.id = e->windowID ? window_module.entity : 0;
    drop.set = window_module.set;
    drop.index = window_module.index;
    drop.x = e->x;
    drop.y = e->y;
    drop.kind = kind;
    if (need <= room) {
        drop.data = put_bytes(api, &window_module.drops, e->data);
        drop.source = put_bytes(api, &window_module.drops, e->source);
        spawn_item(api, DROP, &drop, sizeof(drop));
    } else
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the drop %u.%u needs %u bytes, and the half has %u free bytes",
            drop.set, drop.index, need, room);
    window_module.index++;
}

static void take_drop(const bridge_api_t *api, const SDL_DropEvent *e)
{
    if (e->type == SDL_EVENT_DROP_BEGIN) {
        window_module.set++;
        window_module.index = 0;
        window_module.drop_over = true;
        window_module.drop_x = 0.0F;
        window_module.drop_y = 0.0F;
    }
    if (e->type == SDL_EVENT_DROP_POSITION) {
        window_module.drop_x = e->x;
        window_module.drop_y = e->y;
    }
    if (e->type == SDL_EVENT_DROP_FILE)
        spawn_drop(api, e, IDEATEK_DROP_FILE);
    if (e->type == SDL_EVENT_DROP_TEXT)
        spawn_drop(api, e, IDEATEK_DROP_TEXT);
    if (e->type == SDL_EVENT_DROP_COMPLETE)
        window_module.drop_over = false;
    window_module.dirty = true;
}

void take_drops(const bridge_api_t *api)
{
    SDL_Event e;

    while (has_room(api, 1, 72) && SDL_PeepEvents(&e, 1, SDL_GETEVENT,
        SDL_EVENT_DROP_FILE, SDL_EVENT_DROP_POSITION) == 1)
        take_drop(api, &e.drop);
}
