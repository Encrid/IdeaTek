/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

gltf_file_t *find_file(uint64_t entity)
{
    for (uint32_t i = 0; i < gltf_module.s.max_files; i++)
        if (gltf_module.files[i].entity == entity)
            return (&gltf_module.files[i]);
    return (nullptr);
}

gltf_file_t *add_file(uint64_t entity)
{
    gltf_file_t *f = find_file(0);

    if (f)
        *f = (gltf_file_t){ .entity = entity };
    return (f);
}

void free_key(uint32_t *key)
{
    if (*key)
        blocks_free(&gltf_module.blocks, *key);
    *key = 0;
}

bool take_block(uint32_t size, uint32_t *key, uint32_t *start)
{
    gltf_module.serial++;
    gltf_module.serial += gltf_module.serial == 0;
    *key = gltf_module.serial;
    if (blocks_take(&gltf_module.blocks, size, *key, start))
        return (true);
    *key = 0;
    return (false);
}

void forget_file(gltf_file_t *f)
{
    gltf_part_t *p;

    free_key(&f->now);
    free_key(&f->old);
    free_key(&f->texts);
    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        p = &gltf_module.parts[k];
        if (p->file == f->entity)
            *p = (gltf_part_t){};
        if (gltf_module.deps[k].file == f->entity)
            gltf_module.deps[k] = (gltf_dep_t){};
    }
    *f = (gltf_file_t){};
}
