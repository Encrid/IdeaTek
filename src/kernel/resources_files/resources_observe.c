/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const uint32_t CHANGES[] = { FILE_REQUEST, HANDLE, RESOURCE, BYTES,
    OUTCOME, READ, RAW, SOURCE_READ, RELOAD, MODIFIED, READY, FAILED };

static void add_observer(const bridge_api_t *api, uint32_t list,
    uint32_t slot, uint32_t events)
{
    const char *const names[RES_LISTS] = { "loads", "adds", "loaders",
        "sources", "variables", "outcomes", "labels", "ranges", "reloads",
        "results", "changes", "answers" };
    bridge_term_t term[1] = {{ slot, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_observer_statement_t statement = {
        .name = bridge_native_put(api, names[list],
            (uint32_t)strlen(names[list])),
        .name_len = (uint32_t)strlen(names[list]),
        .events = events,
        .terms = bridge_native_put(api, term, sizeof(term)),
        .term_count = 1,
    };

    res_module.lists[list] = api->observe(api, bridge_native_put(api,
        &statement, sizeof(statement)));
}

static void add_use_observers(const bridge_api_t *api)
{
    uint32_t both = BRIDGE_ON_ENTER | BRIDGE_ON_LEAVE;

    add_observer(api, LIST_LOADS, LOAD, both);
    add_observer(api, LIST_ADDS, ADD, both);
    add_observer(api, LIST_LOADERS, LOADER, both);
    add_observer(api, LIST_SOURCES, SOURCE, both);
}

static void add_work_observers(const bridge_api_t *api)
{
    uint32_t all = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH | BRIDGE_ON_LEAVE;
    uint32_t news = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    add_observer(api, LIST_VARIABLES, VARIABLE, all);
    add_observer(api, LIST_OUTCOMES, OUTCOME, news);
    add_observer(api, LIST_LABELS, LABEL, news);
    add_observer(api, LIST_RANGES, READ, news);
}

static void add_file_observers(const bridge_api_t *api)
{
    uint32_t news = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    add_observer(api, LIST_RELOADS, RELOAD, BRIDGE_ON_ENTER);
    add_observer(api, LIST_RESULTS, FILE_RESULT, news);
    add_observer(api, LIST_CHANGES, FILE_CHANGE, news);
    add_observer(api, LIST_ANSWERS, SOURCE_RESULT, news);
}

void add_runs(const bridge_api_t *api)
{
    uint32_t first = BRIDGE_NATIVE_ADD_SYSTEM(api, "round", "first", nullptr,
        0, CHANGES, sizeof(CHANGES) / sizeof(CHANGES[0]), nullptr, 0);
    bridge_term_t term[1] = {{ LOADER, BRIDGE_READ, BRIDGE_AND, 0 }};

    res_module.found = BRIDGE_NATIVE_ADD_SYSTEM(api, "found", "first", term,
        1, nullptr, 0, &first, 1);
    add_use_observers(api);
    add_work_observers(api);
    add_file_observers(api);
}
