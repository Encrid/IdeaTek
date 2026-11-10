/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "gltf_inside.h"

bridge_text_t text_of(const gltf_build_t *b, gltf_span_t span)
{
    return ((bridge_text_t){ gltf_module.arena_at + b->start + span.at,
        span.len });
}

static bool take_output(gltf_build_t *b, uint32_t *key)
{
    uint32_t size = b->out.at;
    char line[128];

    if (take_block(size, key, &b->start))
        return (true);
    snprintf(line, sizeof(line), ": the output needs %u bytes, and 1 free "
        "part holds at most %u", size, blocks_room(&gltf_module.blocks));
    say_path(b->api, b->file->entity, line);
    return (false);
}

bool build_file(const bridge_api_t *api, gltf_file_t *f, cgltf_data *data)
{
    gltf_build_t b = { .api = api, .data = data, .file = f };
    uint32_t key = 0;

    if (!wire_buffers(&b) || !plan_parts(&b))
        return (false);
    rank_joints(&b);
    say_images(&b);
    write_all(&b);
    if (!take_output(&b, &key))
        return (false);
    b.out = (gltf_out_t){ api->shared_base + gltf_module.arena_at + b.start,
        0 };
    write_all(&b);
    free_key(&f->old);
    f->old = f->now;
    f->now = key;
    attach_all(&b);
    return (true);
}
