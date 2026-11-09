/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

uint32_t new_label(const bridge_api_t *api, uint32_t parent,
    const res_text_t *label, uint64_t entity)
{
    uint32_t j = label_record(parent, label);

    if (j == RES_NONE || !entity) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for the label %.*s",
            (int)label->len, label->at);
        return (RES_NONE);
    }
    bind_resource(j, entity);
    res_module.resources[j].parent = parent;
    res_module.resources[j].step = STEP_DONE;
    res_module.resources[j].seen = true;
    return (j);
}

static void hear_label(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = bridge_native_find_value(api, id, LABEL);
    const ideatek_resource_label_t *c = (const void *)(api->shared_base + at);
    uint32_t parent = at ? resource_of(c->resource.id) : RES_NONE;
    uint32_t j = parent == RES_NONE ? RES_NONE : resource_of(id);
    res_text_t label = { "", 0 };

    if (parent != RES_NONE && j == RES_NONE) {
        label = res_text(api, c->label);
        new_label(api, parent, &label, id);
    } else if (j != RES_NONE)
        res_module.resources[j].seen = true;
}

BRIDGE_NATIVE(labels)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_LABELS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t k = 0; k < l->count; k++)
        hear_label(api, t[k].id);
    return (0);
}
