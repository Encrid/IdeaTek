/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "gltf_inside.h"

const char *path_of(const bridge_api_t *api, uint64_t id, uint32_t *len)
{
    uint32_t at = bridge_native_find_value(api, id, RESOURCE);
    const ideatek_resource_t *r = (const void *)(api->shared_base + at);

    *len = at ? r->path.len : 0;
    return (at ? (const char *)api->shared_base + r->path.at : "");
}

void say_path(const bridge_api_t *api, uint64_t id, const char *rest)
{
    uint32_t len = 0;
    const char *path = path_of(api, id, &len);

    bridge_native_say(api, BRIDGE_LOG_WARN, "%.*s%s", (int)len, path, rest);
}

void say_index(const bridge_api_t *api, uint64_t id, const char *what,
    size_t index)
{
    char line[128];

    snprintf(line, sizeof(line), what, index);
    say_path(api, id, line);
}

void refuse_id(const bridge_api_t *api, uint64_t id, const char *rest)
{
    say_path(api, id, rest);
    give_outcome(api, id, false);
}

void give_outcome(const bridge_api_t *api, uint64_t id, bool ok)
{
    ideatek_resource_outcome_t o = { .error = ok ? IDEATEK_RESOURCE_ERROR_NONE
        : IDEATEK_RESOURCE_ERROR_LOADER_FAILED };

    BRIDGE_NATIVE_ATTACH(api, id, OUTCOME, o);
}
