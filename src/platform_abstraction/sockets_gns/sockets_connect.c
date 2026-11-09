/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

static int options_of(const ideatek_connection_request_t *r,
    sockets_connection_t *c, gns_config_t *o)
{
    int64_t index = c - sockets_module.connections;

    o[0] = (gns_config_t){ .value = GNS_CONFIG_SEND_RATE_MIN,
        .type = GNS_CONFIG_TYPE_INT32, .u.i32 = (int32_t)r->send_rate_min };
    o[1] = (gns_config_t){ .value = GNS_CONFIG_SEND_RATE_MAX,
        .type = GNS_CONFIG_TYPE_INT32, .u.i32 = (int32_t)r->send_rate_max };
    o[2] = (gns_config_t){ .value = GNS_CONFIG_SYMMETRIC,
        .type = GNS_CONFIG_TYPE_INT32, .u.i32 = r->symmetric };
    o[3] = (gns_config_t){ .value = GNS_CONFIG_USER_DATA,
        .type = GNS_CONFIG_TYPE_INT64, .u.i64 = index };
    c->symmetric = r->symmetric;
    return (4);
}

static const char *open_one(const ideatek_connection_request_t *r,
    sockets_connection_t *c)
{
    gns_config_t options[4];
    gns_ip_t address;
    char text[sizeof(r->remote_identity.text) + 1] = "";
    int count = options_of(r, c, options);
    bool ip = r->action == IDEATEK_CONNECTION_ACTION_CONNECT;

    memcpy(&address, &r->address, sizeof(address));
    memcpy(text, r->remote_identity.text, sizeof(r->remote_identity.text));
    if (!ip && !SteamAPI_SteamNetworkingIdentity_ParseString(&c->peer,
            sizeof(c->peer), text))
        return ("refused");
    if (ip)
        c->handle = SteamAPI_ISteamNetworkingSockets_ConnectByIPAddress(
            sockets_module.sockets, &address, count, options);
    else
        c->handle = SteamAPI_ISteamNetworkingSockets_ConnectP2PCustomSignaling(
            sockets_module.sockets, make_signaling(), &c->peer,
            r->virtual_port, count, options);
    return (c->handle ? nullptr : "refused");
}

static void finish(const bridge_api_t *api, sockets_connection_t *c,
    const char *why)
{
    uint64_t entity = c->entity;

    if (why) {
        free_slot(c);
        attach_problem(api, entity, why);
        return;
    }
    SteamAPI_ISteamNetworkingSockets_SetConnectionUserData(
        sockets_module.sockets, c->handle, c - sockets_module.connections);
    attach_state(api, c);
}

static void open_connection(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r)
{
    sockets_connection_t *c;
    char field[32];

    if (find_entity(entity) != sockets_module.connection_max) {
        snprintf(field, sizeof(field), "action %u", r->action);
        say_request(api, entity, field, "a new entity");
        return;
    }
    c = take_slot(entity);
    if (!c) {
        say_bound(api, "connections", sockets_module.connection_max, entity);
        attach_problem(api, entity, "too many connections");
        return;
    }
    finish(api, c, r->action == IDEATEK_CONNECTION_ACTION_PAIR
        ? make_pair(api, c) : open_one(r, c));
}

BRIDGE_NATIVE(connection)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_CONNECTION]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_connection_request_t *r;

    for (uint32_t i = 0; i < list->count; i++) {
        r = (const void *)(api->shared_base
            + bridge_native_find_value(api, t[i].id, CONNECTION_REQUEST));
        if (!judge(api, t[i].id, r))
            continue;
        if (r->action == IDEATEK_CONNECTION_ACTION_ACCEPT)
            accept_connection(api, t[i].id);
        else if (r->action == IDEATEK_CONNECTION_ACTION_CLOSE)
            close_connection(api, t[i].id, r);
        else
            open_connection(api, t[i].id, r);
    }
    return (0);
}
