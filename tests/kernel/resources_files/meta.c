/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "resources_meta.h"

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
    printf("meta ok action %d loader [%.*s] settings [%.*s]\n",
        (int)meta.action, (int)meta.loader.len, meta.loader.at,
        (int)meta.settings.len, meta.settings.at);
    return (0);
}
