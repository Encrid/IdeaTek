/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "resources_meta.h"

static void show(const char *what, const res_text_t *text)
{
    printf("%s [", what);
    for (uint32_t i = 0; i < text->len; i++)
        putchar(text->at[i] == '\n' ? '|' : text->at[i]);
    printf("]");
}

int main(int argc, char **argv)
{
    char text[4096] = "";
    res_text_t bytes = { text, 0 };
    res_meta_t meta;

    if (argc > 1)
        memcpy(text, argv[1], strlen(argv[1]) < 4095 ? strlen(argv[1]) + 1
            : 4095);
    for (char *c = text; *c; c++)
        *c = *c == '|' ? '\n' : *c;
    bytes.len = (uint32_t)strlen(text);
    if (!meta_read(&bytes, &meta)) {
        printf("meta refused line %u\n", meta.line);
        return (0);
    }
    printf("meta ok action %d", (int)meta.action);
    show(" loader", &meta.loader);
    show(" settings", &meta.settings);
    printf("\n");
    return (0);
}
