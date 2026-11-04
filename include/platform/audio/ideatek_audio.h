/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_AUDIO_H
    #define BC_IDEATEK_AUDIO_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_AUDIO_DIRECTION_VALUES { \
        IDEATEK_AUDIO_DIRECTION_PLAYBACK = 0, \
        IDEATEK_AUDIO_DIRECTION_RECORDING = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_audio_direction_t, uint8_t,
    IDEATEK_AUDIO_DIRECTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_audio_direction_t, uint8_t,
    IDEATEK_AUDIO_DIRECTION_VALUES);

    #define IDEATEK_AUDIO_TARGET_VALUES { \
        IDEATEK_AUDIO_TARGET_DEFAULT = 0, \
        IDEATEK_AUDIO_TARGET_CHOSEN = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_audio_target_t, uint8_t,
    IDEATEK_AUDIO_TARGET_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_audio_target_t, uint8_t,
    IDEATEK_AUDIO_TARGET_VALUES);

    #define IDEATEK_AUDIO_STATUS_VALUES { \
        IDEATEK_AUDIO_STATUS_RUNNING = 0, \
        IDEATEK_AUDIO_STATUS_FELL_BACK = 1, \
        IDEATEK_AUDIO_STATUS_NO_DEVICE = 2, \
        IDEATEK_AUDIO_STATUS_SUSPENDED = 3, \
        IDEATEK_AUDIO_STATUS_PENDING = 4, \
        IDEATEK_AUDIO_STATUS_DENIED = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_audio_status_t, uint8_t,
    IDEATEK_AUDIO_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_audio_status_t, uint8_t,
    IDEATEK_AUDIO_STATUS_VALUES);

    #define IDEATEK_AUDIO_DEVICE_FIELDS { \
        char name[128]; \
        ideatek_audio_direction_t direction; \
        bool is_default; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_audio_device_t, IDEATEK_AUDIO_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_audio_device_t, IDEATEK_AUDIO_DEVICE_FIELDS);

    #define IDEATEK_AUDIO_REQUEST_FIELDS { \
        ideatek_entity_t device; \
        uint32_t rate; \
        uint32_t ahead; \
        ideatek_audio_direction_t direction; \
        ideatek_audio_target_t target; \
        uint8_t channels; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_audio_request_t,
    IDEATEK_AUDIO_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_audio_request_t,
    IDEATEK_AUDIO_REQUEST_FIELDS);

    #define IDEATEK_AUDIO_FRAMES_FIELDS { \
        uint64_t at; \
        uint32_t frames; \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_audio_frames_t, IDEATEK_AUDIO_FRAMES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_audio_frames_t, IDEATEK_AUDIO_FRAMES_FIELDS);

    #define IDEATEK_AUDIO_STATE_FIELDS { \
        ideatek_entity_t device; \
        uint64_t dropped; \
        uint32_t rate; \
        uint32_t queued; \
        uint32_t wanted; \
        uint8_t channels; \
        ideatek_audio_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_audio_state_t, IDEATEK_AUDIO_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_audio_state_t, IDEATEK_AUDIO_STATE_FIELDS);

#endif
