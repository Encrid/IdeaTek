/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static bool take_settings(uint32_t i, const res_meta_t *meta)
{
    res_resource_t *r = &res_module.resources[i];
    bool fits = meta->settings.len <= res_module.s.settings_bytes;

    if (r->settings_len || !meta->settings.len)
        return (true);
    memcpy(slot_of(i) + res_module.s.path_bytes, meta->settings.at,
        fits * meta->settings.len);
    r->settings_len = fits * meta->settings.len;
    return (fits);
}

static ideatek_resource_error_t named_loader(const bridge_api_t *api,
    uint32_t i, const res_text_t *name)
{
    res_pick_ask_t ask = { .views = res_module.views, .type = text_of(i, 1),
        .name = *name };
    res_pick_t pick;

    ask.count = build_views(api);
    pick_loader(&ask, &pick);
    res_module.resources[i].loader = pick.loader;
    return (pick.error);
}

static ideatek_resource_error_t meta_error(const bridge_api_t *api,
    uint32_t i, const res_meta_t *meta, ideatek_resource_error_t error)
{
    if (meta->loader.len && !error)
        error = named_loader(api, i, &meta->loader);
    if (meta->action == META_IGNORE && !error)
        error = IDEATEK_RESOURCE_ERROR_IGNORED;
    if (!take_settings(i, meta) && !error)
        error = IDEATEK_RESOURCE_ERROR_NO_ROOM;
    return (error);
}

void apply_meta(const bridge_api_t *api, uint32_t i, const res_text_t *bytes,
    ideatek_resource_error_t error)
{
    res_resource_t *r = &res_module.resources[i];
    res_meta_t meta;
    res_text_t empty = { "", 0 };
    res_text_t path = text_of(i, 0);
    bool good = meta_read(bytes ? bytes : &empty, &meta);

    if (bytes && !good)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the meta file of %.*s is refused at line %u", (int)path.len,
            path.at, meta.line);
    error = !good && !error ? IDEATEK_RESOURCE_ERROR_READ_FAILED : error;
    r->action = meta.action;
    error = meta_error(api, i, &meta, error);
    if (error) {
        fail_resource(api, i, error);
        return;
    }
    r->step = STEP_PICK;
    step_resource(api, i);
}

