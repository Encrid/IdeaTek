/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WINDOW_CLIPBOARD_H
    #define BC_IDEATEK_WINDOW_CLIPBOARD_H

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_CLIPBOARD_REQUEST_FIELDS { \
        bridge_text_t text; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clipboard_request_t,
    IDEATEK_CLIPBOARD_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clipboard_request_t,
    IDEATEK_CLIPBOARD_REQUEST_FIELDS);

    #define IDEATEK_PASTE_FIELDS { \
        bridge_text_t text; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_paste_t, IDEATEK_PASTE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_paste_t, IDEATEK_PASTE_FIELDS);

#endif
