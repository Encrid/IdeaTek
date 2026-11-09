/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static void clear_record(uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];

    memset(r, 0, sizeof(*r));
    r->entity = 1;
    r->source = RES_DEFAULT;
    r->loader = RES_NONE;
    r->parent = RES_NONE;
    r->read = RES_NONE;
    r->view.state = IDEATEK_LOAD_STATE_LOADING;
}

static void write_texts(uint32_t i, const res_text_t *path,
    const res_text_t *type, const res_text_t *settings)
{
    res_resource_t *r = &res_module.resources[i];
    char *slot = slot_of(i);

    r->path_len = path->len;
    r->type_len = type->len;
    r->settings_len = settings->len;
    memcpy(slot, path->at, path->len);
    slot[path->len] = '\0';
    memcpy(slot + path->len + 1, type->at, type->len);
    slot[path->len + 1 + type->len] = '\0';
    memcpy(slot + res_module.s.path_bytes, settings->at, settings->len);
}

uint32_t take_resource(const res_text_t *path, const res_text_t *type,
    const res_text_t *settings)
{
    const res_settings_t *s = &res_module.s;
    uint32_t i = 0;
    bool fits = (path->len + type->len + 2 <= s->path_bytes)
        & (settings->len <= s->settings_bytes);
    res_text_t empty = { "", 0 };

    while (i < s->max_resources && res_module.resources[i].entity)
        i++;
    if (i == s->max_resources)
        return (RES_NONE);
    clear_record(i);
    if (fits)
        write_texts(i, path, type, settings);
    else
        write_texts(i, &empty, &empty, &empty);
    if (fits)
        map_put(&res_module.by_key, res_hash_key(path, type), i);
    else
        fail_resource(nullptr, i, IDEATEK_RESOURCE_ERROR_NO_ROOM);
    return (i);
}

void bind_resource(uint32_t i, uint64_t entity)
{
    res_resource_t *r = &res_module.resources[i];
    uint32_t base = res_module.texts_at + i * res_module.slot_bytes;

    r->entity = entity;
    r->view.path.at = base;
    r->view.path.len = r->path_len;
    r->view.type.at = base + r->path_len + 1;
    r->view.type.len = r->type_len;
    map_put(&res_module.by_entity, entity, i);
    r->fresh = true;
    touch(i);
}

uint32_t use_of(uint64_t entity)
{
    return (map_find(&res_module.use_by_entity, entity));
}
