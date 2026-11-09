/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

static void declare_scalars(const bridge_api_t *api)
{
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_address_t",
        ideatek_address_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_identity_t",
        ideatek_identity_t);
    SOCKETS_TYPE(api, BRIDGE_BITMASK, "ideatek:ideatek_send_t",
        ideatek_send_t);
    SOCKETS_TYPE(api, BRIDGE_BITMASK, "ideatek:ideatek_connection_flag_t",
        ideatek_connection_flag_t);
}

static void declare_enums(const bridge_api_t *api)
{
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_listen_kind_t",
        ideatek_listen_kind_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_listen_status_t",
        ideatek_listen_status_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_connection_action_t",
        ideatek_connection_action_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_connection_status_t",
        ideatek_connection_status_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_auth_status_t",
        ideatek_auth_status_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_session_action_t",
        ideatek_session_action_t);
    SOCKETS_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_address_status_t",
        ideatek_address_status_t);
}

static void declare_batches(const bridge_api_t *api)
{
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:LanesRequest", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:LaneStates", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:Session", ideatek_session_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:SessionRequest",
        ideatek_session_request_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:MessagesOut", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:MessagesIn", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:SessionMessagesOut",
        ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:SessionMessagesIn",
        ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:SignalsOut", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:SignalsIn", ideatek_batch_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:AddressRequest",
        ideatek_address_request_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:AddressResult",
        ideatek_address_result_t);
}

void declare_types(const bridge_api_t *api)
{
    declare_scalars(api);
    declare_enums(api);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:IdentityRequest",
        ideatek_identity_request_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:IdentityState",
        ideatek_identity_state_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ListenRequest",
        ideatek_listen_request_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ListenState",
        ideatek_listen_state_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ConnectionRequest",
        ideatek_connection_request_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:Inbound", ideatek_inbound_t);
    SOCKETS_TYPE(api, BRIDGE_STRUCT, "ideatek:ConnectionState",
        ideatek_connection_state_t);
    declare_batches(api);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek_sockets_gns:Settings",
        sockets_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}
