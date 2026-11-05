/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_NAVIGATION_H
    #define BC_IDEATEK_NAVIGATION_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_space.h"

    #define IDEATEK_NAV_MESH_KIND_VALUES { \
        IDEATEK_NAV_MESH_KIND_SURFACE = 0, \
        IDEATEK_NAV_MESH_KIND_VOLUME = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_nav_mesh_kind_t,
    uint8_t, IDEATEK_NAV_MESH_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_nav_mesh_kind_t,
    uint8_t, IDEATEK_NAV_MESH_KIND_VALUES);

    #define IDEATEK_NAV_MESH_FIELDS { \
        ideatek_entity_t data; \
        uint64_t sources; \
        ideatek_vec3_t size; \
        float agent_radius; \
        float agent_height; \
        float max_climb; \
        float max_slope; \
        float cell_size; \
        float cell_height; \
        float tile_size; \
        ideatek_nav_mesh_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_mesh_t, IDEATEK_NAV_MESH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_mesh_t, IDEATEK_NAV_MESH_FIELDS);

    #define IDEATEK_NAV_BUILD_VALUES { \
        IDEATEK_NAV_BUILD_EMPTY = 0, \
        IDEATEK_NAV_BUILD_BUILDING = 1, \
        IDEATEK_NAV_BUILD_READY = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_nav_build_t,
    uint8_t, IDEATEK_NAV_BUILD_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_nav_build_t,
    uint8_t, IDEATEK_NAV_BUILD_VALUES);

    #define IDEATEK_NAV_MESH_STATE_FIELDS { \
        uint32_t tiles; \
        uint32_t revision; \
        ideatek_nav_build_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_mesh_state_t,
    IDEATEK_NAV_MESH_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_mesh_state_t,
    IDEATEK_NAV_MESH_STATE_FIELDS);

    #define IDEATEK_NAV_AREA_FLAGS_VALUES { \
        IDEATEK_NAV_AREA_FLAGS_BLOCKED = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_nav_area_flags_t,
    uint8_t, IDEATEK_NAV_AREA_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_nav_area_flags_t,
    uint8_t, IDEATEK_NAV_AREA_FLAGS_VALUES);

    #define IDEATEK_NAV_AREA_FIELDS { \
        uint64_t groups; \
        float enter_cost; \
        float travel_cost; \
        ideatek_nav_area_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_area_t, IDEATEK_NAV_AREA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_area_t, IDEATEK_NAV_AREA_FIELDS);

    #define IDEATEK_NAV_MODIFIER_FIELDS { \
        ideatek_entity_t area; \
        ideatek_entity_t shape; \
        ideatek_vec3_t size; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_modifier_t, IDEATEK_NAV_MODIFIER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_modifier_t, IDEATEK_NAV_MODIFIER_FIELDS);

    #define IDEATEK_NAV_LINK_FLAGS_VALUES { \
        IDEATEK_NAV_LINK_FLAGS_BIDIRECTIONAL = 0x01, \
        IDEATEK_NAV_LINK_FLAGS_ENABLED = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_nav_link_flags_t,
    uint8_t, IDEATEK_NAV_LINK_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_nav_link_flags_t,
    uint8_t, IDEATEK_NAV_LINK_FLAGS_VALUES);

    #define IDEATEK_NAV_LINK_FIELDS { \
        ideatek_entity_t area; \
        ideatek_vec3_t start; \
        ideatek_vec3_t end; \
        float radius; \
        ideatek_nav_link_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_link_t, IDEATEK_NAV_LINK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_link_t, IDEATEK_NAV_LINK_FIELDS);

    #define IDEATEK_NAV_QUERY_KIND_VALUES { \
        IDEATEK_NAV_QUERY_KIND_PATH = 0, \
        IDEATEK_NAV_QUERY_KIND_PROJECT = 1, \
        IDEATEK_NAV_QUERY_KIND_RAY = 2, \
        IDEATEK_NAV_QUERY_KIND_RANDOM = 3, \
        IDEATEK_NAV_QUERY_KIND_RANDOM_IN_RADIUS = 4, \
        IDEATEK_NAV_QUERY_KIND_EDGE = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_nav_query_kind_t,
    uint8_t, IDEATEK_NAV_QUERY_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_nav_query_kind_t,
    uint8_t, IDEATEK_NAV_QUERY_KIND_VALUES);

    #define IDEATEK_NAV_QUERY_FLAGS_VALUES { \
        IDEATEK_NAV_QUERY_FLAGS_PARTIAL = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_nav_query_flags_t,
    uint8_t, IDEATEK_NAV_QUERY_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_nav_query_flags_t,
    uint8_t, IDEATEK_NAV_QUERY_FLAGS_VALUES);

    #define IDEATEK_NAV_QUERY_FIELDS { \
        ideatek_entity_t nav_mesh; \
        ideatek_entity_t filter; \
        ideatek_position_t start; \
        ideatek_position_t end; \
        uint64_t groups; \
        float radius; \
        ideatek_nav_query_kind_t kind; \
        ideatek_nav_query_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_query_t, IDEATEK_NAV_QUERY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_query_t, IDEATEK_NAV_QUERY_FIELDS);

    #define IDEATEK_NAV_RESULT_VALUES { \
        IDEATEK_NAV_RESULT_PENDING = 0, \
        IDEATEK_NAV_RESULT_FOUND = 1, \
        IDEATEK_NAV_RESULT_PARTIAL = 2, \
        IDEATEK_NAV_RESULT_NONE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_nav_result_t,
    uint8_t, IDEATEK_NAV_RESULT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_nav_result_t,
    uint8_t, IDEATEK_NAV_RESULT_VALUES);

    #define IDEATEK_NAV_POINT_FIELDS { \
        ideatek_position_t position; \
        ideatek_entity_t link; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_point_t, IDEATEK_NAV_POINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_point_t, IDEATEK_NAV_POINT_FIELDS);

    #define IDEATEK_NAV_PATH_FIELDS { \
        bridge_text_t bytes; \
        uint32_t count; \
        float length; \
        uint32_t revision; \
        ideatek_nav_result_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_path_t, IDEATEK_NAV_PATH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_path_t, IDEATEK_NAV_PATH_FIELDS);

    #define IDEATEK_NAV_AGENT_FLAGS_VALUES { \
        IDEATEK_NAV_AGENT_FLAGS_ACTIVE = 0x01, \
        IDEATEK_NAV_AGENT_FLAGS_AVOID = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_nav_agent_flags_t,
    uint8_t, IDEATEK_NAV_AGENT_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_nav_agent_flags_t,
    uint8_t, IDEATEK_NAV_AGENT_FLAGS_VALUES);

    #define IDEATEK_NAV_AGENT_FIELDS { \
        ideatek_entity_t nav_mesh; \
        ideatek_entity_t goal; \
        ideatek_entity_t filter; \
        ideatek_position_t target; \
        uint64_t groups; \
        uint64_t avoid_groups; \
        uint64_t avoid_mask; \
        float max_speed; \
        float max_acceleration; \
        float avoidance_priority; \
        float radius; \
        float arrive_distance; \
        ideatek_nav_agent_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_agent_t, IDEATEK_NAV_AGENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_agent_t, IDEATEK_NAV_AGENT_FIELDS);

    #define IDEATEK_NAV_STEER_STATE_VALUES { \
        IDEATEK_NAV_STEER_STATE_IDLE = 0, \
        IDEATEK_NAV_STEER_STATE_MOVING = 1, \
        IDEATEK_NAV_STEER_STATE_ARRIVED = 2, \
        IDEATEK_NAV_STEER_STATE_BLOCKED = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_nav_steer_state_t,
    uint8_t, IDEATEK_NAV_STEER_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_nav_steer_state_t,
    uint8_t, IDEATEK_NAV_STEER_STATE_VALUES);

    #define IDEATEK_NAV_STEER_FIELDS { \
        ideatek_entity_t link; \
        ideatek_position_t next; \
        ideatek_vec3_t velocity; \
        ideatek_nav_steer_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_steer_t, IDEATEK_NAV_STEER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_steer_t, IDEATEK_NAV_STEER_FIELDS);

    #define IDEATEK_NAV_LINK_REACHED_FIELDS { \
        ideatek_entity_t agent; \
        ideatek_entity_t link; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_link_reached_t,
    IDEATEK_NAV_LINK_REACHED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_link_reached_t,
    IDEATEK_NAV_LINK_REACHED_FIELDS);

    #define IDEATEK_NAV_OBSTACLE_FIELDS { \
        uint64_t avoid_groups; \
        float radius; \
        float height; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_obstacle_t, IDEATEK_NAV_OBSTACLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_obstacle_t, IDEATEK_NAV_OBSTACLE_FIELDS);

    #define IDEATEK_NAV_MESH_DATA_FIELDS { \
        bridge_text_t bytes; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_mesh_data_t,
    IDEATEK_NAV_MESH_DATA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_mesh_data_t,
    IDEATEK_NAV_MESH_DATA_FIELDS);

    #define IDEATEK_NAV_GOAL_FIELDS { \
        ideatek_position_t position; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_goal_t, IDEATEK_NAV_GOAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_goal_t, IDEATEK_NAV_GOAL_FIELDS);

    #define IDEATEK_NAV_FILTER_FIELDS { \
        bridge_text_t costs; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_nav_filter_t, IDEATEK_NAV_FILTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_nav_filter_t, IDEATEK_NAV_FILTER_FIELDS);

#endif
