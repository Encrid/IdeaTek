/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "resources_fold.h"

int main(int argc, char **argv)
{
    res_fold_t fold;
    unsigned state = 0;
    unsigned error = 0;

    fold_start(&fold);
    for (int i = 1; i < argc; i++) {
        sscanf(argv[i], "%u,%u", &state, &error);
        fold_add(&fold, (ideatek_load_state_t)state,
            (ideatek_resource_error_t)error);
    }
    printf("fold state %d error %d count %u\n", (int)fold_state(&fold),
        (int)fold.error, fold.count);
    return (0);
}
