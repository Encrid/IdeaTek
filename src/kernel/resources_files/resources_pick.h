/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_PICK_H
    #define RESOURCES_PICK_H

    #include <stdint.h>

    #include "ideatek_resources.h"
    #include "resources_paths.h"

typedef struct {
    uint32_t index;
    res_text_t name;
    res_text_t type;
    res_text_t extensions;
    res_text_t magic;
    bool ranged;
} res_loader_view_t;

typedef struct {
    const res_loader_view_t *views;
    uint32_t count;
    res_text_t type;
    res_text_t extension;
    res_text_t name;
    res_text_t bytes;
    bool has_bytes;
} res_pick_ask_t;

typedef struct {
    uint32_t loader;
    ideatek_resource_error_t error;
    bool needs_bytes;
} res_pick_t;

typedef struct {
    uint32_t hits;
    uint32_t magic;
    uint32_t by_ext;
    uint32_t by_magic;
} res_tally_t;

bool has_extension(const res_loader_view_t *view,
    const res_text_t *extension);
bool fits_magic(const res_loader_view_t *v, const res_pick_ask_t *ask);
bool fits_extension(const res_loader_view_t *v, const res_pick_ask_t *ask);
void pick_loader(const res_pick_ask_t *ask, res_pick_t *out);

#endif
