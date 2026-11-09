/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

static void place_second(const bridge_api_t *api,
    sockets_connection_t *second, uint32_t handle)
{
    ideatek_inbound_t inbound = {};

    second->handle = handle;
    second->entity = api->spawn(api);
    SteamAPI_ISteamNetworkingSockets_SetConnectionUserData(
        sockets_module.sockets, handle, second - sockets_module.connections);
    BRIDGE_NATIVE_ATTACH(api, second->entity, INBOUND, inbound);
    attach_state(api, second);
}

const char *make_pair(const bridge_api_t *api, sockets_connection_t *c)
{
    sockets_connection_t *second = take_slot(1);
    uint32_t handles[2] = {};
    bool ok;

    if (!second) {
        say_bound(api, "connections", sockets_module.connection_max,
            c->entity);
        return ("too many connections");
    }
    ok = SteamAPI_ISteamNetworkingSockets_CreateSocketPair(
        sockets_module.sockets, &handles[0], &handles[1], false,
        &sockets_module.identity, &sockets_module.identity);
    if (!ok) {
        free_slot(second);
        return ("refused");
    }
    c->handle = handles[0];
    place_second(api, second, handles[1]);
    return (nullptr);
}
