/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_FILES_H
    #define BC_IDEATEK_FILES_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_STORAGE_VALUES { \
        IDEATEK_STORAGE_CONTENT = 0, \
        IDEATEK_STORAGE_USER = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_storage_t, uint8_t, IDEATEK_STORAGE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_storage_t, uint8_t, IDEATEK_STORAGE_VALUES);

    #define IDEATEK_FILE_ACTION_VALUES { \
        IDEATEK_FILE_ACTION_READ = 0, \
        IDEATEK_FILE_ACTION_WRITE = 1, \
        IDEATEK_FILE_ACTION_LIST = 2, \
        IDEATEK_FILE_ACTION_REMOVE = 3, \
        IDEATEK_FILE_ACTION_WATCH = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_file_action_t, uint8_t,
    IDEATEK_FILE_ACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_file_action_t, uint8_t,
    IDEATEK_FILE_ACTION_VALUES);

    #define IDEATEK_FILE_STATUS_VALUES { \
        IDEATEK_FILE_STATUS_OK = 0, \
        IDEATEK_FILE_STATUS_NOT_FOUND = 1, \
        IDEATEK_FILE_STATUS_TOO_BIG = 2, \
        IDEATEK_FILE_STATUS_DENIED = 3, \
        IDEATEK_FILE_STATUS_FAILED = 4, \
        IDEATEK_FILE_STATUS_UNSUPPORTED = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_file_status_t, uint8_t,
    IDEATEK_FILE_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_file_status_t, uint8_t,
    IDEATEK_FILE_STATUS_VALUES);

    #define IDEATEK_FILE_REQUEST_FIELDS { \
        uint64_t offset; \
        uint64_t length; \
        bridge_text_t path; \
        bridge_text_t data; \
        ideatek_storage_t storage; \
        ideatek_file_action_t action; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_file_request_t,
    IDEATEK_FILE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_file_request_t,
    IDEATEK_FILE_REQUEST_FIELDS);

    #define IDEATEK_FILE_RESULT_FIELDS { \
        uint64_t size; \
        bridge_text_t data; \
        ideatek_file_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_file_result_t, IDEATEK_FILE_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_file_result_t, IDEATEK_FILE_RESULT_FIELDS);

    #define IDEATEK_FILE_CHANGE_FIELDS { \
        bridge_text_t paths; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_file_change_t, IDEATEK_FILE_CHANGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_file_change_t, IDEATEK_FILE_CHANGE_FIELDS);

#endif
