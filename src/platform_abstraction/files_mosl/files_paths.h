/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef FILES_PATHS_H
    #define FILES_PATHS_H

    #include <stddef.h>

constexpr size_t FILES_PATH = 4096;

bool path_is_allowed(const char *path);
bool path_join(char *out, size_t size, const char *storage, const char *path);
void path_split(const char *path, char *folder, char *name);
void path_parent(char *path);

#endif
