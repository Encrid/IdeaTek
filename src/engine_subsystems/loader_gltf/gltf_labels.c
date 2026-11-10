/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "gltf_inside.h"

static const char *const KIND_NAMES[KINDS] = { "mesh", "material", "skin",
    "clip", "scene" };

static const char *name_at(const cgltf_data *d, uint32_t kind, size_t i)
{
    const char *name = nullptr;

    switch (kind) {
        case KIND_MESH:
            name = d->meshes[i].name;
            break;
        case KIND_MATERIAL:
            name = d->materials[i].name;
            break;
        case KIND_SKIN:
            name = d->skins[i].name;
            break;
        case KIND_CLIP:
            name = d->animations[i].name;
            break;
        default:
            name = d->scenes[i].name;
    }
    return (name && name[0] ? name : nullptr);
}

static bool is_shared(const cgltf_data *d, uint32_t kind, size_t index,
    const char *name)
{
    const char *other;

    for (size_t j = 0; j < count_of(d, kind); j++) {
        other = j == index ? nullptr : name_at(d, kind, j);
        if (other && !strcmp(other, name))
            return (true);
    }
    return (false);
}

uint32_t label_of(const cgltf_data *data, uint32_t kind, size_t index,
    char *out)
{
    const char *name = name_at(data, kind, index);
    const char *kind_name = KIND_NAMES[kind];

    if (!name)
        snprintf(out, NAME_ROOM, "%s/%zu", kind_name, index);
    else if (is_shared(data, kind, index, name))
        snprintf(out, NAME_ROOM, "%s/%s.%zu", kind_name, name, index);
    else
        snprintf(out, NAME_ROOM, "%s/%s", kind_name, name);
    return ((uint32_t)strlen(out));
}
