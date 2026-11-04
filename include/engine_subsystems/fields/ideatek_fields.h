/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_FIELDS_H
    #define BC_IDEATEK_FIELDS_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_space.h"

    #define IDEATEK_FIELD_SHAPE_VALUES { \
        IDEATEK_FIELD_SHAPE_INFINITE = 0, \
        IDEATEK_FIELD_SHAPE_SPHERE = 1, \
        IDEATEK_FIELD_SHAPE_BOX = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_field_shape_t,
    uint8_t, IDEATEK_FIELD_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_field_shape_t,
    uint8_t, IDEATEK_FIELD_SHAPE_VALUES);

    #define IDEATEK_FORCE_KIND_VALUES { \
        IDEATEK_FORCE_KIND_ATTRACTOR = 0, \
        IDEATEK_FORCE_KIND_VORTEX = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_force_kind_t,
    uint8_t, IDEATEK_FORCE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_force_kind_t,
    uint8_t, IDEATEK_FORCE_KIND_VALUES);

    #define IDEATEK_WIND_FIELDS { \
        ideatek_vec3_t size; \
        float speed; \
        float falloff; \
        ideatek_field_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wind_t, IDEATEK_WIND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wind_t, IDEATEK_WIND_FIELDS);

    #define IDEATEK_FORCE_FIELD_FIELDS { \
        ideatek_vec3_t size; \
        float acceleration; \
        float falloff; \
        ideatek_force_kind_t kind; \
        ideatek_field_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_force_field_t, IDEATEK_FORCE_FIELD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_force_field_t, IDEATEK_FORCE_FIELD_FIELDS);

#endif
