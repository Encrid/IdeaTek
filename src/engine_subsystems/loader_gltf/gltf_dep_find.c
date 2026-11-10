/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

gltf_dep_t *find_dep(uint64_t use)
{
    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++)
        if (gltf_module.deps[k].use == use)
            return (&gltf_module.deps[k]);
    return (nullptr);
}

gltf_dep_t *find_dep_of(uint64_t file, uint32_t kind, size_t index)
{
    gltf_dep_t *d;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (d->file == file && d->kind == kind && d->index == index)
            return (d);
    }
    return (nullptr);
}
