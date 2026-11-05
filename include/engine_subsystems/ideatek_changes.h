/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_CHANGES_H
    #define BC_IDEATEK_CHANGES_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_ENTITY_ID_FIELDS { \
        uint64_t id; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_entity_id_t, IDEATEK_ENTITY_ID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_entity_id_t, IDEATEK_ENTITY_ID_FIELDS);

    #define IDEATEK_CHANGE_KIND_VALUES { \
        IDEATEK_CHANGE_KIND_STATE = 0, \
        IDEATEK_CHANGE_KIND_EVENT = 1, \
        IDEATEK_CHANGE_KIND_REMOVE = 2, \
        IDEATEK_CHANGE_KIND_FULL = 3, \
        IDEATEK_CHANGE_KIND_PREDICTED = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_change_kind_t,
    uint8_t, IDEATEK_CHANGE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_change_kind_t,
    uint8_t, IDEATEK_CHANGE_KIND_VALUES);

    #define IDEATEK_DURABILITY_VALUES { \
        IDEATEK_DURABILITY_IMMEDIATE = 0, \
        IDEATEK_DURABILITY_PERIODIC = 1, \
        IDEATEK_DURABILITY_NONE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_durability_t,
    uint8_t, IDEATEK_DURABILITY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_durability_t,
    uint8_t, IDEATEK_DURABILITY_VALUES);

    #define IDEATEK_CHANGE_CONDITION_VALUES { \
        IDEATEK_CHANGE_CONDITION_ALL = 0, \
        IDEATEK_CHANGE_CONDITION_OWNER_ONLY = 1, \
        IDEATEK_CHANGE_CONDITION_SKIP_OWNER = 2, \
        IDEATEK_CHANGE_CONDITION_INITIAL_ONLY = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_change_condition_t,
    uint8_t, IDEATEK_CHANGE_CONDITION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_change_condition_t,
    uint8_t, IDEATEK_CHANGE_CONDITION_VALUES);

    #define IDEATEK_CHANGE_TAG_FIELDS { \
        uint64_t tag; \
        float period; \
        uint32_t version; \
        ideatek_durability_t durability; \
        ideatek_change_condition_t condition; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_change_tag_t, IDEATEK_CHANGE_TAG_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_change_tag_t, IDEATEK_CHANGE_TAG_FIELDS);

    #define IDEATEK_ROW_CHANGES_FIELDS { \
        uint64_t tag; \
        bridge_text_t data; \
        ideatek_entity_t request; \
        uint32_t peer; \
        ideatek_change_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_row_changes_t, IDEATEK_ROW_CHANGES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_row_changes_t, IDEATEK_ROW_CHANGES_FIELDS);

    #define IDEATEK_ROWS_IN_FIELDS { \
        uint64_t tag; \
        bridge_text_t data; \
        ideatek_entity_t source; \
        uint32_t peer; \
        uint32_t version; \
        ideatek_change_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_rows_in_t, IDEATEK_ROWS_IN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_rows_in_t, IDEATEK_ROWS_IN_FIELDS);

    #define IDEATEK_STORE_STATE_FIELDS { \
        uint64_t round; \
        uint64_t durable; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_store_state_t, IDEATEK_STORE_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_store_state_t, IDEATEK_STORE_STATE_FIELDS);

    #define IDEATEK_RECORDING_ACTION_VALUES { \
        IDEATEK_RECORDING_ACTION_RECORD = 0, \
        IDEATEK_RECORDING_ACTION_PLAY = 1, \
        IDEATEK_RECORDING_ACTION_STOP = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_recording_action_t,
    uint8_t, IDEATEK_RECORDING_ACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_recording_action_t,
    uint8_t, IDEATEK_RECORDING_ACTION_VALUES);

    #define IDEATEK_RECORDING_FIELDS { \
        bridge_text_t file; \
        ideatek_recording_action_t action; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_recording_t, IDEATEK_RECORDING_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_recording_t, IDEATEK_RECORDING_FIELDS);

    #define IDEATEK_RECORDING_STATUS_VALUES { \
        IDEATEK_RECORDING_STATUS_IDLE = 0, \
        IDEATEK_RECORDING_STATUS_RECORDING = 1, \
        IDEATEK_RECORDING_STATUS_PLAYING = 2, \
        IDEATEK_RECORDING_STATUS_FAILED = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_recording_status_t,
    uint8_t, IDEATEK_RECORDING_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_recording_status_t,
    uint8_t, IDEATEK_RECORDING_STATUS_VALUES);

    #define IDEATEK_RECORDING_STATE_FIELDS { \
        uint64_t round; \
        ideatek_recording_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_recording_state_t,
    IDEATEK_RECORDING_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_recording_state_t,
    IDEATEK_RECORDING_STATE_FIELDS);

#endif
