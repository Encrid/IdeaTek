/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_sockets.h"

#if defined(GAME_SOCKETS)

static const char *const STATUSES[] = { "connecting", "finding_route",
    "connected", "closed_by_peer", "problem" };
static const char *const LISTEN_STATUSES[] = { "listening", "failed" };
static const char *const AUTH[] = { "none", "current", "failed" };
static const char *const ADDRESS_STATUSES[] = { "ok", "not_found", "failed" };

static uint32_t lists[10];

void game_sock_text(char *out, size_t size, const bridge_api_t *api,
    bridge_text_t text)
{
    size_t len = text.len < size - 1 ? text.len : size - 1;

    memcpy(out, api->shared_base + text.at, len);
    out[len] = '\0';
    for (size_t i = 0; i < len; i++)
        if (out[i] == '\n' || out[i] == '\0')
            out[i] = '|';
}

void game_sock_address(char *out, size_t size, const ideatek_address_t *a)
{
    static const uint8_t V4[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };
    static const uint8_t ZERO[15] = {};

    if (!memcmp(a->bytes, V4, 12))
        snprintf(out, size, "v4 %u.%u.%u.%u:%u", a->bytes[12], a->bytes[13],
            a->bytes[14], a->bytes[15], a->port);
    else if (!memcmp(a->bytes, ZERO, 15))
        snprintf(out, size, "::%u:%u", a->bytes[15], a->port);
    else
        snprintf(out, size, "v6 %02x%02x..%02x%02x:%u", a->bytes[0],
            a->bytes[1], a->bytes[14], a->bytes[15], a->port);
}

static const void *cell(const bridge_api_t *api, uint64_t id, uint32_t slot)
{
    uint32_t at = bridge_native_find_value(api, id, slot);

    return (at ? (const void *)(api->shared_base + at) : nullptr);
}

static void say_state(const bridge_api_t *api, uint32_t k, uint32_t round)
{
    const ideatek_connection_state_t *s = cell(api, game_sockets.entities[k],
        game_sockets_slots.connection_state);
    char address[64];

    if (!s)
        return;
    game_sock_address(address, sizeof(address), &s->remote);
    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u conn %u %s flags %u peer [%s] at %s end %d [%s] pending %d %d "
        "unacked %d failed %u", round, k, STATUSES[s->status % 5], s->flags,
        s->remote_identity.text, address, s->end_reason, s->end_text,
        s->pending_reliable, s->pending_unreliable, s->sent_unacked_reliable,
        s->failed_sends);
}

static void show_conns(const bridge_api_t *api)
{
    for (uint32_t k = 0; k < game_sockets.count; k++)
        say_state(api, k, game.round);
}

static void show_listens(const bridge_api_t *api)
{
    const ideatek_listen_state_t *s;

    for (uint32_t k = 0; k < game_sockets.count; k++) {
        s = cell(api, game_sockets.entities[k],
            game_sockets_slots.listen_state);
        if (s)
            bridge_native_say(api, BRIDGE_LOG_INFO,
                "r%u listen %u kind %u port %u vport %d %s", game.round, k,
                s->kind, s->port, s->virtual_port,
                LISTEN_STATUSES[s->status % 2]);
    }
}

static void show_identity(const bridge_api_t *api)
{
    const ideatek_identity_state_t *s = cell(api, game_sockets.self,
        game_sockets_slots.identity_state);

    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO,
            "r%u identity [%s] %s request %u", game.round, s->identity.text,
            AUTH[s->status % 3], s->certificate_request.len);
}

static void show_inbound(const bridge_api_t *api)
{
    const ideatek_inbound_t *i;

    for (uint32_t k = 0; k < game_sockets.count; k++) {
        i = cell(api, game_sockets.entities[k], game_sockets_slots.inbound);
        if (i)
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u inbound %u listen %u",
                game.round, k, game_sock_index(i->listen.id));
    }
}

static void show_sessions(const bridge_api_t *api)
{
    const ideatek_session_t *s;

    for (uint32_t k = 0; k < game_sockets.count; k++) {
        s = cell(api, game_sockets.entities[k], game_sockets_slots.session);
        if (s)
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u session %u [%s]",
                game.round, k, s->remote.text);
    }
}

