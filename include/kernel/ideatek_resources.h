/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_RESOURCES_H
    #define BC_IDEATEK_RESOURCES_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_LOAD_STATE_VALUES { \
        IDEATEK_LOAD_STATE_NOT_LOADED = 0, \
        IDEATEK_LOAD_STATE_LOADING = 1, \
        IDEATEK_LOAD_STATE_LOADED = 2, \
        IDEATEK_LOAD_STATE_FAILED = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_load_state_t,
    uint8_t, IDEATEK_LOAD_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_load_state_t,
    uint8_t, IDEATEK_LOAD_STATE_VALUES);

    #define IDEATEK_RESOURCE_ERROR_VALUES { \
        IDEATEK_RESOURCE_ERROR_NONE = 0, \
        IDEATEK_RESOURCE_ERROR_NO_LOADER = 1, \
        IDEATEK_RESOURCE_ERROR_TWO_LOADERS = 2, \
        IDEATEK_RESOURCE_ERROR_NOT_FOUND = 3, \
        IDEATEK_RESOURCE_ERROR_READ_FAILED = 4, \
        IDEATEK_RESOURCE_ERROR_LOADER_FAILED = 5, \
        IDEATEK_RESOURCE_ERROR_NO_LABEL = 6, \
        IDEATEK_RESOURCE_ERROR_WRONG_TYPE = 7, \
        IDEATEK_RESOURCE_ERROR_IGNORED = 8, \
        IDEATEK_RESOURCE_ERROR_TOO_BIG = 9, \
        IDEATEK_RESOURCE_ERROR_NO_ROOM = 10 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_resource_error_t,
    uint8_t, IDEATEK_RESOURCE_ERROR_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_resource_error_t,
    uint8_t, IDEATEK_RESOURCE_ERROR_VALUES);

    #define IDEATEK_RESOURCE_LOAD_FIELDS { \
        ideatek_entity_t owner; \
        bridge_text_t path; \
        bridge_text_t type; \
        bridge_text_t settings; \
        uint8_t reload_owner; \
        uint8_t priority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_load_t,
    IDEATEK_RESOURCE_LOAD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_load_t,
    IDEATEK_RESOURCE_LOAD_FIELDS);

    #define IDEATEK_RESOURCE_ADD_FIELDS { \
        ideatek_entity_t resource; \
        bridge_text_t type; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_add_t, IDEATEK_RESOURCE_ADD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_add_t, IDEATEK_RESOURCE_ADD_FIELDS);

    #define IDEATEK_RESOURCE_HANDLE_FIELDS { \
        ideatek_entity_t resource; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_handle_t,
    IDEATEK_RESOURCE_HANDLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_handle_t,
    IDEATEK_RESOURCE_HANDLE_FIELDS);

    #define IDEATEK_RESOURCE_FIELDS { \
        bridge_text_t path; \
        bridge_text_t type; \
        ideatek_load_state_t state; \
        ideatek_load_state_t dependencies; \
        ideatek_load_state_t recursive; \
        ideatek_resource_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_t, IDEATEK_RESOURCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_t, IDEATEK_RESOURCE_FIELDS);

    #define IDEATEK_RESOURCE_LOADER_FIELDS { \
        bridge_text_t name; \
        bridge_text_t type; \
        bridge_text_t extensions; \
        bridge_text_t magic; \
        bool ranged; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_loader_t,
    IDEATEK_RESOURCE_LOADER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_loader_t,
    IDEATEK_RESOURCE_LOADER_FIELDS);

    #define IDEATEK_RESOURCE_BYTES_FIELDS { \
        ideatek_entity_t loader; \
        uint64_t offset; \
        uint64_t size; \
        bridge_text_t data; \
        bridge_text_t settings; \
        ideatek_resource_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_bytes_t,
    IDEATEK_RESOURCE_BYTES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_bytes_t,
    IDEATEK_RESOURCE_BYTES_FIELDS);

    #define IDEATEK_RESOURCE_READ_FIELDS { \
        uint64_t offset; \
        uint64_t length; \
        uint8_t priority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_read_t,
    IDEATEK_RESOURCE_READ_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_read_t,
    IDEATEK_RESOURCE_READ_FIELDS);

    #define IDEATEK_RESOURCE_OUTCOME_FIELDS { \
        ideatek_resource_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_outcome_t,
    IDEATEK_RESOURCE_OUTCOME_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_outcome_t,
    IDEATEK_RESOURCE_OUTCOME_FIELDS);

    #define IDEATEK_RESOURCE_LABEL_FIELDS { \
        ideatek_entity_t resource; \
        bridge_text_t label; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_label_t,
    IDEATEK_RESOURCE_LABEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_label_t,
    IDEATEK_RESOURCE_LABEL_FIELDS);

    #define IDEATEK_RAW_BYTES_FIELDS { \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_raw_bytes_t, IDEATEK_RAW_BYTES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_raw_bytes_t, IDEATEK_RAW_BYTES_FIELDS);

    #define IDEATEK_RESOURCE_SOURCE_FIELDS { \
        bridge_text_t name; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_source_t,
    IDEATEK_RESOURCE_SOURCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_source_t,
    IDEATEK_RESOURCE_SOURCE_FIELDS);

    #define IDEATEK_RESOURCE_SOURCE_READ_FIELDS { \
        ideatek_entity_t source; \
        uint64_t offset; \
        uint64_t length; \
        bridge_text_t path; \
        uint8_t priority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_source_read_t,
    IDEATEK_RESOURCE_SOURCE_READ_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_source_read_t,
    IDEATEK_RESOURCE_SOURCE_READ_FIELDS);

    #define IDEATEK_RESOURCE_SOURCE_RESULT_FIELDS { \
        uint64_t size; \
        bridge_text_t data; \
        ideatek_resource_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_source_result_t,
    IDEATEK_RESOURCE_SOURCE_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_source_result_t,
    IDEATEK_RESOURCE_SOURCE_RESULT_FIELDS);

    #define IDEATEK_RESOURCE_VARIABLE_FIELDS { \
        bridge_text_t name; \
        bridge_text_t value; \
        bridge_text_t fallback; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_resource_variable_t,
    IDEATEK_RESOURCE_VARIABLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_resource_variable_t,
    IDEATEK_RESOURCE_VARIABLE_FIELDS);

#endif
