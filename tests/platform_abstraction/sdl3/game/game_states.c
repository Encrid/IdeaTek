/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_states.h"

#if defined(GAME_STATES)

#define TYPED(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_states_slots_t game_states_slots;

static uint64_t machines[64];
static uint32_t machine_count;
static uint64_t scoped[64];
static uint32_t scoped_count;
static uint64_t bare[16];
static uint32_t bare_count;
static uint32_t lists[2];

static uint32_t index_of(uint64_t id)
{
    for (uint32_t k = 0; k < machine_count; k++)
        if (machines[k] == id)
            return (k);
    return (999);
}

static void tie(const bridge_api_t *api, uint64_t id, uint64_t machine,
    uint32_t value)
{
    ideatek_state_scoped_t s = { .machine = { .id = machine }, .value = value };

    BRIDGE_NATIVE_ATTACH(api, id, game_states_slots.scoped, s);
}

static void spawn_machine(const bridge_api_t *api, const char *args)
{
    ideatek_state_t s = { .value = (uint32_t)atoi(args) };
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, game_states_slots.state, s);
    machines[machine_count++] = id;
}

static void spawn_sub(const bridge_api_t *api, const char *args)
{
    unsigned k = 0;
    unsigned v = 0;
    ideatek_state_t s = {};
    uint64_t id = api->spawn(api);

    sscanf(args, "%u,%u,%u", &k, &v, &s.value);
    tie(api, id, machines[k], v);
    BRIDGE_NATIVE_ATTACH(api, id, game_states_slots.state, s);
    machines[machine_count++] = id;
}

static void spawn_scoped(const bridge_api_t *api, const char *args, bool bare)
{
    unsigned k = 0;
    unsigned v = 0;
    uint64_t id = api->spawn(api);
    uint64_t below;

    sscanf(args, "%u,%u", &k, &v);
    tie(api, id, machines[k], v);
    scoped[scoped_count++] = id;
    if (bare) {
        below = api->spawn(api);
        tie(api, below, id, 1);
        scoped[scoped_count++] = below;
    }
}

static void ask_state(const bridge_api_t *api, const char *args, bool loose)
{
    unsigned k = 0;
    ideatek_state_request_t r = {};
    uint64_t id;

    sscanf(args, "%u,%u", &k, &r.value);
    if (loose) {
        id = api->spawn(api);
        r.value = k;
        bare[bare_count++] = id;
    } else
        id = machines[k];
    BRIDGE_NATIVE_ATTACH(api, id, game_states_slots.request, r);
}

static void show_states(const bridge_api_t *api)
{
    const ideatek_state_t *s;
    const ideatek_state_change_t *c;

    for (uint32_t k = 0; k < machine_count; k++) {
        s = game_cell(api, machines[k], game_states_slots.state);
        c = game_cell(api, machines[k], game_states_slots.change);
        if (s)
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u state %u value %u",
                game.round, k, s->value);
        else
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u state %u gone",
                game.round, k);
        if (c)
            bridge_native_say(api, BRIDGE_LOG_INFO,
                "r%u change %u from %u to %u", game.round, k, c->from, c->to);
    }
}

static void show_scoped(const bridge_api_t *api)
{
    const void *cell;

    for (uint32_t j = 0; j < scoped_count; j++) {
        cell = game_cell(api, scoped[j], game_states_slots.scoped);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u scoped %u %s", game.round,
            j, cell ? "alive" : "gone");
    }
}

static void show_bare(const bridge_api_t *api)
{
    const void *cell;

    for (uint32_t j = 0; j < bare_count; j++) {
        cell = game_cell(api, bare[j], game_states_slots.request);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u bare %u request %s",
            game.round, j, cell ? "kept" : "gone");
    }
}

bool game_states_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "machine"))
        spawn_machine(api, args);
    else if (!strcmp(verb, "submachine"))
        spawn_sub(api, args);
    else if (!strcmp(verb, "scoped"))
        spawn_scoped(api, args, false);
    else if (!strcmp(verb, "scopedbare"))
        spawn_scoped(api, args, true);
    else if (!strcmp(verb, "statereq"))
        ask_state(api, args, false);
    else if (!strcmp(verb, "reqbare"))
        ask_state(api, args, true);
    else if (!strcmp(verb, "statekill"))
        api->destroy(api, machines[atoi(args)]);
    else if (!strcmp(verb, "show_states"))
        show_states(api);
    else if (!strcmp(verb, "show_scoped"))
        show_scoped(api);
    else if (!strcmp(verb, "show_bare"))
        show_bare(api);
    else
        done = false;
    return (done);
}

void game_declare_states(const bridge_api_t *api)
{
    game_states_slots.state = TYPED(api, "ideatek:State", ideatek_state_t);
    game_states_slots.request = TYPED(api, "ideatek:StateRequest",
        ideatek_state_request_t);
    game_states_slots.change = TYPED(api, "ideatek:StateChange",
        ideatek_state_change_t);
    game_states_slots.scoped = TYPED(api, "ideatek:StateScoped",
        ideatek_state_scoped_t);
}

BRIDGE_NATIVE(seen_change)
{
    const bridge_list_t *list = bridge_native_get_list(api, lists[0]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_state_change_t *c;

    for (uint32_t i = 0; i < list->count; i++) {
        c = game_cell(api, t[i].id, game_states_slots.change);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u changed %u from %u to %u",
            game.round + 1, index_of(t[i].id), c->from, c->to);
    }
    return (0);
}

BRIDGE_NATIVE(seen_machine)
{
    const bridge_list_t *list = bridge_native_get_list(api, lists[1]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_state_t *s;

    for (uint32_t i = 0; i < list->count; i++) {
        s = game_cell(api, t[i].id, game_states_slots.state);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u entered %u value %u",
            game.round + 1, index_of(t[i].id), s->value);
    }
    return (0);
}

void game_watch_states(const bridge_api_t *api)
{
    bridge_term_t change[1] = {{ game_states_slots.change, BRIDGE_READ,
        BRIDGE_AND, 0 }};
    bridge_term_t state[1] = {{ game_states_slots.state, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    lists[0] = BRIDGE_NATIVE_OBSERVER(api, "seen_change", BRIDGE_ON_ATTACH
        | BRIDGE_ON_ENTER, change, 1, nullptr, 0, nullptr, 0);
    lists[1] = BRIDGE_NATIVE_OBSERVER(api, "seen_machine", BRIDGE_ON_ENTER,
        state, 1, nullptr, 0, nullptr, 0);
}

#endif
