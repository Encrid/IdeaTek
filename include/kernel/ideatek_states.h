/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_STATES_H
    #define BC_IDEATEK_STATES_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_STATE_FIELDS { \
        uint32_t value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_state_t, IDEATEK_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_state_t, IDEATEK_STATE_FIELDS);

    #define IDEATEK_STATE_REQUEST_FIELDS { \
        uint32_t value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_state_request_t,
    IDEATEK_STATE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_state_request_t,
    IDEATEK_STATE_REQUEST_FIELDS);

    #define IDEATEK_STATE_CHANGE_FIELDS { \
        uint32_t from; \
        uint32_t to; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_state_change_t, IDEATEK_STATE_CHANGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_state_change_t, IDEATEK_STATE_CHANGE_FIELDS);

    #define IDEATEK_STATE_SCOPED_FIELDS { \
        ideatek_entity_t machine; \
        uint32_t value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_state_scoped_t, IDEATEK_STATE_SCOPED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_state_scoped_t, IDEATEK_STATE_SCOPED_FIELDS);

#endif
