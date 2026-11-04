/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SUBTITLES_H
    #define BC_IDEATEK_SUBTITLES_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_CAPTION_KIND_VALUES { \
        IDEATEK_CAPTION_KIND_SPEECH = 0, \
        IDEATEK_CAPTION_KIND_EFFECT = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_caption_kind_t,
    uint8_t, IDEATEK_CAPTION_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_caption_kind_t,
    uint8_t, IDEATEK_CAPTION_KIND_VALUES);

    #define IDEATEK_SOUND_CAPTION_FIELDS { \
        ideatek_entity_t sound; \
        bridge_text_t speaker; \
        bridge_text_t text; \
        float time; \
        float duration; \
        int32_t priority; \
        ideatek_caption_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_caption_t,
    IDEATEK_SOUND_CAPTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_caption_t,
    IDEATEK_SOUND_CAPTION_FIELDS);

    #define IDEATEK_SUBTITLE_LINE_FIELDS { \
        ideatek_entity_t caption; \
        ideatek_entity_t player; \
        float start; \
        float end; \
        ideatek_caption_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_subtitle_line_t,
    IDEATEK_SUBTITLE_LINE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_subtitle_line_t,
    IDEATEK_SUBTITLE_LINE_FIELDS);

    #define IDEATEK_SUBTITLE_FLAGS_VALUES { \
        IDEATEK_SUBTITLE_FLAGS_SPEECH = 0x01, \
        IDEATEK_SUBTITLE_FLAGS_EFFECTS = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_subtitle_flags_t,
    uint8_t, IDEATEK_SUBTITLE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_subtitle_flags_t,
    uint8_t, IDEATEK_SUBTITLE_FLAGS_VALUES);

    #define IDEATEK_SUBTITLE_SETTINGS_FIELDS { \
        ideatek_subtitle_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_subtitle_settings_t,
    IDEATEK_SUBTITLE_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_subtitle_settings_t,
    IDEATEK_SUBTITLE_SETTINGS_FIELDS);

#endif
