/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_NETWORKING_H
    #define BC_IDEATEK_NETWORKING_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_NET_ROLE_VALUES { \
        IDEATEK_NET_ROLE_SERVER = 0, \
        IDEATEK_NET_ROLE_CLIENT = 1, \
        IDEATEK_NET_ROLE_HOST = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_net_role_t, uint8_t, IDEATEK_NET_ROLE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_net_role_t, uint8_t, IDEATEK_NET_ROLE_VALUES);

    #define IDEATEK_NET_SESSION_FIELDS { \
        ideatek_entity_t connection; \
        ideatek_entity_t listen; \
        uint32_t protocol; \
        ideatek_net_role_t role; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_session_t, IDEATEK_NET_SESSION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_session_t, IDEATEK_NET_SESSION_FIELDS);

    #define IDEATEK_PEER_STATE_VALUES { \
        IDEATEK_PEER_STATE_JOINING = 0, \
        IDEATEK_PEER_STATE_READY = 1, \
        IDEATEK_PEER_STATE_REFUSED = 2, \
        IDEATEK_PEER_STATE_LEFT = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_peer_state_t,
    uint8_t, IDEATEK_PEER_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_peer_state_t,
    uint8_t, IDEATEK_PEER_STATE_VALUES);

    #define IDEATEK_NET_PEER_FIELDS { \
        ideatek_entity_t session; \
        ideatek_entity_t connection; \
        uint32_t id; \
        ideatek_peer_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_peer_t, IDEATEK_NET_PEER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_peer_t, IDEATEK_NET_PEER_FIELDS);

    #define IDEATEK_NET_JOIN_FIELDS { \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_join_t, IDEATEK_NET_JOIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_join_t, IDEATEK_NET_JOIN_FIELDS);

    #define IDEATEK_NET_JOIN_EVENT_FIELDS { \
        ideatek_entity_t peer; \
        bridge_text_t data; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_join_event_t,
    IDEATEK_NET_JOIN_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_join_event_t,
    IDEATEK_NET_JOIN_EVENT_FIELDS);

    #define IDEATEK_ADMIT_VALUES { \
        IDEATEK_ADMIT_ACCEPT = 0, \
        IDEATEK_ADMIT_REFUSE = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_admit_t, uint8_t, IDEATEK_ADMIT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_admit_t, uint8_t, IDEATEK_ADMIT_VALUES);

    #define IDEATEK_NET_ADMIT_FIELDS { \
        int32_t reason; \
        ideatek_admit_t action; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_admit_t, IDEATEK_NET_ADMIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_admit_t, IDEATEK_NET_ADMIT_FIELDS);

    #define IDEATEK_REPLICATED_FIELDS { \
        uint64_t prefab; \
        uint32_t owner; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_replicated_t, IDEATEK_REPLICATED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_replicated_t, IDEATEK_REPLICATED_FIELDS);

    #define IDEATEK_NET_ENTITY_FIELDS { \
        uint64_t net_id; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_entity_t, IDEATEK_NET_ENTITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_entity_t, IDEATEK_NET_ENTITY_FIELDS);

    #define IDEATEK_NET_KIND_VALUES { \
        IDEATEK_NET_KIND_STATE = 0, \
        IDEATEK_NET_KIND_EVENT = 1, \
        IDEATEK_NET_KIND_PREDICTED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_net_kind_t, uint8_t, IDEATEK_NET_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_net_kind_t, uint8_t, IDEATEK_NET_KIND_VALUES);

    #define IDEATEK_NET_OUT_FIELDS { \
        uint64_t tag; \
        bridge_text_t data; \
        uint32_t peer; \
        ideatek_net_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_out_t, IDEATEK_NET_OUT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_out_t, IDEATEK_NET_OUT_FIELDS);

    #define IDEATEK_NET_IN_FIELDS { \
        uint64_t tag; \
        bridge_text_t data; \
        uint32_t peer; \
        ideatek_net_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_in_t, IDEATEK_NET_IN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_in_t, IDEATEK_NET_IN_FIELDS);

    #define IDEATEK_NET_VIEWER_FIELDS { \
        uint32_t peer; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_viewer_t, IDEATEK_NET_VIEWER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_viewer_t, IDEATEK_NET_VIEWER_FIELDS);

    #define IDEATEK_RELEVANCE_MODE_VALUES { \
        IDEATEK_RELEVANCE_MODE_DISTANCE = 0, \
        IDEATEK_RELEVANCE_MODE_ALWAYS = 1, \
        IDEATEK_RELEVANCE_MODE_OWNER = 2, \
        IDEATEK_RELEVANCE_MODE_NONE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_relevance_mode_t,
    uint8_t, IDEATEK_RELEVANCE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_relevance_mode_t,
    uint8_t, IDEATEK_RELEVANCE_MODE_VALUES);

    #define IDEATEK_NET_RELEVANCE_FIELDS { \
        float distance; \
        float priority; \
        ideatek_relevance_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_relevance_t,
    IDEATEK_NET_RELEVANCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_relevance_t,
    IDEATEK_NET_RELEVANCE_FIELDS);

    #define IDEATEK_NET_COMMAND_FIELDS { \
        bridge_text_t data; \
        uint32_t tick; \
        uint32_t view_tick; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_command_t, IDEATEK_NET_COMMAND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_command_t, IDEATEK_NET_COMMAND_FIELDS);

    #define IDEATEK_NET_COMMAND_IN_FIELDS { \
        bridge_text_t data; \
        uint32_t peer; \
        uint32_t tick; \
        uint32_t view_tick; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_command_in_t,
    IDEATEK_NET_COMMAND_IN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_command_in_t,
    IDEATEK_NET_COMMAND_IN_FIELDS);

    #define IDEATEK_NET_SESSION_STATE_FIELDS { \
        uint32_t tick; \
        uint32_t command_tick; \
        uint32_t playback; \
        uint32_t ack; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_session_state_t,
    IDEATEK_NET_SESSION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_session_state_t,
    IDEATEK_NET_SESSION_STATE_FIELDS);

#endif
