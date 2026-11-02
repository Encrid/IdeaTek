/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_FRAME_H
    #define BC_IDEATEK_FRAME_H

    #include <stdint.h>

    #include "bridge_schema.h"

    #define IDEATEK_FRAME_FIELDS { double delta; double elapsed; \
        double real_delta; uint64_t index; }
    #define IDEATEK_TICK_FIELDS { double step; uint64_t tick; \
        uint32_t steps; float fraction; }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_frame_t, IDEATEK_FRAME_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_frame_t, IDEATEK_FRAME_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_tick_t, IDEATEK_TICK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_tick_t, IDEATEK_TICK_FIELDS);

#endif
