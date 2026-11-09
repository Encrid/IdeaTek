/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_sockets.h"

#if defined(GAME_SOCKETS)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_sockets_slots_t game_sockets_slots;
game_sockets_t game_sockets;

static const char *const KINDS[] = { "ip", "p2p" };
static const char *const ACTIONS[] = { "connect", "p2p", "accept", "close",
    "pair" };
static const char *const SESSION_ACTIONS[] = { "open", "accept", "close",
    "closechannel" };
static const char *const SEND_WORDS[] = { "unreliable", "reliable", "nonagle",
    "nodelay", "restart" };
static const uint8_t SEND_VALUES[] = { 0, IDEATEK_SEND_RELIABLE,
    IDEATEK_SEND_NO_NAGLE, IDEATEK_SEND_NO_DELAY, IDEATEK_SEND_AUTO_RESTART };

static uint8_t index_of(const char *word, const char *const *words,
    uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
        if (!strcmp(word, words[i]))
            return (i);
    return ((uint8_t)atoi(word));
}

static const char *field(char **left)
{
    char *start = *left;
    char *comma = strchr(start, ',');

    if (comma) {
        *comma = '\0';
        *left = comma + 1;
    } else
        *left = start + strlen(start);
    return (start);
}

uint32_t game_sock_index(uint64_t entity)
{
    for (uint32_t k = 0; k < game_sockets.count; k++)
        if (game_sockets.entities[k] == entity)
            return (k);
    return (GAME_SOCKS);
}

uint32_t game_sock_add(uint64_t entity)
{
    uint32_t k = game_sock_index(entity);

    if (k != GAME_SOCKS)
        return (k);
    game_sockets.entities[game_sockets.count] = entity;
    return (game_sockets.count++);
}

bridge_text_t game_sock_bytes(const bridge_api_t *api, const void *bytes,
    uint32_t len)
{
    uint32_t at = api->alloc(api, len ? len : 1);

    memcpy(api->shared_base + at, bytes, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

static uint8_t send_flags(const char *text)
{
    uint8_t flags = 0;
    char copy[64];
    char *left = nullptr;

    strncpy(copy, text, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';
    for (char *w = strtok_r(copy, "+", &left); w; w = strtok_r(nullptr, "+",
            &left))
        flags |= SEND_VALUES[index_of(w, SEND_WORDS, 5) % 5];
    return (flags);
}

static void parse_address(ideatek_address_t *a, const char *text,
    const char *port)
{
    memset(a, 0, sizeof(*a));
    a->port = (uint16_t)atoi(port);
    if (!strcmp(text, "local"))
        a->bytes[15] = 1;
    else if (!strcmp(text, "v4local")) {
        a->bytes[10] = 0xff;
        a->bytes[11] = 0xff;
        a->bytes[12] = 127;
        a->bytes[15] = 1;
    } else if (!strcmp(text, "dead")) {
        a->bytes[0] = 0x20;
        a->bytes[1] = 0x01;
        a->bytes[2] = 0x0d;
        a->bytes[3] = 0xb8;
        a->bytes[15] = 1;
    }
}

static void set_identity(ideatek_identity_t *id, const char *text)
{
    memset(id, 0, sizeof(*id));
    strncpy(id->text, text, sizeof(id->text) - 1);
}

static void ask_identity(const bridge_api_t *api, char *args, bool other)
{
    ideatek_identity_request_t r = {};
    char *left = args;
    const char *text = field(&left);
    const char *certificate = field(&left);
    uint64_t id = other ? api->spawn(api) : game_sockets.self;

    set_identity(&r.identity, text);
    if (strcmp(certificate, "-") && *certificate)
        r.certificate = game_sock_bytes(api, certificate,
            (uint32_t)strlen(certificate));
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.identity_request, r);
}

static void ask_listen(const bridge_api_t *api, char *args, bool again)
{
    ideatek_listen_request_t r = {};
    char *left = args;
    const char *k = again ? field(&left) : "";
    const char *kind = field(&left);
    const char *port = field(&left);
    const char *vport = field(&left);
    uint64_t id = again ? game_sockets.entities[atoi(k) % GAME_SOCKS]
        : api->spawn(api);

    r.kind = index_of(kind, KINDS, 2);
    r.port = (uint16_t)atoi(port);
    r.virtual_port = atoi(vport);
    game_sock_add(id);
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.listen_request, r);
}

static void ask_connect(const bridge_api_t *api, char *args)
{
    ideatek_connection_request_t r = {};
    char *left = args;
    const char *address = field(&left);
    const char *port = field(&left);
    const char *rate_min = field(&left);
    const char *rate_max = field(&left);
    const char *symmetric = field(&left);
    uint64_t id = api->spawn(api);

    r.action = IDEATEK_CONNECTION_ACTION_CONNECT;
    parse_address(&r.address, address, port);
    r.send_rate_min = *rate_min ? (uint32_t)strtoul(rate_min, nullptr, 0)
        : 262144;
    r.send_rate_max = *rate_max ? (uint32_t)strtoul(rate_max, nullptr, 0)
        : 262144;
    r.symmetric = atoi(symmetric) != 0;
    game_sock_add(id);
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.connection_request, r);
}

