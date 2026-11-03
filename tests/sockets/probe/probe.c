/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_sockets.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef struct {
    uint32_t listen_request;
    uint32_t listen_state;
    uint32_t connection_request;
    uint32_t inbound;
    uint32_t connection_state;
    uint32_t messages_out;
    uint32_t messages_in;
    uint32_t address_request;
    uint32_t address_result;
} slots_t;

static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_address_t", ideatek_address_t);
    MASK(api, "ideatek:ideatek_send_t", ideatek_send_t);
    ENUM(api, "ideatek:ideatek_listen_status_t", ideatek_listen_status_t);
    ENUM(api, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    ENUM(api, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    ENUM(api, "ideatek:ideatek_address_status_t", ideatek_address_status_t);
}

static slots_t declare_structs(const bridge_api_t *api)
{
    return ((slots_t){
        .listen_request = STRUCT(api, "ideatek:ListenRequest",
            ideatek_listen_request_t),
        .listen_state = STRUCT(api, "ideatek:ListenState",
            ideatek_listen_state_t),
        .connection_request = STRUCT(api, "ideatek:ConnectionRequest",
            ideatek_connection_request_t),
        .inbound = STRUCT(api, "ideatek:Inbound", ideatek_inbound_t),
        .connection_state = STRUCT(api, "ideatek:ConnectionState",
            ideatek_connection_state_t),
        .messages_out = STRUCT(api, "ideatek:MessagesOut",
            ideatek_messages_t),
        .messages_in = STRUCT(api, "ideatek:MessagesIn", ideatek_messages_t),
        .address_request = STRUCT(api, "ideatek:AddressRequest",
            ideatek_address_request_t),
        .address_result = STRUCT(api, "ideatek:AddressResult",
            ideatek_address_result_t)
    });
}

[[maybe_unused]] static void ask(const bridge_api_t *api, slots_t slots)
{
    ideatek_connection_request_t request = { .send_rate = 262144,
        .address = { .bytes = { [15] = 1 }, .port = 27015 },
        .action = IDEATEK_CONNECTION_ACTION_CONNECT };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.connection_request,
        request);
}

[[maybe_unused]] static void report(const bridge_api_t *api, slots_t slots)
{
    ideatek_connection_state_t state = {
        .status = IDEATEK_CONNECTION_STATUS_CONNECTING };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.connection_state,
        state);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_structs(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    declare_fields(api);
    ask(api, declare_structs(api));
#elif defined(ROLE_SOCKETS)
    declare_fields(api);
    report(api, declare_structs(api));
#else
    declare_fields(api);
    declare_structs(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
