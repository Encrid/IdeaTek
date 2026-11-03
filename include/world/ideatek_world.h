/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WORLD_H
    #define BC_IDEATEK_WORLD_H

    #include <stdint.h>

    #include "bridge_schema.h"

    #define IDEATEK_ENTITY_FIELDS { \
        uint64_t id; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_entity_t, IDEATEK_ENTITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_entity_t, IDEATEK_ENTITY_FIELDS);

#endif
