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

    #define IDEATEK_TERRAIN_KIND_VALUES { \
        IDEATEK_TERRAIN_KIND_VOXEL = 0, \
        IDEATEK_TERRAIN_KIND_HEIGHTMAP = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_terrain_kind_t,
    uint8_t, IDEATEK_TERRAIN_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_terrain_kind_t,
    uint8_t, IDEATEK_TERRAIN_KIND_VALUES);

    #define IDEATEK_TERRAIN_CHANNELS_VALUES { \
        IDEATEK_TERRAIN_CHANNELS_SDF = 0x01, \
        IDEATEK_TERRAIN_CHANNELS_TYPE = 0x02, \
        IDEATEK_TERRAIN_CHANNELS_INDICES = 0x04, \
        IDEATEK_TERRAIN_CHANNELS_WEIGHTS = 0x08, \
        IDEATEK_TERRAIN_CHANNELS_STATE = 0x10, \
        IDEATEK_TERRAIN_CHANNELS_LIGHT = 0x20, \
        IDEATEK_TERRAIN_CHANNELS_DATA = 0x40 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_terrain_channels_t,
    uint8_t, IDEATEK_TERRAIN_CHANNELS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_terrain_channels_t,
    uint8_t, IDEATEK_TERRAIN_CHANNELS_VALUES);

    #define IDEATEK_TERRAIN_FLAGS_VALUES { \
        IDEATEK_TERRAIN_FLAGS_COLLIDE = 0x01, \
        IDEATEK_TERRAIN_FLAGS_NAVIGATE = 0x02, \
        IDEATEK_TERRAIN_FLAGS_MESH = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_terrain_flags_t,
    uint8_t, IDEATEK_TERRAIN_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_terrain_flags_t,
    uint8_t, IDEATEK_TERRAIN_FLAGS_VALUES);

    #define IDEATEK_TERRAIN_FIELDS { \
        ideatek_entity_t heights; \
        ideatek_entity_t weights; \
        ideatek_entity_t holes; \
        float cell_size; \
        float height_scale; \
        uint32_t chunk_cells; \
        uint32_t component_quads; \
        uint16_t background_type; \
        ideatek_terrain_kind_t kind; \
        ideatek_terrain_channels_t channels; \
        ideatek_terrain_flags_t flags; \
        bool background_solid; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_t, IDEATEK_TERRAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_t, IDEATEK_TERRAIN_FIELDS);

    #define IDEATEK_BLOCK_SHAPE_VALUES { \
        IDEATEK_BLOCK_SHAPE_CUBE = 0, \
        IDEATEK_BLOCK_SHAPE_MESH = 1, \
        IDEATEK_BLOCK_SHAPE_NONE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_block_shape_t,
    uint8_t, IDEATEK_BLOCK_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_block_shape_t,
    uint8_t, IDEATEK_BLOCK_SHAPE_VALUES);

    #define IDEATEK_BLOCK_ROTATION_VALUES { \
        IDEATEK_BLOCK_ROTATION_NONE = 0, \
        IDEATEK_BLOCK_ROTATION_FACEDIR = 1, \
        IDEATEK_BLOCK_ROTATION_FOUR_DIR = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_block_rotation_t,
    uint8_t, IDEATEK_BLOCK_ROTATION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_block_rotation_t,
    uint8_t, IDEATEK_BLOCK_ROTATION_VALUES);

    #define IDEATEK_TERRAIN_MATERIAL_FIELDS { \
        ideatek_entity_t terrain; \
        ideatek_entity_t material; \
        ideatek_entity_t physics_material; \
        ideatek_entity_t model; \
        uint16_t index; \
        ideatek_block_shape_t shape; \
        ideatek_block_rotation_t rotation; \
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
        IDEATEK_TERRAIN_SHAPE_BOX = 1, \
        IDEATEK_TERRAIN_SHAPE_POINT = 2, \
        IDEATEK_TERRAIN_SHAPE_PATH = 3, \
        IDEATEK_TERRAIN_SHAPE_MESH = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_terrain_shape_t,
    uint8_t, IDEATEK_TERRAIN_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_terrain_shape_t,
    uint8_t, IDEATEK_TERRAIN_SHAPE_VALUES);

    #define IDEATEK_TERRAIN_OP_VALUES { \
        IDEATEK_TERRAIN_OP_ADD = 0, \
        IDEATEK_TERRAIN_OP_SUBTRACT = 1, \
        IDEATEK_TERRAIN_OP_PAINT = 2, \
        IDEATEK_TERRAIN_OP_SMOOTH = 3, \
        IDEATEK_TERRAIN_OP_SET = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_terrain_op_t,
    uint8_t, IDEATEK_TERRAIN_OP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_terrain_op_t,
    uint8_t, IDEATEK_TERRAIN_OP_VALUES);

    #define IDEATEK_TERRAIN_EDIT_FIELDS { \
        ideatek_entity_t terrain; \
        ideatek_entity_t mesh; \
        ideatek_position_t center; \
        ideatek_position_t end; \
        ideatek_vec3_t size; \
        float strength; \
        uint32_t order; \
        uint16_t material; \
        uint16_t type; \
        ideatek_terrain_shape_t shape; \
        ideatek_terrain_op_t op; \
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

    #define IDEATEK_VIEWER_FLAGS_VALUES { \
        IDEATEK_VIEWER_FLAGS_VISUALS = 0x01, \
        IDEATEK_VIEWER_FLAGS_COLLISIONS = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_viewer_flags_t,
    uint8_t, IDEATEK_VIEWER_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_viewer_flags_t,
    uint8_t, IDEATEK_VIEWER_FLAGS_VALUES);

    #define IDEATEK_TERRAIN_VIEWER_FIELDS { \
        float radius; \
        float vertical_ratio; \
        uint32_t peer; \
        ideatek_viewer_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_viewer_t,
    IDEATEK_TERRAIN_VIEWER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_viewer_t,
    IDEATEK_TERRAIN_VIEWER_FIELDS);

    #define IDEATEK_TERRAIN_SPLIT_FIELDS { \
        ideatek_entity_t source; \
        ideatek_entity_t piece; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_split_t,
    IDEATEK_TERRAIN_SPLIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_split_t,
    IDEATEK_TERRAIN_SPLIT_FIELDS);

    #define IDEATEK_TERRAIN_SCATTER_FIELDS { \
        ideatek_entity_t terrain; \
        ideatek_entity_t mesh; \
        float density; \
        float min_scale; \
        float max_scale; \
        uint32_t seed; \
        uint16_t material; \
        bool align; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_terrain_scatter_t,
    IDEATEK_TERRAIN_SCATTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_terrain_scatter_t,
    IDEATEK_TERRAIN_SCATTER_FIELDS);

#endif
