/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_paths.h"

bool path_variable(const res_text_t *path, res_text_t *name)
{
    const char *open = memchr(path->at, '{', path->len);
    size_t left = open ? path->len - (size_t)(open - path->at) : 0;
    const char *close = open ? memchr(open, '}', left) : nullptr;

    name->at = open ? open + 1 : path->at;
    name->len = close ? (uint32_t)(close - open - 1) : 0;
    return (close != nullptr);
}

static bool put_piece(res_buffer_t *out, const char *at, uint32_t len)
{
    bool fits = out->size - out->len > len;

    memcpy(out->at + out->len, at, fits * len);
    out->len += fits * len;
    out->at[out->len] = '\0';
    return (fits);
}

bool path_resolve(const res_text_t *path, const res_text_t *value,
    res_buffer_t *out)
{
    const char *at = path->at;
    const char *end = path->at + path->len;
    const char *open;
    const char *close;
    bool fits = true;

    out->len = 0;
    out->at[0] = '\0';
    while (at < end && fits) {
        open = memchr(at, '{', (size_t)(end - at));
        close = open ? memchr(open, '}', (size_t)(end - open)) : nullptr;
        if (open && !close)
            return (false);
        fits = put_piece(out, at, (uint32_t)((open ? open : end) - at));
        fits &= !close || put_piece(out, value->at, value->len);
        at = close ? close + 1 : end;
    }
    return (fits);
}

bool path_join(const res_text_t *folder, const res_text_t *path,
    res_buffer_t *out)
{
    uint32_t slash = (folder->len > 0) & (path->len > 0);
    uint32_t len = folder->len + slash + path->len;
    bool fits = len < out->size;

    out->len = fits * len;
    memcpy(out->at, folder->at, fits * folder->len);
    memcpy(out->at + folder->len, "/", fits * slash);
    memcpy(out->at + folder->len + slash, path->at, fits * path->len);
    out->at[out->len] = '\0';
    return (fits);
}

bool path_is_under(const res_text_t *path, const res_text_t *folder)
{
    bool prefix = (path->len >= folder->len)
        && !memcmp(path->at, folder->at, folder->len);
    bool same = path->len == folder->len;
    bool below = (path->len > folder->len)
        && (folder->len == 0 || path->at[folder->len] == '/');

    return (prefix & (same | below));
}
