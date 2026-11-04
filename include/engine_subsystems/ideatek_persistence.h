/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_PERSISTENCE_H
    #define BC_IDEATEK_PERSISTENCE_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_SAVE_ACTION_VALUES { \
        IDEATEK_SAVE_ACTION_SAVE = 0, \
        IDEATEK_SAVE_ACTION_LOAD = 1, \
        IDEATEK_SAVE_ACTION_INFO = 2, \
        IDEATEK_SAVE_ACTION_LIST = 3, \
        IDEATEK_SAVE_ACTION_REMOVE = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_save_action_t,
    uint8_t, IDEATEK_SAVE_ACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_save_action_t,
    uint8_t, IDEATEK_SAVE_ACTION_VALUES);

    #define IDEATEK_SAVE_REQUEST_FIELDS { \
        uint64_t groups; \
        bridge_text_t slot; \
        bridge_text_t info; \
        ideatek_save_action_t action; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_request_t, IDEATEK_SAVE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_request_t, IDEATEK_SAVE_REQUEST_FIELDS);

    #define IDEATEK_SAVED_FIELDS { \
        uint64_t prefab; \
        uint64_t groups; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_saved_t, IDEATEK_SAVED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_saved_t, IDEATEK_SAVED_FIELDS);

    #define IDEATEK_SAVE_ID_FIELDS { \
        uint64_t id; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_id_t, IDEATEK_SAVE_ID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_id_t, IDEATEK_SAVE_ID_FIELDS);

    #define IDEATEK_SAVE_PARTICIPANT_FIELDS { \
        uint64_t tag; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_participant_t,
    IDEATEK_SAVE_PARTICIPANT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_participant_t,
    IDEATEK_SAVE_PARTICIPANT_FIELDS);

    #define IDEATEK_SAVE_CHUNK_FIELDS { \
        ideatek_entity_t request; \
        uint64_t tag; \
        bridge_text_t data; \
        uint32_t version; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_chunk_t, IDEATEK_SAVE_CHUNK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_chunk_t, IDEATEK_SAVE_CHUNK_FIELDS);

    #define IDEATEK_SAVE_READY_FIELDS { \
        ideatek_entity_t request; \
        uint64_t tag; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_ready_t, IDEATEK_SAVE_READY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_ready_t, IDEATEK_SAVE_READY_FIELDS);

    #define IDEATEK_SAVE_REFUSE_FIELDS { \
        ideatek_entity_t request; \
        uint64_t tag; \
        int32_t reason; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_refuse_t, IDEATEK_SAVE_REFUSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_refuse_t, IDEATEK_SAVE_REFUSE_FIELDS);

    #define IDEATEK_SAVE_STATE_VALUES { \
        IDEATEK_SAVE_STATE_PENDING = 0, \
        IDEATEK_SAVE_STATE_DONE = 1, \
        IDEATEK_SAVE_STATE_FAILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_save_state_t,
    uint8_t, IDEATEK_SAVE_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_save_state_t,
    uint8_t, IDEATEK_SAVE_STATE_VALUES);

    #define IDEATEK_SAVE_ERROR_VALUES { \
        IDEATEK_SAVE_ERROR_NONE = 0, \
        IDEATEK_SAVE_ERROR_NOT_FOUND = 1, \
        IDEATEK_SAVE_ERROR_READ = 2, \
        IDEATEK_SAVE_ERROR_WRITE = 3, \
        IDEATEK_SAVE_ERROR_NO_SPACE = 4, \
        IDEATEK_SAVE_ERROR_FORMAT = 5, \
        IDEATEK_SAVE_ERROR_VERSION = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_save_error_t,
    uint8_t, IDEATEK_SAVE_ERROR_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_save_error_t,
    uint8_t, IDEATEK_SAVE_ERROR_VALUES);

    #define IDEATEK_SAVE_RESULT_FIELDS { \
        ideatek_save_state_t state; \
        ideatek_save_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_result_t, IDEATEK_SAVE_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_result_t, IDEATEK_SAVE_RESULT_FIELDS);

    #define IDEATEK_SAVE_INFO_FIELDS { \
        ideatek_entity_t request; \
        int64_t time; \
        uint64_t bytes; \
        uint64_t groups; \
        bridge_text_t slot; \
        bridge_text_t info; \
        uint32_t version; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_save_info_t, IDEATEK_SAVE_INFO_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_save_info_t, IDEATEK_SAVE_INFO_FIELDS);

#endif
