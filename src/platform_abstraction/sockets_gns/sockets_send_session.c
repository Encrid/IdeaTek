/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

void send_session_one(const bridge_api_t *api, const sockets_record_t *r)
{
    uint32_t at = find_entity(r->connection);
    sockets_connection_t *c = &sockets_module.connections[at
        % sockets_module.connection_max];
    bool known = (at != sockets_module.connection_max) & c->session;
    bool channel_ok = (uint32_t)r->channel < sockets_module.channels;
    int result;

    if (!known | c->closed) {
        say_refused(api, r->connection, "no session", nullptr);
        return;
    }
    if (!channel_ok) {
        say_refused(api, r->connection, "outside the limits", c);
        return;
    }
    result = SteamAPI_ISteamNetworkingMessages_SendMessageToUser(
        sockets_module.messages, &c->peer, r->bytes, r->length,
        send_of(r->flags), r->channel);
    if (result != GNS_RESULT_OK)
        say_refused(api, r->connection, "GameNetworkingSockets refused it", c);
}

BRIDGE_NATIVE(session_messages_out)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_SESSION_MESSAGES_OUT]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        send_batch(api, t[i].id, true);
    return (0);
}
