/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

int bridge_reload(const void *api);
int bridge_shutdown(const void *api);

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "reload"))
        printf("reload %d\n", bridge_reload(nullptr));
    if (argc > 1 && !strcmp(argv[1], "shutdown"))
        printf("shutdown %d\n", bridge_shutdown(nullptr));
    return (0);
}
