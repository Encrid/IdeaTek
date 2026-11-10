/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SCENE_H
    #define BC_IDEATEK_SCENE_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_rendering.h"
    #include "ideatek_space.h"
    #include "ideatek_world.h"

static constexpr uint32_t IDEATEK_MODEL_NONE = 0xFFFFFFFF;

    #define IDEATEK_MODEL_NODE_FIELDS { \
        ideatek_position_t position; \
        ideatek_quat_t rotation; \
        ideatek_vec3_t scale; \
        ideatek_entity_t mesh; \
        ideatek_entity_t binds; \
        uint32_t parent; \
        uint32_t name_offset; \
        uint32_t name_length; \
        uint32_t material_first; \
        uint32_t material_count; \
        uint32_t weight_first; \
        uint32_t weight_count; \
        uint32_t light; \
        uint32_t camera; \
        uint32_t joint; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_model_node_t, IDEATEK_MODEL_NODE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_model_node_t, IDEATEK_MODEL_NODE_FIELDS);

    #define IDEATEK_MODEL_FIELDS { \
        bridge_text_t bytes; \
        uint32_t node_count; \
        uint32_t light_count; \
        uint32_t material_count; \
        uint32_t weight_count; \
        uint32_t camera_count; \
        uint32_t joint_count; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_model_t, IDEATEK_MODEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_model_t, IDEATEK_MODEL_FIELDS);

    #define IDEATEK_MODEL_INSTANCE_FIELDS { \
        ideatek_entity_t model; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_model_instance_t,
    IDEATEK_MODEL_INSTANCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_model_instance_t,
    IDEATEK_MODEL_INSTANCE_FIELDS);

    #define IDEATEK_MODEL_NODE_REF_FIELDS { \
        ideatek_entity_t instance; \
        uint32_t node; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_model_node_ref_t,
    IDEATEK_MODEL_NODE_REF_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_model_node_ref_t,
    IDEATEK_MODEL_NODE_REF_FIELDS);

#endif
