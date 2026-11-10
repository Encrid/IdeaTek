/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

void *parse_take(size_t size)
{
    uint32_t used = (gltf_module.parse_used + 15u) & ~15u;
    uint32_t room = gltf_module.s.parse_bytes - used;

    if (size > room)
        return (nullptr);
    gltf_module.parse_used = used + (uint32_t)size;
    return (gltf_module.parse + used);
}

static void *take_for_cgltf([[maybe_unused]] void *user, cgltf_size size)
{
    return (parse_take(size));
}

static void free_for_cgltf([[maybe_unused]] void *user,
    [[maybe_unused]] void *at)
{
}

cgltf_options parse_options(void)
{
    cgltf_options o = { .type = cgltf_file_type_invalid };

    o.memory.alloc_func = take_for_cgltf;
    o.memory.free_func = free_for_cgltf;
    return (o);
}

cgltf_result parse_file(const uint8_t *bytes, uint32_t len, cgltf_data **data)
{
    cgltf_options o = parse_options();
    cgltf_result r;

    gltf_module.parse_used = 0;
    r = cgltf_parse(&o, bytes, len, data);
    if (r == cgltf_result_success)
        r = cgltf_validate(*data);
    return (r);
}
