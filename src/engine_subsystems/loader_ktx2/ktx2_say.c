/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_inside.h"

void say_path(const bridge_api_t *api, uint64_t id, const char *rest)
{
    uint32_t at = bridge_native_find_value(api, id, RESOURCE);
    const ideatek_resource_t *r = (const void *)(api->shared_base + at);
    const char *path = at ? (const char *)api->shared_base + r->path.at : "";
    uint32_t len = at ? r->path.len : 0;

    bridge_native_say(api, BRIDGE_LOG_WARN, "%.*s%s", (int)len, path, rest);
}

void give_outcome(const bridge_api_t *api, uint64_t id, bool ok)
{
    ideatek_resource_outcome_t o = { .error = ok ? IDEATEK_RESOURCE_ERROR_NONE
        : IDEATEK_RESOURCE_ERROR_LOADER_FAILED };

    BRIDGE_NATIVE_ATTACH(api, id, OUTCOME, o);
}
