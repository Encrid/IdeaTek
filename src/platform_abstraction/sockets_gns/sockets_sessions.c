/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

sockets_connection_t *take_session(const bridge_api_t *api, uint64_t entity,
    const gns_identity_t *peer)
{
    sockets_connection_t *c = take_slot(entity);
    ideatek_session_t session = {};

    if (!c) {
        say_bound(api, "connections", sockets_module.connection_max, entity);
        return (nullptr);
    }
    c->session = true;
    c->peer = *peer;
    SteamAPI_SteamNetworkingIdentity_ToString(peer, session.remote.text,
        sizeof(session.remote.text));
    BRIDGE_NATIVE_ATTACH(api, entity, SESSION, session);
    attach_state(api, c);
    return (c);
}

static void open_session(const bridge_api_t *api, uint64_t entity,
    const ideatek_session_request_t *r)
{
    gns_identity_t peer;
    char text[sizeof(r->remote.text) + 1] = "";
    char field[sizeof(text) + 8];

    memcpy(text, r->remote.text, sizeof(r->remote.text));
    if (find_entity(entity) != sockets_module.connection_max) {
        say_request(api, entity, "action 0", "a session");
        return;
    }
    if (!SteamAPI_SteamNetworkingIdentity_ParseString(&peer, sizeof(peer),
            text) || peer.type == GNS_IDENTITY_IP) {
        snprintf(field, sizeof(field), "remote %s", text);
        say_request(api, entity, field, "an identity");
        return;
    }
    if (!take_session(api, entity, &peer))
        attach_problem(api, entity, "too many connections");
}

static void close_session(uint64_t entity, sockets_connection_t *c,
    const ideatek_session_request_t *r)
{
    int state;

    if (r->action == IDEATEK_SESSION_ACTION_CLOSE_CHANNEL)
        SteamAPI_ISteamNetworkingMessages_CloseChannelWithUser(
            sockets_module.messages, &c->peer, r->channel);
    else
        SteamAPI_ISteamNetworkingMessages_CloseSessionWithUser(
            sockets_module.messages, &c->peer);
    state = SteamAPI_ISteamNetworkingMessages_GetSessionConnectionInfo(
        sockets_module.messages, &c->peer, nullptr, nullptr);
    if (state == GNS_STATE_NONE) {
        c->closed = true;
        drop_later(entity);
    }
}

static void session_action(const bridge_api_t *api, uint64_t entity,
    const ideatek_session_request_t *r)
{
    uint32_t at = find_entity(entity);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];
    char field[32];
    bool ok = (at != sockets_module.connection_max) & c->session
        & !c->closed & (r->action <= IDEATEK_SESSION_ACTION_CLOSE_CHANNEL);

    snprintf(field, sizeof(field), "action %u", r->action);
    if (!ok) {
        say_request(api, entity, field, "a session");
        return;
    }
    if (r->action >= IDEATEK_SESSION_ACTION_CLOSE)
        close_session(entity, c, r);
    else if (!SteamAPI_ISteamNetworkingMessages_AcceptSessionWithUser(
            sockets_module.messages, &c->peer))
        say_request(api, entity, field, "a session of a peer");
}

BRIDGE_NATIVE(session)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_SESSION]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_session_request_t *r;

    for (uint32_t i = 0; i < list->count; i++) {
        r = (const void *)(api->shared_base
            + bridge_native_find_value(api, t[i].id, SESSION_REQUEST));
        if (r->action == IDEATEK_SESSION_ACTION_OPEN)
            open_session(api, t[i].id, r);
        else
            session_action(api, t[i].id, r);
    }
    return (0);
}
