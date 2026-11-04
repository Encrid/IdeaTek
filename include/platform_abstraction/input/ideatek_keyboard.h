/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_KEYBOARD_H
    #define BC_IDEATEK_KEYBOARD_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_MODIFIER_VALUES { \
        IDEATEK_MODIFIER_LSHIFT = 0x0001, \
        IDEATEK_MODIFIER_RSHIFT = 0x0002, \
        IDEATEK_MODIFIER_LEVEL5 = 0x0004, \
        IDEATEK_MODIFIER_LCTRL = 0x0040, \
        IDEATEK_MODIFIER_RCTRL = 0x0080, \
        IDEATEK_MODIFIER_LALT = 0x0100, \
        IDEATEK_MODIFIER_RALT = 0x0200, \
        IDEATEK_MODIFIER_LGUI = 0x0400, \
        IDEATEK_MODIFIER_RGUI = 0x0800, \
        IDEATEK_MODIFIER_NUM = 0x1000, \
        IDEATEK_MODIFIER_CAPS = 0x2000, \
        IDEATEK_MODIFIER_MODE = 0x4000, \
        IDEATEK_MODIFIER_SCROLL = 0x8000 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_modifier_t, uint16_t,
    IDEATEK_MODIFIER_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_modifier_t, uint16_t,
    IDEATEK_MODIFIER_VALUES);

    #define IDEATEK_KEYBOARD_FIELDS { \
        uint64_t keys[4]; \
        ideatek_modifier_t modifiers; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_keyboard_t, IDEATEK_KEYBOARD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_keyboard_t, IDEATEK_KEYBOARD_FIELDS);

    #define IDEATEK_KEY_EVENT_FIELDS { \
        uint32_t character; \
        uint32_t key; \
        uint32_t code; \
        uint32_t order; \
        ideatek_modifier_t modifiers; \
        bool down; \
        bool repeat; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_key_event_t, IDEATEK_KEY_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_key_event_t, IDEATEK_KEY_EVENT_FIELDS);

#endif
