/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "files_paths.h"

int main(int argc, char **argv)
{
    char full[FILES_PATH];
    char folder[FILES_PATH];
    char name[FILES_PATH];

    if (argc < 3)
        return (2);
    if (!strcmp(argv[1], "allowed"))
        printf("allowed %d\n", path_is_allowed(argv[2]));
    if (!strcmp(argv[1], "join") && argc >= 4)
        printf("join %d [%s]\n", path_join(full, sizeof(full), argv[2],
            argv[3]), full);
    if (!strcmp(argv[1], "split")) {
        path_split(argv[2], folder, name);
        printf("split [%s] [%s]\n", folder, name);
    }
    if (!strcmp(argv[1], "parent")) {
        path_parent(argv[2]);
        printf("parent [%s]\n", argv[2]);
    }
    return (0);
}
