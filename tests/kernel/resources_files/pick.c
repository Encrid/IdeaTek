/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "resources_pick.h"

static res_text_t field(char **left)
{
    char *start = *left;
    char *semi = strchr(start, ';');
    res_text_t text = { start, 0 };

    if (semi) {
        *semi = '\0';
        *left = semi + 1;
    } else
        *left = start + strlen(start);
    text.len = (uint32_t)strlen(start);
    for (char *c = start; *c; c++)
        *c = *c == ',' ? '\n' : *c;
    return (text);
}

static void read_view(char *text, res_loader_view_t *view, uint32_t index)
{
    char *left = text;

    view->index = index;
    view->name = field(&left);
    view->type = field(&left);
    view->extensions = field(&left);
    view->magic = field(&left);
    view->ranged = left[0] == '1';
}

int main(int argc, char **argv)
{
    res_loader_view_t views[8];
    res_pick_ask_t ask = { .views = views };
    res_pick_t pick;

    if (argc < 6)
        return (2);
    ask.type = (res_text_t){ argv[1], (uint32_t)strlen(argv[1]) };
    ask.extension = (res_text_t){ argv[2], (uint32_t)strlen(argv[2]) };
    ask.name = (res_text_t){ argv[3], (uint32_t)strlen(argv[3]) };
    ask.has_bytes = argv[4][0] == '1';
    ask.bytes = (res_text_t){ argv[5], (uint32_t)strlen(argv[5]) };
    for (int i = 6; i < argc && ask.count < 8; i++)
        read_view(argv[i], &views[ask.count++], (uint32_t)(i - 6));
    pick_loader(&ask, &pick);
    if (pick.loader == RES_NONE)
        printf("pick none error %d needs %d\n", (int)pick.error,
            pick.needs_bytes);
    else
        printf("pick %u error %d needs %d\n", pick.loader, (int)pick.error,
            pick.needs_bytes);
    return (0);
}
