/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

static void write_one(const bridge_api_t *api, gns_message_t *m,
    uint64_t entity, bool session)
{
    sockets_half_t *h = &sockets_module.half;
    uint8_t *at = api->shared_base + h->at + h->side * h->size + h->used;
    sockets_record_t r = { .connection = entity, .number = m->message_number,
        .channel = m->channel, .length = (uint32_t)m->size, .lane = m->lane,
        .flags = send_back(m->flags), .bytes = m->data };

    h->used += session ? write_session_message(at, room_left(), &r)
        : write_message(at, room_left(), &r);
}

static void place_one(const bridge_api_t *api, gns_message_t *m,
    sockets_connection_t *c)
{
    bool session = !c;
    uint32_t at = session ? find_peer(&m->identity_peer)
        : (uint32_t)(c - sockets_module.connections);

    if (at == sockets_module.connection_max)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "a message of %s came before its session",
            m->identity_peer.u.text);
    else
        write_one(api, m, sockets_module.connections[at].entity, session);
    SteamAPI_SteamNetworkingMessage_t_Release(m);
}

static bool take_messages(const bridge_api_t *api, sockets_connection_t *c,
    int channel)
{
    gns_message_t *messages[SOCKETS_BATCH];
    const sockets_settings_t *s = bridge_native_get_single(api, SETTINGS);
    uint32_t want = room_left() / (s->recv_max_message + RECORD_IN_HEAD);
    int count;

    want -= (want > SOCKETS_BATCH) * (want - SOCKETS_BATCH);
    count = c
        ? SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection(
            sockets_module.sockets, c->handle, messages, (int)want)
        : SteamAPI_ISteamNetworkingMessages_ReceiveMessagesOnChannel(
            sockets_module.messages, channel, messages, (int)want);
    for (int i = 0; i < count; i++)
        place_one(api, messages[i], c);
    return (count == SOCKETS_BATCH);
}

void receive_connections(const bridge_api_t *api)
{
    sockets_connection_t *c = sockets_module.connections;
    uint32_t start = sockets_module.half.used;
    bool more;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        more = (c[i].entity != 0) & !c[i].session & !c[i].forgotten
            & !c[i].closed;
        while (more)
            more = take_messages(api, &c[i], -1);
    }
    attach_batch(api, MESSAGES_IN, start);
}

void receive_sessions(const bridge_api_t *api)
{
    uint32_t start = sockets_module.half.used;
    bool more;

    for (uint32_t channel = 0; channel < sockets_module.channels; channel++) {
        more = true;
        while (more)
            more = take_messages(api, nullptr, (int)channel);
    }
    attach_batch(api, SESSION_MESSAGES_IN, start);
}
