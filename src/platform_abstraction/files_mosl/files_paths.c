/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "files_paths.h"

static bool name_is_allowed(const char *name, size_t len)
{
    bool dot = len == 1 && name[0] == '.';
    bool dots = len == 2 && name[0] == '.' && name[1] == '.';

    return (len > 0 && !dot && !dots);
}

bool path_is_allowed(const char *path)
{
    const char *start = path;
    const char *p = path;
    bool ok = true;

    if (!path[0])
        return (true);
    for (; *p; p++) {
        ok &= *p != '\\' && *p != ':';
        if (*p == '/') {
            ok &= name_is_allowed(start, (size_t)(p - start));
            start = p + 1;
        }
    }
    ok &= name_is_allowed(start, (size_t)(p - start));
    return (ok);
}

bool path_join(char *out, size_t size, const char *storage, const char *path)
{
    size_t storage_len = strlen(storage);
    size_t path_len = strlen(path);
    size_t sep = path_len > 0;

    if (storage_len + sep + path_len + 1 > size)
        return (false);
    memcpy(out, storage, storage_len);
    memcpy(out + storage_len, "/", sep);
    memcpy(out + storage_len + sep, path, path_len + 1);
    return (true);
}

void path_split(const char *path, char *folder, char *name)
{
    const char *slash = strrchr(path, '/');
    size_t folder_len = slash ? (size_t)(slash - path) : 0;
    const char *last = slash ? slash + 1 : path;

    memcpy(folder, path, folder_len);
    folder[folder_len] = '\0';
    memcpy(name, last, strlen(last) + 1);
}
