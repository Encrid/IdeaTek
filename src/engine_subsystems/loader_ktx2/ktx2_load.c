/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "ktx2_inside.h"

static void refuse(const bridge_api_t *api, uint64_t id, const char *rest)
{
    say_path(api, id, rest);
    give_outcome(api, id, false);
}

static bool is_ready(const bridge_api_t *api, uint64_t id)
{
    if (!ktx2_module.started)
        start_module(api);
    if (!ktx2_module.broken)
        return (true);
    refuse(api, id, " is refused: the settings of the module are wrong");
    return (false);
}

void load_one(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b)
{
    ktx2_bytes_t file = { api->shared_base + b->data.at, b->data.len };
    ktx2_file_t f = {};
    const char *reason;
    char line[128];
    bool ok;

    if (!is_ready(api, id))
        return;
    reason = ktx2_read(file, &f);
    if (reason) {
        snprintf(line, sizeof(line), " is not a KTX 2.0 file: %s", reason);
        refuse(api, id, line);
        return;
    }
    ok = give_texture(api, id, &f, file);
    if (ok)
        give_sampler(api, id, b);
    give_outcome(api, id, ok);
}
