/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "gltf_inside.h"

static void read_file(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b)
{
    gltf_file_t *f = find_file(id);
    cgltf_data *data = nullptr;
    cgltf_result r;
    char line[96];

    f = f ? f : add_file(id);
    if (!f) {
        refuse_id(api, id, " is refused: the table of the files is full");
        return;
    }
    r = parse_file(api->shared_base + b->data.at, b->data.len, &data);
    snprintf(line, sizeof(line), " is not a glTF file: %s",
        name_of_result(r));
    if (r != cgltf_result_success)
        refuse_file(api, f, line);
    else if (!plan_deps(api, f, data))
        refuse_file(api, f, ": no room for its dependencies");
    else
        wait_for(api, f);
}

static void hear_bytes(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b)
{
    if (start_module(api)) {
        read_file(api, id, b);
        return;
    }
    refuse_id(api, id, " is refused, because the loader has wrong settings");
}

BRIDGE_NATIVE(bytes)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_BYTES, &count);
    uint32_t at;
    const ideatek_resource_bytes_t *b;

    for (uint32_t i = 0; i < count; i++) {
        at = bridge_native_find_value(api, t[i].id, BYTES);
        b = (const void *)(api->shared_base + at);
        if (at && b->loader.id == gltf_module.loader)
            hear_bytes(api, t[i].id, b);
    }
    return (0);
}

BRIDGE_NATIVE(swap)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_SWAP, &count);
    gltf_file_t *f;

    for (uint32_t i = 0; i < count; i++) {
        f = find_file(t[i].id);
        if (f)
            free_key(&f->old);
    }
    return (0);
}

BRIDGE_NATIVE(gone)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_GONE, &count);
    gltf_file_t *f;

    for (uint32_t i = 0; i < count; i++) {
        f = find_file(t[i].id);
        if (f)
            forget_file(f);
    }
    free_caches();
    return (0);
}
