/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_inside.h"

ktx2_texture_t *find_texture(uint64_t entity)
{
    for (uint32_t i = 0; i < ktx2_module.s.max_textures; i++)
        if (ktx2_module.textures[i].entity == entity)
            return (&ktx2_module.textures[i]);
    return (nullptr);
}

ktx2_texture_t *add_texture(uint64_t entity)
{
    ktx2_texture_t *t = find_texture(0);

    if (t)
        *t = (ktx2_texture_t){ .entity = entity };
    return (t);
}

void free_block(uint32_t *key)
{
    if (*key)
        blocks_free(&ktx2_module.blocks, *key);
    *key = 0;
}

void forget_texture(ktx2_texture_t *t)
{
    free_block(&t->now);
    free_block(&t->old);
    *t = (ktx2_texture_t){};
}
