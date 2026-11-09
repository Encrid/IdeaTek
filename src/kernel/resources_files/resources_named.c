/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const char *const LINES[2] = { "2 sources have the name %.*s",
    "no room for the source %.*s" };

res_source_t *free_source(void)
{
    for (uint32_t i = 0; i < res_module.s.max_sources; i++)
        if (!res_module.sources[i].used)
            return (&res_module.sources[i]);
    return (nullptr);
}

static void add_named(const bridge_api_t *api, const res_text_t *item,
    uint32_t at)
{
    const char *eq = memchr(item->at, '=', item->len);
    res_text_t name = { item->at, eq ? (uint32_t)(eq - item->at)
        : item->len };
    res_source_t *s = free_source();
    bool known = find_source(&name) != RES_NONE;
    uint32_t fault = known + 2 * (!known & !s);

    if (fault) {
        bridge_native_say(api, BRIDGE_LOG_WARN, LINES[fault - 1],
            (int)name.len, name.at);
        return;
    }
    s->name_len = name.len;
    s->used = true;
    s->folder_at = at + name.len + (eq != nullptr);
    s->folder_len = item->len - name.len - (eq != nullptr);
    memcpy(s->name, name.at, name.len);
}

void read_sources(const bridge_api_t *api)
{
    bridge_text_t text = res_module.s.sources;
    const char *base = (const char *)api->shared_base + text.at;
    uint32_t start = 0;
    res_text_t item;

    for (uint32_t i = 0; i <= text.len; i++) {
        if (i < text.len && base[i] != ',')
            continue;
        item.at = base + start;
        item.len = i - start;
        if (item.len)
            add_named(api, &item, text.at + start);
        start = i + 1;
    }
}

bridge_text_t folder_of(uint32_t source)
{
    const res_source_t *s = &res_module.sources[source
        % res_module.s.max_sources];
    bridge_text_t named = { s->folder_at, s->folder_len };
    bridge_text_t processed = { res_module.s.processed_folder.at,
        res_module.processed * res_module.s.processed_folder.len };
    bridge_text_t none = { 0, 0 };

    if (source == RES_DEFAULT)
        return (processed);
    if (source == RES_USER || s->entity)
        return (none);
    return (named);
}

void start_watch(const bridge_api_t *api, uint32_t source)
{
    ideatek_file_request_t r = { .path = folder_of(source),
        .storage = IDEATEK_STORAGE_CONTENT,
        .action = IDEATEK_FILE_ACTION_WATCH };
    uint64_t id = api->spawn(api);

    if (!id) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no watch of %.*s: no entity",
            (int)r.path.len, (const char *)api->shared_base + r.path.at);
        return;
    }
    if (source == RES_DEFAULT)
        res_module.default_watch = id;
    else
        res_module.sources[source].watch = id;
    BRIDGE_NATIVE_ATTACH(api, id, FILE_REQUEST, r);
}
