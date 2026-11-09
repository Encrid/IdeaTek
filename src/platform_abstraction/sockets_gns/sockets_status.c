/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

static uint64_t listen_entity(uint32_t handle)
{
    sockets_listen_t *l = sockets_module.listens;
    uint64_t entity = 0;
    bool hit;

    for (uint32_t i = 0; i < sockets_module.listen_max; i++) {
        hit = (l[i].entity != 0) & (l[i].handle == handle);
        entity = hit * l[i].entity + !hit * entity;
    }
    return (entity);
}

static void refuse_inbound(const bridge_api_t *api, const gns_changed_t *g)
{
    char peer[128];

    SteamAPI_SteamNetworkingIdentity_ToString(&g->info.identity_remote, peer,
        sizeof(peer));
    SteamAPI_ISteamNetworkingSockets_CloseConnection(sockets_module.sockets,
        g->connection, GNS_END_MISC_GENERIC, "too many connections", false);
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "more than %u connections: the connection of %s is closed",
        sockets_module.connection_max, peer);
}

static void take_inbound(const bridge_api_t *api, const gns_changed_t *g)
{
    uint64_t entity = api->spawn(api);
    sockets_connection_t *c = take_slot(entity);
    ideatek_inbound_t inbound = {};

    if (!c) {
        api->destroy(api, entity);
        refuse_inbound(api, g);
        return;
    }
    c->handle = g->connection;
    SteamAPI_ISteamNetworkingSockets_SetConnectionUserData(
        sockets_module.sockets, c->handle, c - sockets_module.connections);
    inbound.listen.id = listen_entity(g->info.listen_socket);
    BRIDGE_NATIVE_ATTACH(api, entity, INBOUND, inbound);
    attach_state(api, c);
}

static void say_problem(const bridge_api_t *api, sockets_connection_t *c)
{
    char text[SOCKETS_TEXT] = "";

    SteamAPI_ISteamNetworkingSockets_GetDetailedConnectionStatus(
        sockets_module.sockets, c->handle, text, (int)sizeof(text));
    for (uint32_t i = 0; text[i]; i++)
        text[i] = text[i] == '\n' ? ' ' : text[i];
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the connection of %llu has a problem: %s",
        (unsigned long long)c->entity, text);
}

void on_changed(gns_changed_t *g)
{
    const bridge_api_t *api = sockets_module.api;
    int64_t user = g->info.user_data;
    bool in_range = (user >= 0) & (user < sockets_module.connection_max);
    sockets_connection_t *c = &sockets_module.connections[in_range * user];
    bool known = in_range & (c->handle == g->connection);

    if (!known && g->info.state == GNS_STATE_CONNECTING
        && g->info.listen_socket)
        take_inbound(api, g);
    if (!known)
        return;
    if (g->info.state == GNS_STATE_PROBLEM)
        say_problem(api, c);
    attach_state(api, c);
}
