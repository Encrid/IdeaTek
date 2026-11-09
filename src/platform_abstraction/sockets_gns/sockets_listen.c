/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

static uint32_t find_listen(uint64_t entity)
{
    sockets_listen_t *l = sockets_module.listens;
    uint32_t at = sockets_module.listen_max;
    bool hit;

    for (uint32_t i = 0; i < sockets_module.listen_max; i++) {
        hit = l[i].entity == entity;
        at = hit * i + !hit * at;
    }
    return (at);
}

static void close_listen(uint32_t i)
{
    sockets_listen_t *l = &sockets_module.listens[i];

    SteamAPI_ISteamNetworkingSockets_CloseListenSocket(
        sockets_module.sockets, l->handle);
    memset(l, 0, sizeof(*l));
}

static uint32_t open_listen(const bridge_api_t *api, uint64_t entity,
    const ideatek_listen_request_t *r)
{
    gns_ip_t address = { .port = r->port };
    char value[16];

    if (r->kind == IDEATEK_LISTEN_KIND_IP)
        return (SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP(
                sockets_module.sockets, &address, 0, nullptr));
    if (r->kind == IDEATEK_LISTEN_KIND_P2P)
        return (SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P(
                sockets_module.sockets, r->virtual_port, 0, nullptr));
    snprintf(value, sizeof(value), "kind %u", r->kind);
    say_request(api, entity, value, "0 to 1");
    return (0);
}

static void open_request(const bridge_api_t *api, uint64_t entity,
    const ideatek_listen_request_t *r)
{
    uint32_t at = find_listen(0);
    sockets_listen_t *l = &sockets_module.listens[at
        % sockets_module.listen_max];
    ideatek_listen_state_t state = { .virtual_port = r->virtual_port,
        .port = r->port, .kind = r->kind,
        .status = IDEATEK_LISTEN_STATUS_FAILED };

    if (at == sockets_module.listen_max)
        say_bound(api, "listen sockets", sockets_module.listen_max, entity);
    else {
        l->handle = open_listen(api, entity, r);
        l->entity = l->handle ? entity : 0;
        l->kind = r->kind;
        state.status = l->handle ? IDEATEK_LISTEN_STATUS_LISTENING
            : IDEATEK_LISTEN_STATUS_FAILED;
    }
    BRIDGE_NATIVE_ATTACH(api, entity, LISTEN_STATE, state);
}

BRIDGE_NATIVE(listen)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_LISTEN]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_listen_request_t *r;
    uint32_t at;

    for (uint32_t i = 0; i < list->count; i++) {
        at = find_listen(t[i].id);
        if (at != sockets_module.listen_max)
            close_listen(at);
        if (t[i].events & BRIDGE_ON_LEAVE)
            continue;
        r = (const void *)(api->shared_base
            + bridge_native_find_value(api, t[i].id, LISTEN_REQUEST));
        open_request(api, t[i].id, r);
    }
    return (0);
}
