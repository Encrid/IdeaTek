/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const res_text_t META = { ".meta", 5 };

static uint32_t watch_source(uint64_t entity)
{
    uint32_t source = entity == res_module.default_watch ? RES_DEFAULT
        : RES_NONE;

    for (uint32_t s = 0; s < res_module.s.max_sources; s++)
        source = res_module.sources[s].watch == entity ? s : source;
    return (source);
}

static void mark_changed(uint32_t source, res_text_t changed)
{
    res_resource_t *r;
    res_text_t path;
    res_path_t p;
    bool meta = changed.len >= META.len && !memcmp(changed.at + changed.len
        - META.len, META.at, META.len);
    double delay = res_module.no_clock ? 0.0 : res_module.s.reload_delay_ms
        / 1000.0;

    changed.len -= meta * META.len;
    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        if (!r->entity || r->source != source || r->parent != RES_NONE)
            continue;
        path = text_of(i, 0);
        path_split(&path, &p);
        if (path_is_under(&p.path, &changed)) {
            r->reload_at = res_module.now + delay;
            r->reload_set = true;
        }
    }
}

static void mark_lines(const bridge_api_t *api, uint32_t source,
    res_text_t lines)
{
    res_text_t folder = res_text(api, folder_of(source));
    res_text_t line;
    const char *nl;
    uint32_t cut;

    while (lines.len) {
        nl = memchr(lines.at, '\n', lines.len);
        line.at = lines.at;
        line.len = nl ? (uint32_t)(nl - lines.at) : lines.len;
        lines.at += line.len + (nl != nullptr);
        lines.len -= line.len + (nl != nullptr);
        if (!path_is_under(&line, &folder))
            continue;
        cut = folder.len + (folder.len > 0) * (line.len > folder.len);
        cut += (line.len > cut) & (line.at[cut] == '/');
        line.at += cut;
        line.len -= cut;
        mark_changed(source, line);
    }
}

static void say_no_clock(const bridge_api_t *api)
{
    bool clock = bridge_native_get_single(api, FRAME) != nullptr;

    if (!clock && !res_module.no_clock)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no clock: a change reloads in the round after");
    res_module.no_clock |= !clock;
}

BRIDGE_NATIVE(changes)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_CHANGES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    const ideatek_file_change_t *c;
    uint32_t source;
    uint32_t at;

    for (uint32_t k = 0; k < l->count; k++) {
        source = watch_source(t[k].id);
        at = source == RES_NONE ? 0 : bridge_native_find_value(api, t[k].id,
            FILE_CHANGE);
        if (!at)
            continue;
        c = (const void *)(api->shared_base + at);
        say_no_clock(api);
        mark_lines(api, source, res_text(api, c->paths));
    }
    return (0);
}
