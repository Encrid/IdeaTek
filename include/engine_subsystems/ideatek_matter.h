/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_MATTER_H
    #define BC_IDEATEK_MATTER_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_physics.h"
    #include "ideatek_world.h"

    #define IDEATEK_COATING_RULE_VALUES { \
        IDEATEK_COATING_RULE_LARGER = 0, \
        IDEATEK_COATING_RULE_NEWER = 1, \
        IDEATEK_COATING_RULE_OLDER = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_coating_rule_t,
    uint8_t, IDEATEK_COATING_RULE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_coating_rule_t,
    uint8_t, IDEATEK_COATING_RULE_VALUES);

    #define IDEATEK_MATTER_FAMILY_FIELDS { \
        float thermal_conductivity; \
        float specific_heat; \
        float emissivity; \
        float air_rate; \
        float volume_keep; \
        float smoke_share; \
        float ash_share; \
        bool particles_burn; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_matter_family_t,
    IDEATEK_MATTER_FAMILY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_matter_family_t,
    IDEATEK_MATTER_FAMILY_FIELDS);

    #define IDEATEK_MATTER_MATERIAL_FIELDS { \
        ideatek_entity_t family; \
        ideatek_entity_t solid; \
        ideatek_entity_t soft; \
        ideatek_entity_t liquid; \
        ideatek_entity_t gas; \
        float softening_point; \
        float melting_point; \
        float boiling_point; \
        float fire_temperature; \
        float burn_rate; \
        float burn_heat; \
        float electric_conductivity; \
        float adhesion; \
        bool flammable; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_matter_material_t,
    IDEATEK_MATTER_MATERIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_matter_material_t,
    IDEATEK_MATTER_MATERIAL_FIELDS);

    #define IDEATEK_MATTER_CHUNK_FIELDS { \
        float temperature; \
        float coating_mass; \
        ideatek_entity_t coating; \
        ideatek_solver_t phase; \
        bool burning; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_matter_chunk_t, IDEATEK_MATTER_CHUNK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_matter_chunk_t, IDEATEK_MATTER_CHUNK_FIELDS);

    #define IDEATEK_MATTER_FIELDS { \
        ideatek_entity_t material; \
        bridge_text_t chunks; \
        uint32_t chunk_count; \
        float charge; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_matter_t, IDEATEK_MATTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_matter_t, IDEATEK_MATTER_FIELDS);

    #define IDEATEK_PARTICLE_STATE_FIELDS { \
        float temperature; \
        uint32_t material; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_state_t,
    IDEATEK_PARTICLE_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_state_t,
    IDEATEK_PARTICLE_STATE_FIELDS);

    #define IDEATEK_PARTICLE_MATTER_FIELDS { \
        bridge_text_t materials; \
        bridge_text_t states; \
        uint32_t material_count; \
        uint32_t state_count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_particle_matter_t,
    IDEATEK_PARTICLE_MATTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_particle_matter_t,
    IDEATEK_PARTICLE_MATTER_FIELDS);

#endif
