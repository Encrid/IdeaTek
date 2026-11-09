/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const uint32_t TAG_SLOTS[4] = { READY, FAILED, MODIFIED, RAW };

static void drop_tags(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    uint32_t tags = r->entity ? r->tags : 0;

    for (uint32_t b = 0; b < 4; b++)
        if (tags & (1u << b))
            api->detach(api, r->entity, TAG_SLOTS[b]);
    if (tags & TAG_RAW)
        blocks_free(&res_module.blocks, i);
    r->block_len = tags & TAG_RAW ? 0 : r->block_len;
    r->tags = 0;
}

void turn_tags(const bridge_api_t *api)
{
    for (uint32_t i = 0; i < res_module.s.max_resources; i++)
        drop_tags(api, i);
}
