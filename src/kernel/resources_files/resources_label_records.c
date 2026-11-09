/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static uint32_t count_labels(void)
{
    uint32_t count = 0;

    for (uint32_t j = 0; j < res_module.s.max_resources; j++)
        count += res_module.resources[j].entity
            && res_module.resources[j].parent != RES_NONE;
    return (count);
}

static void label_path(uint32_t parent, const res_text_t *label,
    res_buffer_t *out)
{
    res_text_t path = text_of(parent, 0);
    bool fits = path.len + 1 + label->len < out->size;

    out->len = fits * (path.len + 1 + label->len);
    memcpy(out->at, path.at, fits * path.len);
    memcpy(out->at + path.len, "#", fits);
    memcpy(out->at + path.len + 1, label->at, fits * label->len);
    out->at[out->len] = '\0';
}

uint32_t label_record(uint32_t parent, const res_text_t *label)
{
    res_text_t type = text_of(parent, 1);
    res_text_t empty = { "", 0 };
    char full[RES_PATH];
    res_buffer_t buffer = { full, sizeof(full), 0 };
    res_text_t joined;

    if (count_labels() >= res_module.s.max_labels)
        return (RES_NONE);
    label_path(parent, label, &buffer);
    joined.at = full;
    joined.len = buffer.len;
    return (take_resource(&joined, &type, &empty));
}
