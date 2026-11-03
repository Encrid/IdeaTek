/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SPACE_H
    #define BC_IDEATEK_SPACE_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_world.h"

static constexpr uint8_t IDEATEK_ABSOLUTE_POSITION = 1;
static constexpr uint8_t IDEATEK_ABSOLUTE_ROTATION = 2;
static constexpr uint8_t IDEATEK_ABSOLUTE_SCALE = 4;

    #define IDEATEK_VEC3_FIELDS { float x; float y; float z; }
    #define IDEATEK_QUAT_FIELDS { float x; float y; float z; float w; }
    #define IDEATEK_POSITION_FIELDS { double x; double y; double z; }
    #define IDEATEK_KEEP_VALUES { IDEATEK_KEEP_LOCAL = 0, \
        IDEATEK_KEEP_WORLD = 1 }
    #define IDEATEK_ORPHAN_VALUES { IDEATEK_ORPHAN_DESTROY = 0, \
        IDEATEK_ORPHAN_KEEP_WORLD = 1 }
    #define IDEATEK_TRANSFORM_FIELDS { ideatek_position_t position; \
        ideatek_quat_t rotation; ideatek_vec3_t scale; }
    #define IDEATEK_PARENT_FIELDS { ideatek_entity_t entity; int32_t order; \
        ideatek_keep_t keep; ideatek_orphan_t orphan; uint8_t absolute; }
    #define IDEATEK_WORLD_TRANSFORM_FIELDS { ideatek_position_t origin; \
        float basis[9]; }
    #define IDEATEK_CHILDREN_FIELDS { ideatek_entity_t first; \
        ideatek_entity_t last; uint32_t count; }
    #define IDEATEK_SIBLING_FIELDS { ideatek_entity_t prev; \
        ideatek_entity_t next; }
    #define IDEATEK_PREVIOUS_WORLD_TRANSFORM_FIELDS { \
        ideatek_world_transform_t world; uint64_t tick; }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vec3_t, IDEATEK_VEC3_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vec3_t, IDEATEK_VEC3_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_quat_t, IDEATEK_QUAT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_quat_t, IDEATEK_QUAT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_position_t, IDEATEK_POSITION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_position_t, IDEATEK_POSITION_FIELDS);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_keep_t, uint8_t, IDEATEK_KEEP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_keep_t, uint8_t, IDEATEK_KEEP_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_orphan_t, uint8_t, IDEATEK_ORPHAN_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_orphan_t, uint8_t, IDEATEK_ORPHAN_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_transform_t, IDEATEK_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_transform_t, IDEATEK_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_parent_t, IDEATEK_PARENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_parent_t, IDEATEK_PARENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_world_transform_t,
    IDEATEK_WORLD_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_world_transform_t,
    IDEATEK_WORLD_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_children_t, IDEATEK_CHILDREN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_children_t, IDEATEK_CHILDREN_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sibling_t, IDEATEK_SIBLING_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sibling_t, IDEATEK_SIBLING_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_previous_world_transform_t,
    IDEATEK_PREVIOUS_WORLD_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_previous_world_transform_t,
    IDEATEK_PREVIOUS_WORLD_TRANSFORM_FIELDS);

#endif
