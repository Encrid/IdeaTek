/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sockets_values.h"

static void add_word(char *line, const char *word)
{
    strcat(line, *line ? " " : "");
    strcat(line, word);
}

int main(int argc, char **argv)
{
    uint8_t base[256];
    uint8_t bytes[256];
    sockets_ring_t ring = { .size = argc > 1 ? (uint32_t)atoi(argv[1]) : 0 };
    const uint8_t *taken;
    char line[1024] = "";
    char word[16];

    if (argc < 3 || ring.size > sizeof(base))
        return (2);
    memset(bytes, 'x', sizeof(bytes));
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == 'p')
            add_word(line, ring_put(base, &ring, bytes,
                (uint32_t)atoi(argv[i] + 1)) ? "ok" : "full");
        else {
            snprintf(word, sizeof(word), "%u", ring_take(base, &ring,
                &taken));
            add_word(line, word);
        }
    }
    printf("%s\n", line);
    return (0);
}
