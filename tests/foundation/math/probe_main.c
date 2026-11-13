/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "probe.h"

static const probe_entry_t *find_entry(const char *name)
{
    const probe_group_t *groups[] = { &SCALAR_PROBES, &ROOT_PROBES,
        &VEC3_PROBES, &VEC3_SHAPE_PROBES, &VEC3_NUMBER_PROBES, &VEC4_PROBES,
        &TRIG_PROBES, &DISTANCE_PROBES, &QUATD_PROBES, &QUATF_PROBES,
        &MAT3D_PROBES, &MAT3F_PROBES, &PLACE_PROBES,
        &MAT4_PROBES };

    for (size_t g = 0; g < sizeof(groups) / sizeof(groups[0]); g++)
        for (size_t i = 0; i < groups[g]->count; i++)
            if (!strcmp(groups[g]->entries[i].name, name))
                return (&groups[g]->entries[i]);
    return (nullptr);
}

int main(int argc, char **argv)
{
    const probe_entry_t *entry = argc > 1 ? find_entry(argv[1]) : nullptr;

    if (!entry || argc - 2 != entry->count) {
        printf("usage: math_probe <function> <values>\n");
        return (2);
    }
    printf("%s", entry->name);
    entry->run(argv + 2);
    printf("\n");
    return (0);
}
