/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resources_arena.h"

static res_block_t blocks[4];
static res_blocks_t list = { blocks, 4, 0, 100 };

int main(int argc, char **argv)
{
    uint32_t start = 0;
    bool ok;

    for (int i = 1; i + 2 < argc; i += 3) {
        if (!strcmp(argv[i], "take")) {
            ok = blocks_take(&list, (uint32_t)atoi(argv[i + 1]),
                (uint32_t)atoi(argv[i + 2]), &start);
            printf("take %s at %u\n", ok ? "ok" : "refused", ok * start);
        }
        if (!strcmp(argv[i], "free"))
            blocks_free(&list, (uint32_t)atoi(argv[i + 1]));
        if (!strcmp(argv[i], "room"))
            printf("room %u\n", blocks_room(&list));
    }
    printf("used %u\n", list.used);
    return (0);
}
