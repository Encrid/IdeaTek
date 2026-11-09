/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "states_chain.h"

static const char *const WORDS[] = { "stays", "leaves", "deep" };

static void read_link(const char *text, chain_link_t *link)
{
    unsigned has_state = 0;
    unsigned has_request = 0;
    unsigned scoped = 0;

    sscanf(text, "%u,%u,%u,%u,%u", &has_state, &has_request, &scoped,
        &link->state, &link->value);
    link->has_state = has_state != 0;
    link->has_request = has_request != 0;
    link->scoped = scoped != 0;
}

int main(int argc, char **argv)
{
    chain_link_t links[16] = {};
    uint32_t count = (uint32_t)(argc - 1);

    if (count > 16)
        return (2);
    for (uint32_t i = 0; i < count; i++)
        read_link(argv[i + 1], &links[i]);
    printf("verdict %s\n", WORDS[chain_verdict(links, count)]);
    return (0);
}