static void show_lanes(const bridge_api_t *api)
{
    const ideatek_batch_t *b;
    const uint8_t *at;
    int64_t queue;
    int32_t pending[3];

    for (uint32_t k = 0; k < game_sockets.count; k++) {
        b = cell(api, game_sockets.entities[k], game_sockets_slots.lane_states);
        if (!b)
            continue;
        at = api->shared_base + b->data.at;
        for (uint32_t i = 0; i < b->data.len / 20; i++) {
            memcpy(&queue, at + i * 20, 8);
            memcpy(pending, at + i * 20 + 8, 12);
            bridge_native_say(api, BRIDGE_LOG_INFO,
                "r%u lane %u/%u queue %lld pending %d %d unacked %d",
                game.round, k, i, (long long)queue, pending[0], pending[1],
                pending[2]);
        }
    }
}

static void show_addrs(const bridge_api_t *api)
{
    const ideatek_address_result_t *r;
    ideatek_address_t a;
    char text[64];

    for (uint32_t k = 0; k < game_sockets.count; k++) {
        r = cell(api, game_sockets.entities[k],
            game_sockets_slots.address_result);
        if (!r)
            continue;
        text[0] = '\0';
        if (r->addresses.len >= 18) {
            memcpy(&a, api->shared_base + r->addresses.at, 18);
            game_sock_address(text, sizeof(text), &a);
        }
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u addr %u %s count %u [%s]",
            game.round, k, ADDRESS_STATUSES[r->status % 3], r->addresses.len
            / 18, text);
    }
}

static void say_messages(const bridge_api_t *api, const ideatek_batch_t *b,
    bool session, uint32_t round)
{
    const uint8_t *at = api->shared_base + b->data.at;
    uint32_t left = b->data.len;
    uint32_t head = session ? 17 : 23;
    uint64_t entity;
    int64_t number = 0;
    int32_t channel = 0;
    uint32_t len;
    uint16_t lane = 0;
    char text[128];

    while (left >= head) {
        memcpy(&entity, at, 8);
        if (session) {
            memcpy(&channel, at + 8, 4);
            memcpy(&len, at + 12, 4);
        } else {
            memcpy(&number, at + 8, 8);
            memcpy(&len, at + 16, 4);
            memcpy(&lane, at + 20, 2);
        }
        game_sock_text(text, sizeof(text), api,
            (bridge_text_t){ .at = b->data.at + (uint32_t)(at + head
                - api->shared_base - b->data.at), .len = len });
        bridge_native_say(api, BRIDGE_LOG_INFO,
            "r%u %s %u %s %lld lane %u flags %u len %u [%s]", round,
            session ? "sessmsg" : "msg", game_sock_index(entity),
            session ? "channel" : "number", session ? (long long)channel
            : (long long)number, lane, at[head - 1], len, text);
        if (left < head + len)
            break;
        at += head + len;
        left -= head + len;
    }
}

bool game_sockets_show_step(const bridge_api_t *api, const char *verb)
{
    const ideatek_batch_t *b;
    bool done = true;

    if (!strcmp(verb, "show_conns"))
        show_conns(api);
    else if (!strcmp(verb, "show_listens"))
        show_listens(api);
    else if (!strcmp(verb, "show_identity"))
        show_identity(api);
    else if (!strcmp(verb, "show_inbound"))
        show_inbound(api);
    else if (!strcmp(verb, "show_sessions"))
        show_sessions(api);
    else if (!strcmp(verb, "show_lanes"))
        show_lanes(api);
    else if (!strcmp(verb, "show_addrs"))
        show_addrs(api);
    else if (!strcmp(verb, "show_msgs")) {
        b = cell(api, game_sockets.self, game_sockets_slots.messages_in);
        if (b)
            say_messages(api, b, false, game.round);
    } else if (!strcmp(verb, "show_count"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u socks %u", game.round,
            game_sockets.count);
    else
        done = false;
    return (done);
}

static void seen_each(const bridge_api_t *api, uint32_t list,
    void (*say)(const bridge_api_t *, uint64_t, uint32_t))
{
    const bridge_list_t *l = bridge_native_get_list(api, list);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        say(api, t[i].id, game.round + 1);
}

static void say_conn(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    say_state(api, game_sock_add(id), round);
}

static void say_inbound(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_inbound_t *i = cell(api, id, game_sockets_slots.inbound);

    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u inbound %u listen %u", round,
        game_sock_add(id), i ? game_sock_index(i->listen.id) : 999);
}

static void say_listen(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_listen_state_t *s = cell(api, id,
        game_sockets_slots.listen_state);

    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u listen %u %s", round,
            game_sock_add(id), LISTEN_STATUSES[s->status % 2]);
}

static void say_session(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_session_t *s = cell(api, id, game_sockets_slots.session);

    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u session %u [%s]", round,
            game_sock_add(id), s->remote.text);
}

