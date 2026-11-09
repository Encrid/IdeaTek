/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_PATHS_H
    #define RESOURCES_PATHS_H

    #include <stdint.h>

constexpr uint32_t RES_NAME = 64;
constexpr uint32_t RES_NONE = UINT32_MAX;

typedef struct {
    const char *at;
    uint32_t len;
} res_text_t;

typedef struct {
    char *at;
    uint32_t size;
    uint32_t len;
} res_buffer_t;

typedef struct {
    res_text_t source;
    res_text_t path;
    res_text_t label;
} res_path_t;

bool same_text(const res_text_t *a, const res_text_t *b);
void path_split(const res_text_t *text, res_path_t *out);
bool source_name_is_allowed(const res_text_t *name);
bool path_variable(const res_text_t *path, res_text_t *name);
bool path_resolve(const res_text_t *path, const res_text_t *value,
    res_buffer_t *out);
bool path_extension(const res_text_t *path, res_text_t *extension);
bool path_join(const res_text_t *folder, const res_text_t *path,
    res_buffer_t *out);
bool path_is_under(const res_text_t *path, const res_text_t *folder);

#endif
