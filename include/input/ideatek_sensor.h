/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SENSOR_H
    #define BC_IDEATEK_SENSOR_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_SENSOR_TYPE_VALUES { \
        IDEATEK_SENSOR_TYPE_UNKNOWN = 0, \
        IDEATEK_SENSOR_TYPE_ACCEL = 1, \
        IDEATEK_SENSOR_TYPE_GYRO = 2, \
        IDEATEK_SENSOR_TYPE_ACCEL_L = 3, \
        IDEATEK_SENSOR_TYPE_GYRO_L = 4, \
        IDEATEK_SENSOR_TYPE_ACCEL_R = 5, \
        IDEATEK_SENSOR_TYPE_GYRO_R = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_sensor_type_t, uint8_t,
    IDEATEK_SENSOR_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_sensor_type_t, uint8_t,
    IDEATEK_SENSOR_TYPE_VALUES);

    #define IDEATEK_SENSOR_FIELDS { \
        ideatek_entity_t gamepad; \
        int32_t raw_type; \
        float rate; \
        ideatek_sensor_type_t type; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sensor_t, IDEATEK_SENSOR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sensor_t, IDEATEK_SENSOR_FIELDS);

    #define IDEATEK_SENSOR_REQUEST_FIELDS { \
        bool enabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sensor_request_t,
    IDEATEK_SENSOR_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sensor_request_t,
    IDEATEK_SENSOR_REQUEST_FIELDS);

    #define IDEATEK_SENSOR_STATE_FIELDS { \
        uint64_t time; \
        float data[6]; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sensor_state_t, IDEATEK_SENSOR_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sensor_state_t, IDEATEK_SENSOR_STATE_FIELDS);

    #define IDEATEK_SENSOR_EVENT_FIELDS { \
        uint64_t time; \
        ideatek_entity_t sensor; \
        float data[6]; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sensor_event_t, IDEATEK_SENSOR_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sensor_event_t, IDEATEK_SENSOR_EVENT_FIELDS);

#endif
