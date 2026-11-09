/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

static void add_requests(const bridge_api_t *api)
{
    static const uint32_t IDENTITY[] = { IDENTITY_STATE, CONNECTION_STATE,
        LISTEN_STATE };
    static const uint32_t LISTEN[] = { LISTEN_STATE };
    static const uint32_t CONNECTION[] = { CONNECTION_STATE, INBOUND };
    uint32_t *l = sockets_module.lists;
    uint32_t after[1];

    l[LIST_IDENTITY] = SOCKETS_OBSERVER(api, "identity", IDENTITY_REQUEST,
        ON_REQUEST, IDENTITY, 3, nullptr, 0);
    after[0] = l[LIST_IDENTITY];
    l[LIST_LISTEN] = SOCKETS_OBSERVER(api, "listen", LISTEN_REQUEST,
        ON_REQUEST | BRIDGE_ON_LEAVE, LISTEN, 1, after, 1);
    l[LIST_CONNECTION] = SOCKETS_OBSERVER(api, "connection",
        CONNECTION_REQUEST, ON_REQUEST, CONNECTION, 2, after, 1);
}

static void add_sessions(const bridge_api_t *api)
{
    static const uint32_t SESSIONS[] = { SESSION, CONNECTION_STATE };
    static const uint32_t ADDRESS[] = { ADDRESS_RESULT };
    uint32_t *l = sockets_module.lists;
    uint32_t after[2] = { l[LIST_IDENTITY], l[LIST_CONNECTION] };

    l[LIST_LANES] = SOCKETS_OBSERVER(api, "lanes", LANES_REQUEST, ON_REQUEST,
        nullptr, 0, nullptr, 0);
    l[LIST_SESSION] = SOCKETS_OBSERVER(api, "session", SESSION_REQUEST,
        ON_REQUEST, SESSIONS, 2, after, 2);
    l[LIST_ADDRESS] = SOCKETS_OBSERVER(api, "address", ADDRESS_REQUEST,
        ON_REQUEST, ADDRESS, 1, nullptr, 0);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { IDENTITY_STATE, LISTEN_STATE, INBOUND,
        CONNECTION_STATE, LANE_STATES, SESSION, MESSAGES_IN,
        SESSION_MESSAGES_IN, SIGNALS_OUT, ADDRESS_RESULT };
    uint32_t *l = sockets_module.lists;

    BRIDGE_NATIVE_ADD_SYSTEM(api, "round", "first", nullptr, 0, changes, 10,
        nullptr, 0);
    add_requests(api);
    add_sessions(api);
    l[LIST_MESSAGES_OUT] = SOCKETS_OBSERVER(api, "messages_out", MESSAGES_OUT,
        ON_REQUEST, nullptr, 0, nullptr, 0);
    l[LIST_SESSION_MESSAGES_OUT] = SOCKETS_OBSERVER(api,
        "session_messages_out", SESSION_MESSAGES_OUT, ON_REQUEST, nullptr, 0,
        nullptr, 0);
    l[LIST_SIGNALS_IN] = SOCKETS_OBSERVER(api, "signals_in", SIGNALS_IN,
        ON_REQUEST, nullptr, 0, nullptr, 0);
}
