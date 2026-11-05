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

    #define IDEATEK_NET_AUTHORITY_VALUES { \
        IDEATEK_NET_AUTHORITY_SERVER = 0, \
        IDEATEK_NET_AUTHORITY_OWNER = 1, \
        IDEATEK_NET_AUTHORITY_HOST = 2, \
        IDEATEK_NET_AUTHORITY_DETERMINISTIC = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_net_authority_t,
    uint8_t, IDEATEK_NET_AUTHORITY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_net_authority_t,
    uint8_t, IDEATEK_NET_AUTHORITY_VALUES);

    #define IDEATEK_REPLICATED_FIELDS { \
        uint64_t prefab; \
        uint64_t group; \
        uint32_t owner; \
        uint32_t host; \
        ideatek_net_authority_t authority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_replicated_t, IDEATEK_REPLICATED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_replicated_t, IDEATEK_REPLICATED_FIELDS);

    #define IDEATEK_NET_VIEWER_FIELDS { \
        uint32_t peer; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_viewer_t, IDEATEK_NET_VIEWER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_viewer_t, IDEATEK_NET_VIEWER_FIELDS);

    #define IDEATEK_RELEVANCE_MODE_VALUES { \
        IDEATEK_RELEVANCE_MODE_DISTANCE = 0, \
        IDEATEK_RELEVANCE_MODE_ALWAYS = 1, \
        IDEATEK_RELEVANCE_MODE_OWNER = 2, \
        IDEATEK_RELEVANCE_MODE_NONE = 3, \
        IDEATEK_RELEVANCE_MODE_GROUP = 4, \
        IDEATEK_RELEVANCE_MODE_VISIBLE = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_relevance_mode_t,
    uint8_t, IDEATEK_RELEVANCE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_relevance_mode_t,
    uint8_t, IDEATEK_RELEVANCE_MODE_VALUES);

    #define IDEATEK_NET_RELEVANCE_FIELDS { \
        uint64_t groups; \
        float distance; \
        float priority; \
        uint32_t steps; \
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
        uint32_t confirmed; \
        uint32_t host; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_session_state_t,
    IDEATEK_NET_SESSION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_session_state_t,
    IDEATEK_NET_SESSION_STATE_FIELDS);

    #define IDEATEK_NET_PEER_GROUPS_FIELDS { \
        uint64_t groups; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_peer_groups_t,
    IDEATEK_NET_PEER_GROUPS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_peer_groups_t,
    IDEATEK_NET_PEER_GROUPS_FIELDS);

    #define IDEATEK_NET_PREDICTED_SPAWN_FIELDS { \
        uint64_t key; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_predicted_spawn_t,
    IDEATEK_NET_PREDICTED_SPAWN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_predicted_spawn_t,
    IDEATEK_NET_PREDICTED_SPAWN_FIELDS);

    #define IDEATEK_NET_ROLLBACK_FIELDS { \
        uint64_t group; \
        uint32_t tick; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_rollback_t, IDEATEK_NET_ROLLBACK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_rollback_t, IDEATEK_NET_ROLLBACK_FIELDS);

    #define IDEATEK_NET_AUTHORITY_CHANGED_FIELDS { \
        ideatek_entity_t entity; \
        uint32_t old_host; \
        uint32_t new_host; \
        uint32_t order; \
        ideatek_net_authority_t old_authority; \
        ideatek_net_authority_t new_authority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_authority_changed_t,
    IDEATEK_NET_AUTHORITY_CHANGED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_authority_changed_t,
    IDEATEK_NET_AUTHORITY_CHANGED_FIELDS);

    #define IDEATEK_NET_HOST_CHANGED_FIELDS { \
        uint32_t old_host; \
        uint32_t new_host; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_net_host_changed_t,
    IDEATEK_NET_HOST_CHANGED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_net_host_changed_t,
    IDEATEK_NET_HOST_CHANGED_FIELDS);

#endif
