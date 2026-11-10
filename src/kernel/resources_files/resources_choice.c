/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const res_text_t RAW_TYPE = { "ideatek:RawBytes", 16 };

static void ask_for(const bridge_api_t *api, uint32_t i, res_pick_ask_t *ask)
{
    const res_resource_t *r = &res_module.resources[i];
    res_text_t path = text_of(i, 0);

    ask->views = res_module.views;
    ask->count = 0;
    ask->type = text_of(i, 1);
    ask->name.at = "";
    ask->name.len = 0;
    ask->has_bytes = r->block_len > 0;
    ask->bytes.at = (const char *)api->shared_base + r->block_at;
    ask->bytes.len = r->block_len;
    path_extension(&path, &ask->extension);
}

void pick_for(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    res_pick_ask_t ask;
    res_pick_t pick = { .loader = r->loader };

    ask_for(api, i, &ask);
    if (same_text(&ask.type, &RAW_TYPE))
        pick.loader = RES_RAW;
    else if (r->loader == RES_NONE) {
        ask.count = build_views(api);
        pick_loader(&ask, &pick);
    }
    if (pick.loader == RES_NONE && !pick.needs_bytes) {
        fail_resource(api, i, pick.error);
        return;
    }
    r->loader = pick.loader;
    r->step = STEP_READ;
    if (r->block_len)
        give_bytes(api, i);
}
