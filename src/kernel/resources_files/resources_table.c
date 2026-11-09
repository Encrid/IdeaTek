/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

char *slot_of(uint32_t i)
{
    return ((char *)res_module.texts + (size_t)i * res_module.slot_bytes);
}

res_text_t text_of(uint32_t i, uint32_t part)
{
    const res_resource_t *r = &res_module.resources[i];
    const char *slot = slot_of(i);
    res_text_t parts[3] = {{ slot, r->path_len },
        { slot + r->path_len + 1, r->type_len },
        { slot + res_module.s.path_bytes, r->settings_len }};

    return (parts[part]);
}

uint32_t find_resource(const res_text_t *path, const res_text_t *type)
{
    uint32_t i = map_find(&res_module.by_key, res_hash_key(path, type));
    res_text_t own_path;
    res_text_t own_type;

    if (i == RES_NONE)
        return (RES_NONE);
    own_path = text_of(i, 0);
    own_type = text_of(i, 1);
    return (same_text(&own_path, path) && same_text(&own_type, type) ? i
        : RES_NONE);
}

uint32_t resource_of(uint64_t entity)
{
    return (map_find(&res_module.by_entity, entity));
}

void drop_resource(uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    res_text_t path = text_of(i, 0);
    res_text_t type = text_of(i, 1);

    if (r->path_len | r->type_len)
        map_drop(&res_module.by_key, res_hash_key(&path, &type));
    map_drop(&res_module.by_entity, r->entity);
    blocks_free(&res_module.blocks, i);
    r->entity = 0;
}
