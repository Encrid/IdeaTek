/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_paths.h"

static const char *find_last(const res_text_t *text, char c)
{
    const char *found = nullptr;

    for (uint32_t i = 0; i < text->len; i++)
        found = text->at[i] == c ? text->at + i : found;
    return (found);
}

static const char *find_scheme(const res_text_t *text)
{
    for (uint32_t i = 0; i + 3 <= text->len; i++)
        if (!memcmp(text->at + i, "://", 3))
            return (text->at + i);
    return (nullptr);
}

void path_split(const res_text_t *text, res_path_t *out)
{
    const char *end = text->at + text->len;
    const char *colon = find_scheme(text);
    const char *start = colon ? colon + 3 : text->at;
    res_text_t rest = { start, (uint32_t)(end - start) };
    const char *hash = find_last(&rest, '#');
    const char *stop = hash ? hash : end;

    out->source.at = text->at;
    out->source.len = colon ? (uint32_t)(colon - text->at) : 0;
    out->path.at = start;
    out->path.len = (uint32_t)(stop - start);
    out->label.at = hash ? hash + 1 : end;
    out->label.len = hash ? (uint32_t)(end - hash - 1) : 0;
}

bool source_name_is_allowed(const res_text_t *name)
{
    bool ok = (name->len > 0) & (name->len < RES_NAME);
    char c;

    for (uint32_t i = 0; i < name->len; i++) {
        c = name->at[i];
        ok &= ((c >= 'a') & (c <= 'z')) | ((c >= '0') & (c <= '9'))
            | (c == '_');
    }
    return (ok);
}

bool path_extension(const res_text_t *path, res_text_t *extension)
{
    const char *slash = find_last(path, '/');
    const char *end = path->at + path->len;
    res_text_t name = { slash ? slash + 1 : path->at, 0 };
    const char *dot;

    name.len = (uint32_t)(end - name.at);
    dot = find_last(&name, '.');
    extension->at = dot ? dot + 1 : end;
    extension->len = (uint32_t)(end - extension->at);
    return (dot != nullptr);
}
