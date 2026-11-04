/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_TERRAIN_H
    #define BC_IDEATEK_TERRAIN_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_space.h"

    #define IDEATEK_TERRAIN_FLAGS_VALUES { \
        IDEATEK_TERRAIN_FLAGS_COLLIDE = 0x01, \
        IDEATEK_TERRAIN_FLAGS_NAVIGATE = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_terrain_flags_t,
    uint8_t, IDEATEK_TERRAIN_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_terrain_flags_t,
    uint8_t, IDEATEK_TERRAIN_FLAGS_VALUES);

    #define IDEATEK_TERRAIN_FIELDS { \
        float cell_size; \
        uint32_t chunk_cells; \
        ideatek_terrain_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_t, IDEATEK_TERRAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_t, IDEATEK_TERRAIN_FIELDS);

    #define IDEATEK_TERRAIN_MATERIAL_FIELDS { \
        ideatek_entity_t terrain; \
        ideatek_entity_t material; \
        ideatek_entity_t physics_material; \
        uint8_t index; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_material_t,
    IDEATEK_TERRAIN_MATERIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_material_t,
    IDEATEK_TERRAIN_MATERIAL_FIELDS);

    #define IDEATEK_TERRAIN_CHUNK_FIELDS { \
        ideatek_entity_t terrain; \
        bridge_text_t bytes; \
        int32_t x; \
        int32_t y; \
        int32_t z; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_chunk_t,
    IDEATEK_TERRAIN_CHUNK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_chunk_t,
    IDEATEK_TERRAIN_CHUNK_FIELDS);

    #define IDEATEK_TERRAIN_SHAPE_VALUES { \
        IDEATEK_TERRAIN_SHAPE_SPHERE = 0, \
        IDEATEK_TERRAIN_SHAPE_BOX = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_terrain_shape_t,
    uint8_t, IDEATEK_TERRAIN_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_terrain_shape_t,
    uint8_t, IDEATEK_TERRAIN_SHAPE_VALUES);

    #define IDEATEK_TERRAIN_OP_VALUES { \
        IDEATEK_TERRAIN_OP_ADD = 0, \
        IDEATEK_TERRAIN_OP_SUBTRACT = 1, \
        IDEATEK_TERRAIN_OP_PAINT = 2, \
        IDEATEK_TERRAIN_OP_SMOOTH = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_terrain_op_t,
    uint8_t, IDEATEK_TERRAIN_OP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_terrain_op_t,
    uint8_t, IDEATEK_TERRAIN_OP_VALUES);

    #define IDEATEK_TERRAIN_EDIT_FIELDS { \
        ideatek_entity_t terrain; \
        ideatek_position_t center; \
        ideatek_vec3_t size; \
        float strength; \
        uint32_t order; \
        ideatek_terrain_shape_t shape; \
        ideatek_terrain_op_t op; \
        uint8_t material; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_edit_t, IDEATEK_TERRAIN_EDIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_edit_t, IDEATEK_TERRAIN_EDIT_FIELDS);

    #define IDEATEK_TERRAIN_FILL_FIELDS { \
        ideatek_entity_t terrain; \
        bridge_text_t data; \
        int32_t x; \
        int32_t y; \
        int32_t z; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_fill_t, IDEATEK_TERRAIN_FILL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_fill_t, IDEATEK_TERRAIN_FILL_FIELDS);

#endif
