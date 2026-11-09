/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef STATES_INSIDE_H
    #define STATES_INSIDE_H

    #include "bridge_native.h"
    #include "ideatek_states.h"
    #include "states_chain.h"

    #define STATES_TYPE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_TABLE, \
        BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t ENTITY = 0;
constexpr uint32_t STATE = 1;
constexpr uint32_t REQUEST = 2;
constexpr uint32_t CHANGE = 3;
constexpr uint32_t SCOPED = 4;

typedef void (*states_visit_t)(const bridge_api_t *api, uint64_t id,
    uint32_t table, uint32_t row);

typedef struct {
    uint32_t forget;
    uint32_t clear;
    uint32_t apply;
} states_module_t;

extern states_module_t states_module;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);

void walk_query(const bridge_api_t *api, uint32_t query, states_visit_t visit);
const void *cell_of(const bridge_api_t *api, uint32_t table, uint32_t slot,
    uint32_t row);

#endif
