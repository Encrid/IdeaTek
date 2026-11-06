/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static const uint16_t MODIFIERS[][2] = {
    { SDL_KMOD_LCTRL, IDEATEK_MODIFIER_LCTRL },
    { SDL_KMOD_LSHIFT, IDEATEK_MODIFIER_LSHIFT },
    { SDL_KMOD_LALT, IDEATEK_MODIFIER_LALT },
    { SDL_KMOD_LGUI, IDEATEK_MODIFIER_LGUI },
    { SDL_KMOD_RCTRL, IDEATEK_MODIFIER_RCTRL },
    { SDL_KMOD_RSHIFT, IDEATEK_MODIFIER_RSHIFT },
    { SDL_KMOD_RALT, IDEATEK_MODIFIER_RALT },
    { SDL_KMOD_RGUI, IDEATEK_MODIFIER_RGUI },
    { SDL_KMOD_NUM, IDEATEK_MODIFIER_NUM },
    { SDL_KMOD_CAPS, IDEATEK_MODIFIER_CAPS },
    { SDL_KMOD_SCROLL, IDEATEK_MODIFIER_SCROLL },
};

ideatek_modifier_t modifiers_of(SDL_Keymod mod)
{
    uint16_t bits = 0;

    for (size_t i = 0; i < SDL_arraysize(MODIFIERS); i++)
        if (mod & MODIFIERS[i][0])
            bits |= MODIFIERS[i][1];
    return ((ideatek_modifier_t)bits);
}

static void set_bit(uint32_t code, bool down)
{
    uint64_t *keys = input_module.next_keyboard.keys;
    uint32_t id = code & 0xFFu;

    if ((code >> 16) != 0x07u)
        return;
    if (down)
        keys[id / 64] |= 1ull << (id % 64);
    else
        keys[id / 64] &= ~(1ull << (id % 64));
}

void take_key(const bridge_api_t *api, const SDL_Event *e)
{
    const SDL_KeyboardEvent *k = &e->key;
    ideatek_key_event_t event = {};

    event.code = usage_of_scancode(k->scancode);
    if (!event.code)
        return;
    event.key = usage_of_keycode(k->key);
    event.character = character_of(k->scancode, k->mod);
    event.modifiers = modifiers_of(k->mod);
    event.down = k->down;
    event.repeat = k->repeat;
    event.order = next_order();
    spawn_event(api, KEY_EVENT, &event, sizeof(event));
    set_bit(event.code, k->down);
    input_module.next_keyboard.modifiers = event.modifiers;
}

void release_keys(const bridge_api_t *api)
{
    uint64_t *keys = input_module.next_keyboard.keys;
    ideatek_key_event_t event = {};

    for (uint32_t id = 0; id < 256; id++) {
        if (!(keys[id / 64] >> (id % 64) & 1u))
            continue;
        event.code = 0x00070000u | id;
        event.key = event.code;
        event.modifiers = input_module.next_keyboard.modifiers;
        event.order = next_order();
        spawn_event(api, KEY_EVENT, &event, sizeof(event));
        set_bit(event.code, false);
    }
}
