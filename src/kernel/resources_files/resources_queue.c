/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static uint32_t free_read(void)
{
    for (uint32_t j = 0; j < res_module.s.max_reads; j++)
        if (!res_module.reads[j].entity)
            return (j);
    return (RES_NONE);
}

static void fill_read(uint32_t j, uint32_t i, uint32_t kind)
{
    res_read_t *read = &res_module.reads[j];
    const res_resource_t *r = &res_module.resources[i];
    bool ranged = r->loader < RES_RAW && res_module.loaders[r->loader].ranged;
    bool range = kind == READ_RANGE;
    bool head = (kind == READ_FILE) & ranged;

    read->resource = i;
    read->kind = kind;
    read->offset = range * r->range_offset;
    read->length = range * r->range_length + head * res_module.s.head_bytes;
    read->done = false;
    read->status = IDEATEK_FILE_STATUS_OK;
    read->error = IDEATEK_RESOURCE_ERROR_NONE;
}

static bool send_read(const bridge_api_t *api, uint32_t i, uint32_t j)
{
    res_resource_t *r = &res_module.resources[i];
    res_read_t *read = &res_module.reads[j];
    bridge_text_t where = { res_module.paths_at + j * RES_PATH, 0 };
    res_buffer_t path = { (char *)api->shared_base + where.at, RES_PATH, 0 };

    if (!write_path(api, i, read->kind, &path)) {
        fail_resource(api, i, IDEATEK_RESOURCE_ERROR_NOT_FOUND);
        return (true);
    }
    read->entity = api->spawn(api);
    if (!read->entity)
        return (false);
    r->read = j;
    where.len = path.len;
    if (is_module_source(r->source))
        ask_source(api, read, where);
    else
        ask_file(api, read, where);
    return (true);
}

bool start_read(const bridge_api_t *api, uint32_t i, uint32_t kind)
{
    uint32_t j = free_read();

    if (j == RES_NONE)
        return (false);
    fill_read(j, i, kind);
    return (send_read(api, i, j));
}