static void ask_p2p(const bridge_api_t *api, char *args)
{
    ideatek_connection_request_t r = {};
    char *left = args;
    const char *remote = field(&left);
    const char *vport = field(&left);
    const char *symmetric = field(&left);
    uint64_t id = api->spawn(api);

    r.action = IDEATEK_CONNECTION_ACTION_CONNECT_P2P;
    set_identity(&r.remote_identity, remote);
    r.virtual_port = atoi(vport);
    r.send_rate_min = 262144;
    r.send_rate_max = 262144;
    r.symmetric = atoi(symmetric) != 0;
    game_sock_add(id);
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.connection_request, r);
}

static void ask_pair(const bridge_api_t *api)
{
    ideatek_connection_request_t r = {};
    uint64_t id = api->spawn(api);

    r.action = IDEATEK_CONNECTION_ACTION_PAIR;
    r.send_rate_min = 262144;
    r.send_rate_max = 262144;
    game_sock_add(id);
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.connection_request, r);
}

static void ask_action(const bridge_api_t *api, char *args)
{
    ideatek_connection_request_t r = {};
    char *left = args;
    const char *k = field(&left);
    const char *action = field(&left);
    const char *reason = field(&left);
    const char *linger = field(&left);
    const char *rate_min = field(&left);
    const char *rate_max = field(&left);
    uint64_t id = game_sockets.entities[atoi(k) % GAME_SOCKS];

    if ((uint32_t)atoi(k) >= game_sockets.count) {
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u no sock %s", game.round, k);
        return;
    }
    r.action = index_of(action, ACTIONS, 5);
    r.reason = atoi(reason);
    r.linger = atoi(linger) != 0;
    r.send_rate_min = *rate_min ? (uint32_t)strtoul(rate_min, nullptr, 0)
        : 262144;
    r.send_rate_max = *rate_max ? (uint32_t)strtoul(rate_max, nullptr, 0)
        : 262144;
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.connection_request, r);
}

static void ask_lanes(const bridge_api_t *api, char *args)
{
    ideatek_batch_t b = {};
    char *left = args;
    const char *k = field(&left);
    uint8_t records[256 * 6];
    uint32_t count = 0;
    char *lanes = nullptr;
    int32_t priority;
    uint16_t weight;
    uint64_t id = game_sockets.entities[atoi(k) % GAME_SOCKS];
    bool odd = !strcmp(left, "odd");

    if ((uint32_t)atoi(k) >= game_sockets.count) {
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u no sock %s", game.round, k);
        return;
    }

    for (char *lane = strtok_r(left, ";", &lanes); lane && count < 256;
        lane = strtok_r(nullptr, ";", &lanes)) {
        priority = atoi(lane);
        weight = (uint16_t)atoi(strchr(lane, ':') ? strchr(lane, ':') + 1
            : "1");
        memcpy(records + count * 6, &priority, 4);
        memcpy(records + count * 6 + 4, &weight, 2);
        count++;
    }
    b.data = game_sock_bytes(api, records, count * 6);
    if (odd)
        b.data.len = 7;
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.lanes_request, b);
}

