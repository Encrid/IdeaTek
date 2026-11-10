/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "space_pure.h"

static uint64_t slots[64];

int main(int argc, char **argv)
{
    space_set_t set;
    uint64_t id;

    if (argc < 2)
        return (0);
    start_set(&set, slots, (uint32_t)atoi(argv[1]));
    for (int i = 2; i + 1 < argc; i += 2) {
        id = strtoull(argv[i + 1], nullptr, 10);
        if (!strcmp(argv[i], "put"))
            printf("put %llu %s\n", (unsigned long long)id,
                set_put(&set, id) ? "ok" : "full");
        if (!strcmp(argv[i], "has"))
            printf("has %llu %d\n", (unsigned long long)id, set_has(&set, id));
    }
    printf("count %u\n", set.count);
    return (0);
}
