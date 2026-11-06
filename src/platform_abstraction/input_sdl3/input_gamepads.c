/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "input_inside.h"

static void name_pad(const bridge_api_t *api, const input_pad_t *p,
    ideatek_gamepad_t *named)
{
    const char *name = SDL_GetGamepadName(p->gamepad);

    if (name && SDL_utf8strlcpy(named->name, name, sizeof(named->name))
        < SDL_strlen(name))
        bridge_native_say(api, BRIDGE_LOG_WARN, "the name of the gamepad %u "
            "is longer than 378 bytes", p->id);
}

void add_pad(const bridge_api_t *api, const SDL_Event *e)
{
    SDL_JoystickID id = e->gdevice.which;
    input_pad_t *p = &input_module.pads[input_module.pad_count];
    ideatek_gamepad_t named = {};

    if (input_module.pad_count >= INPUT_GAMEPADS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "more than 16 gamepads: the "
            "gamepad %u gets no entity", id);
        return;
    }
    *p = (input_pad_t){ .id = id, .gamepad = SDL_OpenGamepad(id) };
    if (!p->gamepad) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no gamepad %u: %s", id,
            SDL_GetError());
        return;
    }
    name_pad(api, p, &named);
    p->entity = spawn_entity(api);
    BRIDGE_NATIVE_ATTACH(api, p->entity, GAMEPAD, named);
    BRIDGE_NATIVE_ATTACH(api, p->entity, GAMEPAD_STATE, p->last);
    input_module.pad_count++;
}

void remove_pad(const bridge_api_t *api, const SDL_Event *e)
{
    input_pad_t *p = find_pad(e->gdevice.which, 0);

    if (!p)
        return;
    SDL_CloseGamepad(p->gamepad);
    api->destroy(api, p->entity);
    *p = input_module.pads[--input_module.pad_count];
}

static void play_rumble(const bridge_api_t *api, uint64_t entity)
{
    input_pad_t *p = find_pad(0, entity);
    const ideatek_gamepad_request_t *r;
    float strong;
    float weak;

    if (!p)
        return;
    r = (const void *)(api->shared_base + bridge_native_find_value(api,
        entity, GAMEPAD_REQUEST));
    strong = fmaxf(0.0F, fminf(r->strong, 1.0F));
    weak = fmaxf(0.0F, fminf(r->weak, 1.0F));
    if (!SDL_RumbleGamepad(p->gamepad, (Uint16)(strong * 65535.0F),
        (Uint16)(weak * 65535.0F), r->duration_ms))
        bridge_native_say(api, BRIDGE_LOG_WARN, "no rumble on the gamepad "
            "%llu: %s", (unsigned long long)entity, SDL_GetError());
}

BRIDGE_NATIVE(rumble)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        input_module.pad_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        play_rumble(api, t[i].id);
    return (0);
}
