/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const res_text_t META = { ".meta", 5 };

static bool value_of(const bridge_api_t *api, uint32_t i,
    const res_text_t *name, res_text_t *value)
{
    const res_resource_t *r = &res_module.resources[i];
    uint32_t k = find_variable(api, name);
    const ideatek_resource_variable_t *v;
    uint32_t at;

    if (k == RES_NONE)
        return (false);
    at = bridge_native_find_value(api, res_module.variables[k].entity,
        VARIABLE);
    v = (const void *)(api->shared_base + at);
    *value = res_text(api, r->fallback_used ? v->fallback : v->value);
    return (true);
}

bool write_path(const bridge_api_t *api, uint32_t i, uint32_t kind,
    res_buffer_t *out)
{
    const res_resource_t *r = &res_module.resources[i];
    res_text_t text = text_of(i, 0);
    res_text_t folder = res_text(api, folder_of(r->source));
    res_text_t value = { "", 0 };
    res_text_t name;
    char resolved[RES_PATH];
    res_buffer_t buffer = { resolved, sizeof(resolved), 0 };
    res_path_t p;

    path_split(&text, &p);
    if (path_variable(&p.path, &name) && !value_of(api, i, &name, &value))
        return (false);
    path_resolve(&p.path, &value, &buffer);
    text.at = buffer.at;
    text.len = buffer.len;
    path_join(&folder, &text, out);
    memcpy(out->at + out->len, META.at, (kind == READ_META) * META.len);
    out->len += (kind == READ_META) * META.len;
    out->at[out->len] = '\0';
    return (true);
}

void ask_file(const bridge_api_t *api, const res_read_t *read,
    bridge_text_t path)
{
    const res_resource_t *r = &res_module.resources[read->resource];
    ideatek_file_request_t q = { .offset = read->offset,
        .length = read->length, .path = path,
        .action = IDEATEK_FILE_ACTION_READ, .priority = r->priority };

    q.storage = r->source == RES_USER ? IDEATEK_STORAGE_USER
        : IDEATEK_STORAGE_CONTENT;
    BRIDGE_NATIVE_ATTACH(api, read->entity, FILE_REQUEST, q);
}

void ask_source(const bridge_api_t *api, const res_read_t *read,
    bridge_text_t path)
{
    const res_resource_t *r = &res_module.resources[read->resource];
    ideatek_resource_source_read_t q = { .offset = read->offset,
        .length = read->length, .path = path, .priority = r->priority };

    q.source.id = res_module.sources[r->source].entity;
    BRIDGE_NATIVE_ATTACH(api, read->entity, SOURCE_READ, q);
}

bool is_module_source(uint32_t source)
{
    return (source < RES_RAW && res_module.sources[source].entity != 0);
}
