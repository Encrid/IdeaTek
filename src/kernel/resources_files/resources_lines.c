/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_meta.h"

res_text_t line_at(const res_text_t *bytes, uint32_t at, uint32_t *end)
{
    const char *nl = memchr(bytes->at + at, '\n', bytes->len - at);
    res_text_t line = { bytes->at + at, 0 };

    *end = nl ? (uint32_t)(nl - bytes->at) : bytes->len;
    line.len = *end - at;
    return (line);
}

void meta_start(const res_text_t *bytes, res_meta_t *meta)
{
    meta->action = META_LOAD;
    meta->loader.at = bytes->at;
    meta->loader.len = 0;
    meta->settings.at = bytes->at + bytes->len;
    meta->settings.len = 0;
    meta->line = 1;
}

void take_rest(const res_text_t *bytes, res_meta_t *meta,
    uint32_t end)
{
    uint32_t start = end < bytes->len ? end + 1 : end;

    meta->settings.at = bytes->at + start;
    meta->settings.len = bytes->len - start;
}