static void say_identity(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_identity_state_t *s = cell(api, id,
        game_sockets_slots.identity_state);

    game_sockets.self = id;
    if (!s)
        return;
    strncpy(game_sockets.identity, s->identity.text,
        sizeof(game_sockets.identity) - 1);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u identity [%s] %s request %u",
        round, s->identity.text, AUTH[s->status % 3],
        s->certificate_request.len);
}

static void say_in(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_batch_t *b = cell(api, id, game_sockets_slots.messages_in);

    if (b)
        say_messages(api, b, false, round);
}

static void say_session_in(const bridge_api_t *api, uint64_t id,
    uint32_t round)
{
    const ideatek_batch_t *b = cell(api, id,
        game_sockets_slots.session_messages_in);

    if (b)
        say_messages(api, b, true, round);
}

static void say_signals(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_batch_t *b = cell(api, id, game_sockets_slots.signals_out);

    (void)round;
    if (b)
        game_sockets_write_signals(api, b);
}

static void say_addr(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_address_result_t *r = cell(api, id,
        game_sockets_slots.address_result);
    ideatek_address_t a;
    char text[64] = "";

    if (!r)
        return;
    if (r->addresses.len >= 18) {
        memcpy(&a, api->shared_base + r->addresses.at, 18);
        game_sock_address(text, sizeof(text), &a);
    }
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u addr %u %s count %u [%s]",
        round, game_sock_index(id), ADDRESS_STATUSES[r->status % 3],
        r->addresses.len / 18, text);
}

static void say_lanes(const bridge_api_t *api, uint64_t id, uint32_t round)
{
    const ideatek_batch_t *b = cell(api, id, game_sockets_slots.lane_states);

    if (b)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u lanes %u count %u", round,
            game_sock_index(id), b->data.len / 20);
}

BRIDGE_NATIVE(seen_conn)
{
    seen_each(api, lists[0], say_conn);
    return (0);
}

BRIDGE_NATIVE(seen_inbound)
{
    seen_each(api, lists[1], say_inbound);
    return (0);
}

BRIDGE_NATIVE(seen_listen)
{
    seen_each(api, lists[2], say_listen);
    return (0);
}

BRIDGE_NATIVE(seen_session)
{
    seen_each(api, lists[3], say_session);
    return (0);
}

BRIDGE_NATIVE(seen_identity)
{
    seen_each(api, lists[4], say_identity);
    return (0);
}

BRIDGE_NATIVE(seen_in)
{
    seen_each(api, lists[5], say_in);
    return (0);
}

BRIDGE_NATIVE(seen_session_in)
{
    seen_each(api, lists[6], say_session_in);
    return (0);
}

BRIDGE_NATIVE(seen_signals)
{
    seen_each(api, lists[7], say_signals);
    return (0);
}

BRIDGE_NATIVE(seen_addr)
{
    seen_each(api, lists[8], say_addr);
    return (0);
}

BRIDGE_NATIVE(seen_lanes)
{
    seen_each(api, lists[9], say_lanes);
    return (0);
}

BRIDGE_NATIVE(sock_early)
{
    game_sockets_signals_step(api);
    return (0);
}

#define SEEN(api, name, slot) BRIDGE_NATIVE_OBSERVER(api, name, \
    BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, (&(bridge_term_t){ slot, BRIDGE_READ, \
    BRIDGE_AND, 0 }), 1, nullptr, 0, nullptr, 0)

void game_sockets_watch_shows(const bridge_api_t *api)
{
    game_sockets_slots_t *s = &game_sockets_slots;
    uint32_t changes[1] = { s->signals_in };

    lists[0] = SEEN(api, "seen_conn", s->connection_state);
    lists[1] = SEEN(api, "seen_inbound", s->inbound);
    lists[2] = SEEN(api, "seen_listen", s->listen_state);
    lists[3] = SEEN(api, "seen_session", s->session);
    lists[4] = SEEN(api, "seen_identity", s->identity_state);
    lists[5] = SEEN(api, "seen_in", s->messages_in);
    lists[6] = SEEN(api, "seen_session_in", s->session_messages_in);
    lists[7] = SEEN(api, "seen_signals", s->signals_out);
    lists[8] = SEEN(api, "seen_addr", s->address_result);
    lists[9] = SEEN(api, "seen_lanes", s->lane_states);
    BRIDGE_NATIVE_ADD_SYSTEM(api, "sock_early", "first", nullptr, 0, changes,
        1, nullptr, 0);
}

#endif