static uint32_t put_record(uint8_t *out, uint64_t entity, char *record,
    bool session)
{
    char *left = record;
    const char *lane_or_channel = field(&left);
    const char *flags = field(&left);
    const char *text = left;
    uint32_t len = (uint32_t)strlen(text);
    uint32_t at = 0;
    uint16_t lane = (uint16_t)atoi(lane_or_channel);
    int32_t channel = atoi(lane_or_channel);
    uint8_t f = send_flags(flags);

    if (!strncmp(text, "big", 3))
        len = (uint32_t)strtoul(text + 3, nullptr, 0);
    memcpy(out + at, &entity, 8);
    at += 8;
    if (session) {
        memcpy(out + at, &channel, 4);
        at += 4;
    }
    memcpy(out + at, &len, 4);
    at += 4;
    if (!session) {
        memcpy(out + at, &lane, 2);
        at += 2;
    }
    out[at++] = f;
    if (!strncmp(text, "big", 3))
        memset(out + at, 'x', len);
    else
        memcpy(out + at, text, len);
    return (at + len);
}

static void ask_send(const bridge_api_t *api, char *args, bool session)
{
    ideatek_batch_t b = {};
    static uint8_t batch[1 << 20];
    uint32_t used = 0;
    char *records = nullptr;
    char *comma;
    uint64_t entity;
    uint32_t k;

    for (char *r = strtok_r(args, ";", &records); r;
        r = strtok_r(nullptr, ";", &records)) {
        comma = strchr(r, ',');
        if (!comma)
            continue;
        *comma = '\0';
        k = (uint32_t)atoi(r);
        entity = k < game_sockets.count ? game_sockets.entities[k]
            : 0xdeadULL;
        used += put_record(batch + used, entity, comma + 1, session);
    }
    b.data = game_sock_bytes(api, batch, used);
    if (!strcmp(args, "cut"))
        b.data.len = 9;
    BRIDGE_NATIVE_ATTACH(api, game.window ? game.window
        : (game.window = api->spawn(api)), session
        ? game_sockets_slots.session_messages_out
        : game_sockets_slots.messages_out, b);
}

static void ask_session(const bridge_api_t *api, char *args)
{
    ideatek_session_request_t r = {};
    char *left = args;
    const char *action = field(&left);
    const char *remote = field(&left);
    const char *channel = field(&left);
    const char *k = field(&left);
    uint64_t id;

    r.action = index_of(action, SESSION_ACTIONS, 4);
    set_identity(&r.remote, remote);
    r.channel = atoi(channel);
    if (*k && (uint32_t)atoi(k) >= game_sockets.count) {
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u no sock %s", game.round, k);
        return;
    }
    if (*k)
        id = game_sockets.entities[atoi(k) % GAME_SOCKS];
    else {
        id = api->spawn(api);
        game_sock_add(id);
    }
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.session_request, r);
}

static void ask_address(const bridge_api_t *api, char *args)
{
    ideatek_address_request_t r = {};
    uint64_t id = api->spawn(api);
    uint32_t len = (uint32_t)strlen(args);

    if (!strncmp(args, "long", 4))
        len = (uint32_t)strtoul(args + 4, nullptr, 0);
    r.name = game_sock_bytes(api, args, (uint32_t)strlen(args));
    r.name.len = len;
    if (!strcmp(args, "far"))
        r.name.at = UINT32_MAX - 8;
    game_sock_add(id);
    BRIDGE_NATIVE_ATTACH(api, id, game_sockets_slots.address_request, r);
}

static void raw_signal(const bridge_api_t *api, const char *text, bool cut)
{
    ideatek_batch_t b = {};
    uint8_t batch[512];
    uint32_t len = (uint32_t)strlen(text);

    memcpy(batch, &len, 4);
    memcpy(batch + 4, text, len);
    b.data = game_sock_bytes(api, batch, 4 + len);
    if (cut)
        b.data.len = 2;
    BRIDGE_NATIVE_ATTACH(api, game_sockets.self,
        game_sockets_slots.signals_in, b);
}

