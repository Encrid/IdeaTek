/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_states.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef struct {
    uint32_t state;
    uint32_t request;
    uint32_t change;
    uint32_t scoped;
} slots_t;

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
}

[[maybe_unused]] static uint32_t declare_scoped(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:StateScoped", ideatek_state_scoped_t));
}

[[maybe_unused]] static slots_t declare_all(const bridge_api_t *api)
{
    slots_t slots;

    declare_fields(api);
    slots.state = STRUCT(api, "ideatek:State", ideatek_state_t);
    slots.request = STRUCT(api, "ideatek:StateRequest",
        ideatek_state_request_t);
    slots.change = STRUCT(api, "ideatek:StateChange", ideatek_state_change_t);
    slots.scoped = declare_scoped(api);
    return (slots);
}

[[maybe_unused]] static void play(const bridge_api_t *api)
{
    ideatek_state_t state = { .value = 1 };
    ideatek_state_request_t request = { .value = 2 };
    slots_t slots = declare_all(api);
    uint64_t id = api->spawn(api);
    ideatek_state_scoped_t scoped = { .machine = { .id = id }, .value = 1 };

    BRIDGE_NATIVE_ATTACH(api, id, slots.state, state);
    BRIDGE_NATIVE_ATTACH(api, id, slots.request, request);
    BRIDGE_NATIVE_ATTACH(api, id, slots.scoped, scoped);
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    ideatek_state_request_t request = { .value = 2 };
    slots_t slots = declare_all(api);
    uint64_t id = api->spawn(api);
    ideatek_state_scoped_t scoped = { .machine = { .id = id }, .value = 1 };

    BRIDGE_NATIVE_ATTACH(api, id, slots.request, request);
    BRIDGE_NATIVE_ATTACH(api, id, slots.scoped, scoped);
}

[[maybe_unused]] static void change(const bridge_api_t *api)
{
    ideatek_state_t state = { .value = 2 };
    ideatek_state_change_t done = { .from = 1, .to = 2 };
    slots_t slots = declare_all(api);
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, slots.state, state);
    BRIDGE_NATIVE_ATTACH(api, id, slots.change, done);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_scoped(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    play(api);
#elif defined(ROLE_OTHER)
    ask(api);
#elif defined(ROLE_STATES)
    change(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
