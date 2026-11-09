/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

void on_session_request(gns_session_request_t *request)
{
    const bridge_api_t *api = sockets_module.api;
    uint64_t entity;

    if (find_peer(&request->identity_remote) != sockets_module.connection_max) {
        SteamAPI_ISteamNetworkingMessages_AcceptSessionWithUser(
            sockets_module.messages, &request->identity_remote);
        return;
    }
    entity = api->spawn(api);
    if (!take_session(api, entity, &request->identity_remote)) {
        api->destroy(api, entity);
        SteamAPI_ISteamNetworkingMessages_CloseSessionWithUser(
            sockets_module.messages, &request->identity_remote);
    }
}

void on_session_failed(gns_session_failed_t *failed)
{
    const bridge_api_t *api = sockets_module.api;
    uint32_t at = find_peer(&failed->info.identity_remote);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];

    if ((at == sockets_module.connection_max) | c->closed)
        return;
    c->last.status = status_of(failed->info.state);
    c->last.end_reason = failed->info.end_reason;
    SteamAPI_SteamNetworkingIdentity_ToString(&failed->info.identity_remote,
        c->last.remote_identity.text, sizeof(c->last.remote_identity.text));
    BRIDGE_NATIVE_ATTACH(api, c->entity, CONNECTION_STATE, c->last);
}