bool game_sockets_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "identity"))
        ask_identity(api, args, false);
    else if (!strcmp(verb, "identityother"))
        ask_identity(api, args, true);
    else if (!strcmp(verb, "listen"))
        ask_listen(api, args, false);
    else if (!strcmp(verb, "relisten"))
        ask_listen(api, args, true);
    else if (!strcmp(verb, "rawsignal"))
        raw_signal(api, args, false);
    else if (!strcmp(verb, "badsignal"))
        raw_signal(api, args, true);
    else if (!strcmp(verb, "connect"))
        ask_connect(api, args);
    else if (!strcmp(verb, "p2p"))
        ask_p2p(api, args);
    else if (!strcmp(verb, "pair"))
        ask_pair(api);
    else if (!strcmp(verb, "conn"))
        ask_action(api, args);
    else if (!strcmp(verb, "lanes"))
        ask_lanes(api, args);
    else if (!strcmp(verb, "send"))
        ask_send(api, args, false);
    else if (!strcmp(verb, "sesssend"))
        ask_send(api, args, true);
    else if (!strcmp(verb, "sess"))
        ask_session(api, args);
    else if (!strcmp(verb, "addr"))
        ask_address(api, args);
    else if (!strcmp(verb, "sockkill"))
        api->destroy(api, game_sockets.entities[atoi(args) % GAME_SOCKS]);
    else if (!strcmp(verb, "signals")) {
        strncpy(game_sockets.folder, args, sizeof(game_sockets.folder) - 1);
        SDL_CreateDirectory(args);
    } else
        done = game_sockets_show_step(api, verb);
    return (done);
}

void game_declare_sockets(const bridge_api_t *api)
{
    game_sockets_slots_t *s = &game_sockets_slots;

    TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_address_t", ideatek_address_t);
    TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_identity_t",
        ideatek_identity_t);
    TYPED(api, BRIDGE_BITMASK, "ideatek:ideatek_send_t", ideatek_send_t);
    TYPED(api, BRIDGE_BITMASK, "ideatek:ideatek_connection_flag_t",
        ideatek_connection_flag_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_listen_kind_t",
        ideatek_listen_kind_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_listen_status_t",
        ideatek_listen_status_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_auth_status_t",
        ideatek_auth_status_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_session_action_t",
        ideatek_session_action_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_address_status_t",
        ideatek_address_status_t);
    s->identity_request = TYPED(api, BRIDGE_STRUCT, "ideatek:IdentityRequest",
        ideatek_identity_request_t);
    s->identity_state = TYPED(api, BRIDGE_STRUCT, "ideatek:IdentityState",
        ideatek_identity_state_t);
    s->listen_request = TYPED(api, BRIDGE_STRUCT, "ideatek:ListenRequest",
        ideatek_listen_request_t);
    s->listen_state = TYPED(api, BRIDGE_STRUCT, "ideatek:ListenState",
        ideatek_listen_state_t);
    s->connection_request = TYPED(api, BRIDGE_STRUCT,
        "ideatek:ConnectionRequest", ideatek_connection_request_t);
    s->inbound = TYPED(api, BRIDGE_STRUCT, "ideatek:Inbound",
        ideatek_inbound_t);
    s->connection_state = TYPED(api, BRIDGE_STRUCT, "ideatek:ConnectionState",
        ideatek_connection_state_t);
    s->lanes_request = TYPED(api, BRIDGE_STRUCT, "ideatek:LanesRequest",
        ideatek_batch_t);
    s->lane_states = TYPED(api, BRIDGE_STRUCT, "ideatek:LaneStates",
        ideatek_batch_t);
    s->session = TYPED(api, BRIDGE_STRUCT, "ideatek:Session",
        ideatek_session_t);
    s->session_request = TYPED(api, BRIDGE_STRUCT, "ideatek:SessionRequest",
        ideatek_session_request_t);
    s->messages_out = TYPED(api, BRIDGE_STRUCT, "ideatek:MessagesOut",
        ideatek_batch_t);
    s->messages_in = TYPED(api, BRIDGE_STRUCT, "ideatek:MessagesIn",
        ideatek_batch_t);
    s->session_messages_out = TYPED(api, BRIDGE_STRUCT,
        "ideatek:SessionMessagesOut", ideatek_batch_t);
    s->session_messages_in = TYPED(api, BRIDGE_STRUCT,
        "ideatek:SessionMessagesIn", ideatek_batch_t);
    s->signals_out = TYPED(api, BRIDGE_STRUCT, "ideatek:SignalsOut",
        ideatek_batch_t);
    s->signals_in = TYPED(api, BRIDGE_STRUCT, "ideatek:SignalsIn",
        ideatek_batch_t);
    s->address_request = TYPED(api, BRIDGE_STRUCT, "ideatek:AddressRequest",
        ideatek_address_request_t);
    s->address_result = TYPED(api, BRIDGE_STRUCT, "ideatek:AddressResult",
        ideatek_address_result_t);
}

void game_watch_sockets(const bridge_api_t *api)
{
    game_sockets_watch_shows(api);
}

#endif
