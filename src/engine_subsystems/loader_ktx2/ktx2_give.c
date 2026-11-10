/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "ktx2_inside.h"

static uint32_t next_key(void)
{
    ktx2_module.serial++;
    ktx2_module.serial += ktx2_module.serial == 0;
    return (ktx2_module.serial);
}

static bool take_room(const bridge_api_t *api, uint64_t id,
    const ktx2_file_t *f, uint32_t *start)
{
    res_blocks_t *blocks = &ktx2_module.blocks;
    char line[128];

    if (f->texel_bytes <= blocks->size && blocks_take(blocks,
        (uint32_t)f->texel_bytes, ktx2_module.serial, start))
        return (true);
    snprintf(line, sizeof(line), " needs %llu bytes, and the most free "
        "bytes in 1 part are %u", (unsigned long long)f->texel_bytes,
        blocks_room(blocks));
    say_path(api, id, line);
    return (false);
}

static void attach_texture(const bridge_api_t *api, ktx2_texture_t *t,
    const ktx2_file_t *f, uint32_t start)
{
    ideatek_texture_t x = { .bytes = { ktx2_module.arena_at + start,
        (uint32_t)f->texel_bytes }, .format = f->format, .width = f->width,
        .height = f->height, .mip_count = f->levels,
        .revision = ++t->revision, .dimension = ktx2_dimension(f) };

    x.depth_or_layers = f->depth ? f->depth : f->layers;
    x.depth_or_layers += x.depth_or_layers == 0;
    BRIDGE_NATIVE_ATTACH(api, t->entity, TEXTURE, x);
}

static ktx2_texture_t *get_record(const bridge_api_t *api, uint64_t id)
{
    ktx2_texture_t *t = find_texture(id);
    char line[96];

    t = t ? t : add_texture(id);
    if (t)
        return (t);
    snprintf(line, sizeof(line), " is refused: %u textures are loaded",
        ktx2_module.s.max_textures);
    say_path(api, id, line);
    return (nullptr);
}

bool give_texture(const bridge_api_t *api, uint64_t id,
    const ktx2_file_t *f, ktx2_bytes_t file)
{
    ktx2_texture_t *t = get_record(api, id);
    uint32_t start;

    if (!t)
        return (false);
    next_key();
    if (!take_room(api, id, f, &start)) {
        if (!t->now)
            forget_texture(t);
        return (false);
    }
    ktx2_copy(file, f, api->shared_base + ktx2_module.arena_at + start);
    free_block(&t->old);
    t->old = t->now;
    t->now = ktx2_module.serial;
    attach_texture(api, t, f, start);
    return (true);
}
