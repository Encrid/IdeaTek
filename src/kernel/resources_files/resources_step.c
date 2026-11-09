/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

void fail_resource(const bridge_api_t *api, uint32_t i,
    ideatek_resource_error_t error)
{
    res_resource_t *r = &res_module.resources[i];
    bool first = r->view.error == IDEATEK_RESOURCE_ERROR_NONE;

    r->view.state = IDEATEK_LOAD_STATE_FAILED;
    r->view.error = first ? error : r->view.error;
    r->step = STEP_DONE;
    r->pending |= TAG_FAILED;
    r->reloading = false;
    blocks_free(&res_module.blocks, i);
    r->block_len = 0;
    if (api)
        touch(i);
}

static bool wants_meta(const bridge_api_t *api, uint32_t i)
{
    res_text_t path = text_of(i, 0);
    res_text_t list = res_text(api, res_module.s.meta_paths);
    const char *at = list.at;
    const char *end = list.at + list.len;
    const char *comma;
    res_text_t item;
    bool found = false;

    while (at < end) {
        comma = memchr(at, ',', (size_t)(end - at));
        item.at = at;
        item.len = (uint32_t)((comma ? comma : end) - at);
        found |= same_text(&item, &path);
        at = (comma ? comma : end) + 1;
    }
    return ((res_module.meta_check == 0) | ((res_module.meta_check == 2)
        & found));
}

void step_resource(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];

    if (r->step == STEP_NEW && wants_meta(api, i)) {
        r->step = start_read(api, i, READ_META) ? STEP_META : STEP_NEW;
        return;
    }
    if (r->step == STEP_NEW)
        r->step = STEP_PICK;
    if (r->step == STEP_PICK)
        pick_for(api, i);
}

static void give_raw(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    ideatek_raw_bytes_t raw = { .data = { r->block_at, r->block_len } };

    BRIDGE_NATIVE_ATTACH(api, r->entity, RAW, raw);
    r->view.state = IDEATEK_LOAD_STATE_LOADED;
    r->step = STEP_DONE;
    r->pending |= TAG_RAW | (r->reloading ? TAG_MODIFIED : 0);
    r->reloading = false;
    mark_labels(api, i);
    touch(i);
}

void give_bytes(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    bool ranged = r->loader < RES_RAW && res_module.loaders[r->loader].ranged;
    uint32_t head = res_module.s.head_bytes;
    uint32_t len = ranged && r->block_len > head ? head : r->block_len;
    ideatek_resource_bytes_t b = { .size = r->size,
        .data = { r->block_at, len } };

    if (r->loader == RES_RAW) {
        give_raw(api, i);
        return;
    }
    b.settings.at = res_module.texts_at + i * res_module.slot_bytes
        + res_module.s.path_bytes;
    b.settings.len = r->settings_len;
    b.loader.id = res_module.loaders[r->loader].entity;
    BRIDGE_NATIVE_ATTACH(api, r->entity, BYTES, b);
    r->step = STEP_LOADER;
}
