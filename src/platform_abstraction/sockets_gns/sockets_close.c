/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

static void mark_closed(sockets_connection_t *c)
{
    c->closed = true;
    drop_later(c->entity);
}

void accept_connection(const bridge_api_t *api, uint64_t entity)
{
    uint32_t at = find_entity(entity);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];
    bool ok = (at != sockets_module.connection_max) & !c->session
        & !c->forgotten;

    if (ok)
        ok = SteamAPI_ISteamNetworkingSockets_AcceptConnection(
            sockets_module.sockets, c->handle) == GNS_RESULT_OK;
    if (!ok)
        say_request(api, entity, "action 2", "an inbound connection");
}

void close_connection(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r)
{
    uint32_t at = find_entity(entity);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];
    bool wrong = (at == sockets_module.connection_max) | c->session
        | c->closed;

    if (wrong) {
        say_request(api, entity, "action 3", "a connection");
        return;
    }
    if (!c->forgotten)
        SteamAPI_ISteamNetworkingSockets_CloseConnection(
            sockets_module.sockets, c->handle, r->reason,
            "closed by the game", r->linger);
    mark_closed(c);
}

static void fail_listens(const bridge_api_t *api)
{
    sockets_listen_t *l = sockets_module.listens;
    ideatek_listen_state_t state = {};

    for (uint32_t i = 0; i < sockets_module.listen_max; i++) {
        if (!l[i].entity)
            continue;
        SteamAPI_ISteamNetworkingSockets_CloseListenSocket(
            sockets_module.sockets, l[i].handle);
        state.kind = (ideatek_listen_kind_t)l[i].kind;
        state.status = IDEATEK_LISTEN_STATUS_FAILED;
        BRIDGE_NATIVE_ATTACH(api, l[i].entity, LISTEN_STATE, state);
        memset(&l[i], 0, sizeof(l[i]));
    }
}

void stop_all(const bridge_api_t *api)
{
    sockets_connection_t *c = sockets_module.connections;
    bool live;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        live = (c[i].entity != 0) & !c[i].closed & !c[i].forgotten;
        if (!live)
            continue;
        c[i].forgotten = true;
        c[i].last.status = IDEATEK_CONNECTION_STATUS_PROBLEM;
        strncpy(c[i].last.end_text, "identity changed",
            sizeof(c[i].last.end_text) - 1);
        BRIDGE_NATIVE_ATTACH(api, c[i].entity, CONNECTION_STATE, c[i].last);
    }
    fail_listens(api);
}
