/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

static bool read_lanes(const bridge_api_t *api, const ideatek_batch_t *b,
    int *priorities, uint16_t *weights)
{
    const uint8_t *at = api->shared_base + b->data.at;
    uint32_t count = b->data.len / 6;
    bool ok = true;

    for (uint32_t i = 0; i < count; i++) {
        memcpy(&priorities[i], at + i * 6, 4);
        memcpy(&weights[i], at + i * 6 + 4, 2);
        ok &= weights[i] >= 1;
    }
    return (ok);
}

static bool judge_lanes(const bridge_api_t *api, uint64_t entity,
    const ideatek_batch_t *b, const sockets_connection_t *c)
{
    uint32_t count = b->data.len / 6 + (b->data.len % 6 != 0);
    bool shape = (b->data.len % 6 == 0) & (count >= 1) & (count <= 255);
    bool enough = count >= c->lanes;
    char field[32];
    char range[64];

    snprintf(field, sizeof(field), "lanes %u", count);
    snprintf(range, sizeof(range), "%u to 255 records of 6 bytes", c->lanes);
    if (!shape || !enough)
        say_request(api, entity, field, range);
    return (shape & enough);
}

static void apply_lanes(const bridge_api_t *api, uint64_t entity,
    const ideatek_batch_t *b, sockets_connection_t *c)
{
    int priorities[255];
    uint16_t weights[255];
    uint32_t count = b->data.len / 6;
    char field[32];
    int result;

    snprintf(field, sizeof(field), "lanes %u", count);
    if (!read_lanes(api, b, priorities, weights)) {
        say_request(api, entity, field, "weights of 1 or more");
        return;
    }
    result = SteamAPI_ISteamNetworkingSockets_ConfigureConnectionLanes(
        sockets_module.sockets, c->handle, (int)count, priorities, weights);
    if (result != GNS_RESULT_OK)
        say_request(api, entity, field, "the lanes that GameNetworkingSockets"
            " takes: refused");
    else
        c->lanes = count;
}

BRIDGE_NATIVE(lanes)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_LANES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_batch_t *b;
    sockets_connection_t *c;
    uint32_t at;

    for (uint32_t i = 0; i < list->count; i++) {
        b = batch_of(api, t[i].id, LANES_REQUEST);
        at = find_entity(t[i].id);
        c = &sockets_module.connections[at % sockets_module.connection_max];
        if ((at == sockets_module.connection_max) | c->session)
            say_request(api, t[i].id, "lanes", "a connection");
        else if (judge_lanes(api, t[i].id, b, c))
            apply_lanes(api, t[i].id, b, c);
    }
    return (0);
}
