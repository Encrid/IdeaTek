/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resources_hash.h"

static res_slot_t slots[8];
static res_map_t map = { slots, 8, 0 };

static void run(const char *verb, const char *a, const char *b)
{
    uint64_t key = strtoull(a, nullptr, 10);
    uint32_t found;
    res_text_t path = { a, (uint32_t)strlen(a) };
    res_text_t type = { b, (uint32_t)strlen(b) };

    if (!strcmp(verb, "put"))
        printf("put %llu %s\n", (unsigned long long)key, map_put(&map, key,
            (uint32_t)atoi(b)) ? "ok" : "refused");
    if (!strcmp(verb, "find")) {
        found = map_find(&map, key);
        if (found == RES_NONE)
            printf("find %llu none\n", (unsigned long long)key);
        else
            printf("find %llu %u\n", (unsigned long long)key, found);
    }
    if (!strcmp(verb, "drop"))
        map_drop(&map, key);
    if (!strcmp(verb, "key"))
        printf("key %llx\n", (unsigned long long)res_hash_key(&path, &type));
    if (!strcmp(verb, "clear"))
        map_clear(&map);
}

int main(int argc, char **argv)
{
    for (int i = 1; i + 2 < argc; i += 3)
        run(argv[i], argv[i + 1], argv[i + 2]);
    printf("used %u\n", map.used);
    return (0);
}
