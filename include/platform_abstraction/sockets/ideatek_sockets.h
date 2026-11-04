/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SOCKETS_H
    #define BC_IDEATEK_SOCKETS_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_ADDRESS_FIELDS { \
        uint8_t bytes[16]; \
        uint16_t port; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_address_t, IDEATEK_ADDRESS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_address_t, IDEATEK_ADDRESS_FIELDS);

    #define IDEATEK_SEND_VALUES { \
        IDEATEK_SEND_RELIABLE = 0x01, \
        IDEATEK_SEND_NO_NAGLE = 0x02, \
        IDEATEK_SEND_NO_DELAY = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_send_t, uint8_t, IDEATEK_SEND_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_send_t, uint8_t, IDEATEK_SEND_VALUES);

    #define IDEATEK_LISTEN_STATUS_VALUES { \
        IDEATEK_LISTEN_STATUS_LISTENING = 0, \
        IDEATEK_LISTEN_STATUS_FAILED = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_listen_status_t, uint8_t,
    IDEATEK_LISTEN_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_listen_status_t, uint8_t,
    IDEATEK_LISTEN_STATUS_VALUES);

    #define IDEATEK_CONNECTION_ACTION_VALUES { \
        IDEATEK_CONNECTION_ACTION_CONNECT = 0, \
        IDEATEK_CONNECTION_ACTION_ACCEPT = 1, \
        IDEATEK_CONNECTION_ACTION_CLOSE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_connection_action_t, uint8_t,
    IDEATEK_CONNECTION_ACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_connection_action_t, uint8_t,
    IDEATEK_CONNECTION_ACTION_VALUES);

    #define IDEATEK_CONNECTION_STATUS_VALUES { \
        IDEATEK_CONNECTION_STATUS_CONNECTING = 0, \
        IDEATEK_CONNECTION_STATUS_CONNECTED = 1, \
        IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER = 2, \
        IDEATEK_CONNECTION_STATUS_PROBLEM = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_connection_status_t, uint8_t,
    IDEATEK_CONNECTION_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_connection_status_t, uint8_t,
    IDEATEK_CONNECTION_STATUS_VALUES);

    #define IDEATEK_ADDRESS_STATUS_VALUES { \
        IDEATEK_ADDRESS_STATUS_OK = 0, \
        IDEATEK_ADDRESS_STATUS_NOT_FOUND = 1, \
        IDEATEK_ADDRESS_STATUS_FAILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_address_status_t, uint8_t,
    IDEATEK_ADDRESS_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_address_status_t, uint8_t,
    IDEATEK_ADDRESS_STATUS_VALUES);

    #define IDEATEK_LISTEN_REQUEST_FIELDS { \
        uint16_t port; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_listen_request_t,
    IDEATEK_LISTEN_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_listen_request_t,
    IDEATEK_LISTEN_REQUEST_FIELDS);

    #define IDEATEK_LISTEN_STATE_FIELDS { \
        uint16_t port; \
        ideatek_listen_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_listen_state_t, IDEATEK_LISTEN_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_listen_state_t, IDEATEK_LISTEN_STATE_FIELDS);

    #define IDEATEK_CONNECTION_REQUEST_FIELDS { \
        uint32_t send_rate; \
        int32_t reason; \
        ideatek_address_t address; \
        ideatek_connection_action_t action; \
        bool linger; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_connection_request_t,
    IDEATEK_CONNECTION_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_connection_request_t,
    IDEATEK_CONNECTION_REQUEST_FIELDS);

    #define IDEATEK_INBOUND_FIELDS { \
        ideatek_entity_t listen; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_inbound_t, IDEATEK_INBOUND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_inbound_t, IDEATEK_INBOUND_FIELDS);

    #define IDEATEK_CONNECTION_STATE_FIELDS { \
        int64_t queue_time; \
        int32_t end_reason; \
        int32_t ping; \
        float quality_local; \
        float quality_remote; \
        float in_bytes; \
        float out_bytes; \
        int32_t pending_reliable; \
        int32_t pending_unreliable; \
        ideatek_address_t remote; \
        ideatek_connection_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_connection_state_t,
    IDEATEK_CONNECTION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_connection_state_t,
    IDEATEK_CONNECTION_STATE_FIELDS);

    #define IDEATEK_MESSAGES_FIELDS { \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_messages_t, IDEATEK_MESSAGES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_messages_t, IDEATEK_MESSAGES_FIELDS);

    #define IDEATEK_ADDRESS_REQUEST_FIELDS { \
        bridge_text_t name; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_address_request_t,
    IDEATEK_ADDRESS_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_address_request_t,
    IDEATEK_ADDRESS_REQUEST_FIELDS);

    #define IDEATEK_ADDRESS_RESULT_FIELDS { \
        bridge_text_t addresses; \
        ideatek_address_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_address_result_t,
    IDEATEK_ADDRESS_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_address_result_t,
    IDEATEK_ADDRESS_RESULT_FIELDS);

#endif
