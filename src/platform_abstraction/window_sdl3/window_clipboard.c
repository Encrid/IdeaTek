/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "window_inside.h"

static void set_clipboard(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = get_cell(api, id, CLIPBOARD);
    const ideatek_clipboard_request_t *request = (const void *)(api->shared_base
        + at);
    char *text = copy_arena_text(api, request->text);

    if (!text)
        return;
    if (!SDL_SetClipboardText(text))
        bridge_native_say(api, BRIDGE_LOG_WARN, "no clipboard: %s",
            SDL_GetError());
    free(text);
}

BRIDGE_NATIVE(clipboard)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        window_module.clipboard_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        set_clipboard(api, t[i].id);
    return (0);
}

static bool is_paste(const SDL_KeyboardEvent *key)
{
    SDL_Keymod modifier = window_module.driver->paste;

    return (key->key == SDLK_V && (key->mod & modifier)
        && !(key->mod & SDL_KMOD_ALT));
}

void count_pastes(void)
{
    SDL_Event keys[WINDOW_PEEKS];
    int count = SDL_PeepEvents(keys, (int)WINDOW_PEEKS, SDL_PEEKEVENT,
        SDL_EVENT_KEY_DOWN, SDL_EVENT_KEY_DOWN);

    for (int i = 0; i < count; i++)
        if (is_paste(&keys[i].key))
            window_module.paste_count++;
}

void spawn_pastes(const bridge_api_t *api)
{
    window_half_t *half = &window_module.pastes;
    ideatek_paste_t paste = {};
    char *text;
    uint32_t len;

    if (!window_module.paste_count || !has_room(api, 1, 32))
        return;
    text = SDL_GetClipboardText();
    len = text ? (uint32_t)SDL_strlen(text) : 0;
    if (len > half->size - half->used) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "the paste needs %u bytes, "
            "and the half has %u free bytes", len, half->size - half->used);
        window_module.paste_count = 0;
    }
    paste.text = put_bytes(api, half, window_module.paste_count ? text : "");
    for (; window_module.paste_count && has_room(api, 1, 32);
        window_module.paste_count--)
        spawn_item(api, PASTE, &paste, sizeof(paste));
    SDL_free(text);
}
