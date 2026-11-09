/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static bool is_alive(const bridge_api_t *api, uint64_t id)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    const bridge_entity_t *e = (const bridge_entity_t *)(api->shared_base
        + h->entity_index) + (uint32_t)id;
    bool inside = (uint32_t)id < h->entities;

    return (inside && e->generation == (uint32_t)(id >> 32));
}

void check_owners(const bridge_api_t *api)
{
    const res_use_t *u;

    for (uint32_t k = 0; k < res_module.s.max_uses; k++) {
        u = &res_module.uses[k];
        if (u->entity && u->owner && !is_alive(api, u->owner))
            api->destroy(api, u->entity);
    }
}

void cancel_read(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];

    if (r->read == RES_NONE)
        return;
    api->destroy(api, res_module.reads[r->read].entity);
    res_module.reads[r->read].entity = 0;
    r->read = RES_NONE;
}

static void free_resource(const bridge_api_t *api, uint32_t i)
{
    const res_resource_t *r = &res_module.resources[i];
    uint64_t entity = r->entity;

    cancel_read(api, i);
    for (uint32_t j = 0; j < res_module.s.max_resources; j++)
        if (res_module.resources[j].entity
            && res_module.resources[j].parent == i) {
            api->destroy(api, res_module.resources[j].entity);
            drop_resource(j);
        }
    for (uint32_t k = 0; k < res_module.s.max_uses; k++)
        if (res_module.uses[k].entity && res_module.uses[k].owner == entity)
            api->destroy(api, res_module.uses[k].entity);
    api->destroy(api, entity);
    drop_resource(i);
}

void free_dead(const bridge_api_t *api)
{
    const res_resource_t *r;

    if (!res_module.dirty)
        return;
    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        if (r->entity && !r->count && r->parent == RES_NONE)
            free_resource(api, i);
    }
}
