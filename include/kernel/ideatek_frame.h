/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_FRAME_H
    #define BC_IDEATEK_FRAME_H

    #include <stdint.h>

    #include "bridge_schema.h"

    #define IDEATEK_FRAME_FIELDS { \
        double delta; \
        double elapsed; \
        double real_delta; \
        double real_elapsed; \
        double scale; \
        uint64_t index; \
        uint8_t paused; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_frame_t, IDEATEK_FRAME_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_frame_t, IDEATEK_FRAME_FIELDS);

    #define IDEATEK_TICK_FIELDS { \
        double step; \
        double elapsed; \
        uint64_t tick; \
        uint32_t steps; \
        float fraction; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_tick_t, IDEATEK_TICK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_tick_t, IDEATEK_TICK_FIELDS);

    #define IDEATEK_TIME_REQUEST_FIELDS { \
        double scale; \
        uint32_t rate; \
        uint32_t max_rate; \
        uint8_t paused; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_time_request_t,
    IDEATEK_TIME_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_time_request_t,
    IDEATEK_TIME_REQUEST_FIELDS);

#endif
