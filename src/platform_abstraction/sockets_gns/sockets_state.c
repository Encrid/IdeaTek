/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

static bool live_state(const bridge_api_t *api,
    const sockets_connection_t *c, gns_info_t *info, gns_status_t *status)
{
    bool known = SteamAPI_ISteamNetworkingSockets_GetConnectionInfo(
        sockets_module.sockets, c->handle, info);
    bool ok = SteamAPI_ISteamNetworkingSockets_GetConnectionRealTimeStatus(
        sockets_module.sockets, c->handle, status, 0, nullptr)
        == GNS_RESULT_OK;

    ok &= known;
    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no state of the connection of %llu",
            (unsigned long long)c->entity);
    return (ok);
}

void fill_state(const bridge_api_t *api, sockets_connection_t *c,
    ideatek_connection_state_t *state)
{
    gns_info_t info = {};
    gns_status_t status = {};
    bool ok = false;

    *state = c->last;
    if (c->session)
        ok = SteamAPI_ISteamNetworkingMessages_GetSessionConnectionInfo(
            sockets_module.messages, &c->peer, &info, &status)
            != GNS_STATE_NONE;
    else if (!c->forgotten)
        ok = live_state(api, c, &info, &status);
    if (ok) {
        fill_info(state, &info);
        fill_status(state, &status);
    }
    state->failed_sends = c->failed_sends;
}

void attach_state(const bridge_api_t *api, sockets_connection_t *c)
{
    ideatek_connection_state_t state;

    fill_state(api, c, &state);
    if (!memcmp(&state, &c->last, sizeof(state)))
        return;
    c->last = state;
    BRIDGE_NATIVE_ATTACH(api, c->entity, CONNECTION_STATE, state);
}

void attach_states(const bridge_api_t *api)
{
    sockets_connection_t *c = sockets_module.connections;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        if ((c[i].entity == 0) | c[i].closed)
            continue;
        attach_state(api, &c[i]);
        c[i].failed_sends = 0;
    }
}
