/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_sockets.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_address_t", ideatek_address_t);
    STRUCT(api, "ideatek:ideatek_identity_t", ideatek_identity_t);
    MASK(api, "ideatek:ideatek_send_t", ideatek_send_t);
    MASK(api, "ideatek:ideatek_connection_flag_t", ideatek_connection_flag_t);
    ENUM(api, "ideatek:ideatek_listen_kind_t", ideatek_listen_kind_t);
    ENUM(api, "ideatek:ideatek_listen_status_t", ideatek_listen_status_t);
    ENUM(api, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    ENUM(api, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    ENUM(api, "ideatek:ideatek_auth_status_t", ideatek_auth_status_t);
    ENUM(api, "ideatek:ideatek_session_action_t", ideatek_session_action_t);
    ENUM(api, "ideatek:ideatek_address_status_t", ideatek_address_status_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:IdentityRequest", ideatek_identity_request_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:IdentityRequest", ideatek_identity_request_t);
    STRUCT(api, "ideatek:IdentityState", ideatek_identity_state_t);
    STRUCT(api, "ideatek:ListenRequest", ideatek_listen_request_t);
    STRUCT(api, "ideatek:ListenState", ideatek_listen_state_t);
    STRUCT(api, "ideatek:ConnectionRequest", ideatek_connection_request_t);
    STRUCT(api, "ideatek:Inbound", ideatek_inbound_t);
    STRUCT(api, "ideatek:ConnectionState", ideatek_connection_state_t);
    STRUCT(api, "ideatek:LanesRequest", ideatek_batch_t);
    STRUCT(api, "ideatek:LaneStates", ideatek_batch_t);
    STRUCT(api, "ideatek:Session", ideatek_session_t);
    STRUCT(api, "ideatek:SessionRequest", ideatek_session_request_t);
    STRUCT(api, "ideatek:MessagesOut", ideatek_batch_t);
    STRUCT(api, "ideatek:MessagesIn", ideatek_batch_t);
    STRUCT(api, "ideatek:SessionMessagesOut", ideatek_batch_t);
    STRUCT(api, "ideatek:SessionMessagesIn", ideatek_batch_t);
    STRUCT(api, "ideatek:SignalsOut", ideatek_batch_t);
    STRUCT(api, "ideatek:SignalsIn", ideatek_batch_t);
    STRUCT(api, "ideatek:AddressRequest", ideatek_address_request_t);
    STRUCT(api, "ideatek:AddressResult", ideatek_address_result_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_address_t", ideatek_address_t);
    STRUCT(api, "ideatek:ideatek_identity_t", ideatek_identity_t);
    MASK(api, "ideatek:ideatek_connection_flag_t", ideatek_connection_flag_t);
    ENUM(api, "ideatek:ideatek_listen_kind_t", ideatek_listen_kind_t);
    ENUM(api, "ideatek:ideatek_listen_status_t", ideatek_listen_status_t);
    ENUM(api, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    ENUM(api, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    ENUM(api, "ideatek:ideatek_auth_status_t", ideatek_auth_status_t);
    ENUM(api, "ideatek:ideatek_session_action_t", ideatek_session_action_t);
    ENUM(api, "ideatek:ideatek_address_status_t", ideatek_address_status_t);
    STRUCT(api, "ideatek:IdentityState", ideatek_identity_state_t);
    STRUCT(api, "ideatek:ListenState", ideatek_listen_state_t);
    STRUCT(api, "ideatek:Inbound", ideatek_inbound_t);
    STRUCT(api, "ideatek:ConnectionState", ideatek_connection_state_t);
    STRUCT(api, "ideatek:LaneStates", ideatek_batch_t);
    STRUCT(api, "ideatek:Session", ideatek_session_t);
    STRUCT(api, "ideatek:MessagesIn", ideatek_batch_t);
    STRUCT(api, "ideatek:SessionMessagesIn", ideatek_batch_t);
    STRUCT(api, "ideatek:SignalsOut", ideatek_batch_t);
    STRUCT(api, "ideatek:AddressResult", ideatek_address_result_t);
    api->attach(api, id, STRUCT(api, "ideatek:IdentityRequest",
        ideatek_identity_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ListenRequest",
        ideatek_listen_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ConnectionRequest",
        ideatek_connection_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LanesRequest", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SessionRequest",
        ideatek_session_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MessagesOut", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SessionMessagesOut",
        ideatek_batch_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SignalsIn", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AddressRequest",
        ideatek_address_request_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_address_t", ideatek_address_t);
    STRUCT(api, "ideatek:ideatek_identity_t", ideatek_identity_t);
    MASK(api, "ideatek:ideatek_connection_flag_t", ideatek_connection_flag_t);
    ENUM(api, "ideatek:ideatek_listen_kind_t", ideatek_listen_kind_t);
    ENUM(api, "ideatek:ideatek_listen_status_t", ideatek_listen_status_t);
    ENUM(api, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    ENUM(api, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    ENUM(api, "ideatek:ideatek_auth_status_t", ideatek_auth_status_t);
    ENUM(api, "ideatek:ideatek_session_action_t", ideatek_session_action_t);
    ENUM(api, "ideatek:ideatek_address_status_t", ideatek_address_status_t);
    STRUCT(api, "ideatek:IdentityRequest", ideatek_identity_request_t);
    STRUCT(api, "ideatek:ListenRequest", ideatek_listen_request_t);
    STRUCT(api, "ideatek:ConnectionRequest", ideatek_connection_request_t);
    STRUCT(api, "ideatek:LanesRequest", ideatek_batch_t);
    STRUCT(api, "ideatek:SessionRequest", ideatek_session_request_t);
    STRUCT(api, "ideatek:MessagesOut", ideatek_batch_t);
    STRUCT(api, "ideatek:SessionMessagesOut", ideatek_batch_t);
    STRUCT(api, "ideatek:SignalsIn", ideatek_batch_t);
    STRUCT(api, "ideatek:AddressRequest", ideatek_address_request_t);
    api->attach(api, id, STRUCT(api, "ideatek:IdentityState",
        ideatek_identity_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ListenState",
        ideatek_listen_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Inbound", ideatek_inbound_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ConnectionState",
        ideatek_connection_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LaneStates", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Session", ideatek_session_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MessagesIn", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SessionMessagesIn",
        ideatek_batch_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SignalsOut", ideatek_batch_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AddressResult",
        ideatek_address_result_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_MODULE)
    role_module(api);
#else
    declare_all(api);
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
