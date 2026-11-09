/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

void say_refused(const bridge_api_t *api, uint64_t entity,
    const char *reason, sockets_connection_t *c)
{
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the message of %llu is refused: %s", (unsigned long long)entity,
        reason);
    if (c)
        c->failed_sends++;
}

static int send_on_lane(sockets_connection_t *c, const sockets_record_t *r)
{
    gns_message_t *m = SteamAPI_ISteamNetworkingUtils_AllocateMessage(
        sockets_module.utils, (int)r->length);
    int64_t result = 0;

    if (!m)
        return (-1);
    memcpy(m->data, r->bytes, r->length);
    m->connection = c->handle;
    m->flags = send_of(r->flags);
    m->lane = r->lane;
    SteamAPI_ISteamNetworkingSockets_SendMessages(sockets_module.sockets, 1,
        &m, &result, true);
    return (result < 0 ? (int)-result : GNS_RESULT_OK);
}

static void send_one(const bridge_api_t *api, const sockets_record_t *r)
{
    uint32_t at = find_entity(r->connection);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];
    bool known = (at != sockets_module.connection_max) & !c->session;
    int result;

    if (!known | c->forgotten | c->closed) {
        say_refused(api, r->connection, "no connection", nullptr);
        return;
    }
    if (r->lane >= c->lanes) {
        say_refused(api, r->connection, "outside the limits", c);
        return;
    }
    result = send_on_lane(c, r);
    SteamAPI_ISteamNetworkingSockets_FlushMessagesOnConnection(
        sockets_module.sockets, c->handle);
    if (result != GNS_RESULT_OK)
        say_refused(api, r->connection, "GameNetworkingSockets refused it", c);
}

void send_batch(const bridge_api_t *api, uint64_t entity, bool session)
{
    const ideatek_batch_t *b = batch_of(api, entity,
        session ? SESSION_MESSAGES_OUT : MESSAGES_OUT);
    const uint8_t *at = api->shared_base + b->data.at;
    uint32_t left = b->data.len;
    uint32_t used = 1;
    sockets_record_t r;

    while (left && used) {
        used = session ? read_session_message(at, left, &r)
            : read_message(at, left, &r);
        if (used && session)
            send_session_one(api, &r);
        if (used && !session)
            send_one(api, &r);
        at += used;
        left -= used;
    }
    if (left)
        say_malformed(api, entity, b->data.len - left);
    SteamNetworkingSockets_Poll(0);
}

BRIDGE_NATIVE(messages_out)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_MESSAGES_OUT]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        send_batch(api, t[i].id, false);
    return (0);
}
