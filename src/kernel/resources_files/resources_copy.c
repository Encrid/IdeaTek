/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const ideatek_resource_error_t OF_STATUS[6] = {
    IDEATEK_RESOURCE_ERROR_NONE, IDEATEK_RESOURCE_ERROR_NOT_FOUND,
    IDEATEK_RESOURCE_ERROR_TOO_BIG, IDEATEK_RESOURCE_ERROR_READ_FAILED,
    IDEATEK_RESOURCE_ERROR_READ_FAILED, IDEATEK_RESOURCE_ERROR_READ_FAILED };

void read_arrival(const bridge_api_t *api, uint32_t j,
    res_arrival_t *out)
{
    const res_read_t *read = &res_module.reads[j];
    const res_resource_t *r = &res_module.resources[read->resource];
    bool module = is_module_source(r->source);
    uint32_t at = bridge_native_find_value(api, read->entity, module
        ? SOURCE_RESULT : FILE_RESULT);
    const ideatek_file_result_t *f = (const void *)(api->shared_base + at);
    const ideatek_resource_source_result_t *s = (const void *)f;

    out->module = module;
    out->size = module ? s->size : f->size;
    out->data = res_text(api, module ? s->data : f->data);
    out->error = module ? read->error : OF_STATUS[read->status % 6];
}

bool copy_bytes(const bridge_api_t *api, uint32_t i, const res_text_t *data)
{
    res_resource_t *r = &res_module.resources[i];
    uint32_t start;

    if (!blocks_take(&res_module.blocks, data->len, i, &start))
        return (false);
    memcpy(api->shared_base + res_module.arena_at + start, data->at,
        data->len);
    r->block_at = res_module.arena_at + start;
    r->block_len = data->len;
    return (true);
}
