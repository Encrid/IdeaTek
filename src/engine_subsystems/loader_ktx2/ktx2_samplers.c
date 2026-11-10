/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "ktx2_inside.h"

static void say_fault(const bridge_api_t *api, uint64_t id, ktx2_bytes_t text,
    const ktx2_fault_t *fault)
{
    const char *key = (const char *)text.at + fault->at;
    int len = (int)fault->len;
    char line[128];

    if (fault->kind == KTX2_UNKNOWN_KEY)
        snprintf(line, sizeof(line), ": the key %.*s of the settings is not "
            "known", len, key);
    else
        snprintf(line, sizeof(line), ": the value of %.*s is not allowed",
            len, key);
    say_path(api, id, line);
}

void give_sampler(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b)
{
    ktx2_bytes_t text = { api->shared_base + b->settings.at,
        b->settings.len };
    ktx2_sampler_read_t r;

    ktx2_read_sampler(text, &r);
    for (uint32_t i = 0; i < r.count; i++)
        say_fault(api, id, text, &r.faults[i]);
    BRIDGE_NATIVE_ATTACH(api, id, SAMPLER, r.sampler);
}
