/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_EFFECTS_H
    #define BC_IDEATEK_EFFECTS_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_rendering.h"

    #define IDEATEK_CURVE_KIND_VALUES { \
        IDEATEK_CURVE_KIND_FLOAT = 0, \
        IDEATEK_CURVE_KIND_COLOR = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_curve_kind_t,
    uint8_t, IDEATEK_CURVE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_curve_kind_t,
    uint8_t, IDEATEK_CURVE_KIND_VALUES);

    #define IDEATEK_CURVE_FIELDS { \
        bridge_text_t bytes; \
        uint32_t key_count; \
        uint32_t revision; \
        ideatek_curve_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_curve_t, IDEATEK_CURVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_curve_t, IDEATEK_CURVE_FIELDS);

    #define IDEATEK_EMITTER_STATE_VALUES { \
        IDEATEK_EMITTER_STATE_STOPPED = 0, \
        IDEATEK_EMITTER_STATE_PLAYING = 1, \
        IDEATEK_EMITTER_STATE_PAUSED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_emitter_state_t,
    uint8_t, IDEATEK_EMITTER_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_emitter_state_t,
    uint8_t, IDEATEK_EMITTER_STATE_VALUES);

    #define IDEATEK_EMITTER_FLAGS_VALUES { \
        IDEATEK_EMITTER_FLAGS_ONE_SHOT = 0x01, \
        IDEATEK_EMITTER_FLAGS_LOCAL = 0x02, \
        IDEATEK_EMITTER_FLAGS_DESTROY_AT_END = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_emitter_flags_t,
    uint8_t, IDEATEK_EMITTER_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_emitter_flags_t,
    uint8_t, IDEATEK_EMITTER_FLAGS_VALUES);

    #define IDEATEK_PARTICLE_EMITTER_FIELDS { \
        float rate; \
        float rate_per_meter; \
        float duration; \
        float delay; \
        float burst_interval; \
        float lifetime_min; \
        float lifetime_max; \
        float speed_scale; \
        float prewarm; \
        uint32_t burst; \
        uint32_t max; \
        uint32_t seed; \
        ideatek_emitter_state_t state; \
        ideatek_emitter_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_emitter_t,
    IDEATEK_PARTICLE_EMITTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_emitter_t,
    IDEATEK_PARTICLE_EMITTER_FIELDS);

    #define IDEATEK_EMISSION_SHAPE_VALUES { \
        IDEATEK_EMISSION_SHAPE_POINT = 0, \
        IDEATEK_EMISSION_SHAPE_SPHERE = 1, \
        IDEATEK_EMISSION_SHAPE_SPHERE_SURFACE = 2, \
        IDEATEK_EMISSION_SHAPE_BOX = 3, \
        IDEATEK_EMISSION_SHAPE_RING = 4, \
        IDEATEK_EMISSION_SHAPE_MESH = 5, \
        IDEATEK_EMISSION_SHAPE_MESH_SURFACE = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_emission_shape_t,
    uint8_t, IDEATEK_EMISSION_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_emission_shape_t,
    uint8_t, IDEATEK_EMISSION_SHAPE_VALUES);

    #define IDEATEK_PARTICLE_SHAPE_FIELDS { \
        ideatek_entity_t mesh; \
        ideatek_vec3_t size; \
        ideatek_emission_shape_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_shape_t,
    IDEATEK_PARTICLE_SHAPE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_shape_t,
    IDEATEK_PARTICLE_SHAPE_FIELDS);

    #define IDEATEK_PARTICLE_VELOCITY_FIELDS { \
        ideatek_vec3_t min; \
        ideatek_vec3_t max; \
        float radial_min; \
        float radial_max; \
        float inherit_velocity; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_velocity_t,
    IDEATEK_PARTICLE_VELOCITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_velocity_t,
    IDEATEK_PARTICLE_VELOCITY_FIELDS);

    #define IDEATEK_PARTICLE_FORCES_FIELDS { \
        ideatek_vec3_t gravity; \
        float drag; \
        float noise_strength; \
        float noise_frequency; \
        float radial_acceleration; \
        float tangential_acceleration; \
        float max_speed; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_forces_t,
    IDEATEK_PARTICLE_FORCES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_forces_t,
    IDEATEK_PARTICLE_FORCES_FIELDS);

    #define IDEATEK_PARTICLE_EMITTER_STATE_FIELDS { \
        uint32_t alive; \
        bool done; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_emitter_state_t,
    IDEATEK_PARTICLE_EMITTER_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_emitter_state_t,
    IDEATEK_PARTICLE_EMITTER_STATE_FIELDS);

    #define IDEATEK_PARTICLE_ENDED_FIELDS { \
        ideatek_entity_t emitter; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_ended_t,
    IDEATEK_PARTICLE_ENDED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_ended_t,
    IDEATEK_PARTICLE_ENDED_FIELDS);

    #define IDEATEK_PARTICLE_KIND_VALUES { \
        IDEATEK_PARTICLE_KIND_SPRITE = 0, \
        IDEATEK_PARTICLE_KIND_MESH = 1, \
        IDEATEK_PARTICLE_KIND_RIBBON = 2, \
        IDEATEK_PARTICLE_KIND_TRAIL = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_particle_kind_t,
    uint8_t, IDEATEK_PARTICLE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_particle_kind_t,
    uint8_t, IDEATEK_PARTICLE_KIND_VALUES);

    #define IDEATEK_PARTICLE_ALIGN_VALUES { \
        IDEATEK_PARTICLE_ALIGN_CAMERA = 0, \
        IDEATEK_PARTICLE_ALIGN_VELOCITY = 1, \
        IDEATEK_PARTICLE_ALIGN_HORIZONTAL = 2, \
        IDEATEK_PARTICLE_ALIGN_VERTICAL = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_particle_align_t,
    uint8_t, IDEATEK_PARTICLE_ALIGN_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_particle_align_t,
    uint8_t, IDEATEK_PARTICLE_ALIGN_VALUES);

    #define IDEATEK_PARTICLE_SORT_VALUES { \
        IDEATEK_PARTICLE_SORT_NONE = 0, \
        IDEATEK_PARTICLE_SORT_DEPTH = 1, \
        IDEATEK_PARTICLE_SORT_AGE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_particle_sort_t,
    uint8_t, IDEATEK_PARTICLE_SORT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_particle_sort_t,
    uint8_t, IDEATEK_PARTICLE_SORT_VALUES);

    #define IDEATEK_PARTICLE_LOOK_FIELDS { \
        ideatek_entity_t material; \
        ideatek_entity_t mesh; \
        ideatek_entity_t size_curve; \
        ideatek_entity_t color_curve; \
        ideatek_entity_t start_color_curve; \
        ideatek_color_t color; \
        float size_min; \
        float size_max; \
        float rotation_min; \
        float rotation_max; \
        float spin_min; \
        float spin_max; \
        float frame_rate; \
        float trail_lifetime; \
        uint32_t frames_x; \
        uint32_t frames_y; \
        ideatek_particle_kind_t kind; \
        ideatek_particle_align_t align; \
        ideatek_particle_sort_t sort; \
        bool frame_random; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_look_t,
    IDEATEK_PARTICLE_LOOK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_look_t,
    IDEATEK_PARTICLE_LOOK_FIELDS);

    #define IDEATEK_COLLISION_SOURCE_VALUES { \
        IDEATEK_COLLISION_SOURCE_DEPTH = 0, \
        IDEATEK_COLLISION_SOURCE_SHAPES = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_collision_source_t,
    uint8_t, IDEATEK_COLLISION_SOURCE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_collision_source_t,
    uint8_t, IDEATEK_COLLISION_SOURCE_VALUES);

    #define IDEATEK_COLLISION_MODE_VALUES { \
        IDEATEK_COLLISION_MODE_BOUNCE = 0, \
        IDEATEK_COLLISION_MODE_KILL = 1, \
        IDEATEK_COLLISION_MODE_STICK = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_collision_mode_t,
    uint8_t, IDEATEK_COLLISION_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_collision_mode_t,
    uint8_t, IDEATEK_COLLISION_MODE_VALUES);

    #define IDEATEK_PARTICLE_COLLISION_FIELDS { \
        uint64_t touches; \
        float bounce; \
        float friction; \
        float radius; \
        ideatek_collision_source_t source; \
        ideatek_collision_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_collision_t,
    IDEATEK_PARTICLE_COLLISION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_collision_t,
    IDEATEK_PARTICLE_COLLISION_FIELDS);

    #define IDEATEK_SPAWN_ON_VALUES { \
        IDEATEK_SPAWN_ON_START = 0, \
        IDEATEK_SPAWN_ON_END = 1, \
        IDEATEK_SPAWN_ON_COLLISION = 2, \
        IDEATEK_SPAWN_ON_CONSTANT = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_spawn_on_t, uint8_t, IDEATEK_SPAWN_ON_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_spawn_on_t, uint8_t, IDEATEK_SPAWN_ON_VALUES);

    #define IDEATEK_PARTICLE_SPAWN_FIELDS { \
        ideatek_entity_t parent; \
        float rate; \
        float inherit_velocity; \
        uint32_t count; \
        ideatek_spawn_on_t on; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_spawn_t,
    IDEATEK_PARTICLE_SPAWN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_spawn_t,
    IDEATEK_PARTICLE_SPAWN_FIELDS);

    #define IDEATEK_PARTICLE_LIGHT_FIELDS { \
        float intensity; \
        float range_scale; \
        float fraction; \
        uint32_t max; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_light_t,
    IDEATEK_PARTICLE_LIGHT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_light_t,
    IDEATEK_PARTICLE_LIGHT_FIELDS);

#endif
