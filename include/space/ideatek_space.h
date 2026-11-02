/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SPACE_H
    #define BC_IDEATEK_SPACE_H

    #include <stdint.h>

    #include "bridge_schema.h"

    #define IDEATEK_VEC3_FIELDS { float x; float y; float z; }
    #define IDEATEK_QUAT_FIELDS { float x; float y; float z; float w; }
    #define IDEATEK_POSITION_FIELDS { double x; double y; double z; }
    #define IDEATEK_ENTITY_FIELDS { uint64_t id; }
    #define IDEATEK_TRANSFORM_FIELDS { ideatek_position_t position; \
        ideatek_quat_t rotation; ideatek_vec3_t scale; }
    #define IDEATEK_PARENT_FIELDS { ideatek_entity_t entity; }
    #define IDEATEK_WORLD_TRANSFORM_FIELDS { ideatek_position_t origin; \
        float basis[9]; }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vec3_t, IDEATEK_VEC3_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vec3_t, IDEATEK_VEC3_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_quat_t, IDEATEK_QUAT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_quat_t, IDEATEK_QUAT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_position_t, IDEATEK_POSITION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_position_t, IDEATEK_POSITION_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_entity_t, IDEATEK_ENTITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_entity_t, IDEATEK_ENTITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_transform_t, IDEATEK_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_transform_t, IDEATEK_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_parent_t, IDEATEK_PARENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_parent_t, IDEATEK_PARENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_world_transform_t,
    IDEATEK_WORLD_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_world_transform_t,
    IDEATEK_WORLD_TRANSFORM_FIELDS);

#endif
