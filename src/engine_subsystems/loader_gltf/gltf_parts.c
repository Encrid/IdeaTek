/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

size_t count_of(const cgltf_data *data, uint32_t kind)
{
    const size_t counts[KINDS] = { data->meshes_count, data->materials_count,
        data->skins_count, data->animations_count, data->scenes_count };

    return (counts[kind]);
}

gltf_part_t *get_part(gltf_build_t *b, uint32_t kind, const char *label,
    uint32_t len)
{
    uint64_t hash = hash_text(label, len, 0);
    gltf_part_t *free_one = nullptr;
    gltf_part_t *p;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        p = &gltf_module.parts[k];
        if (p->file == b->file->entity && p->hash == hash)
            return (p);
        free_one = !free_one && !p->file ? p : free_one;
    }
    if (free_one)
        *free_one = (gltf_part_t){ .file = b->file->entity, .hash = hash,
            .kind = kind, .entity = b->api->spawn(b->api) };
    return (free_one);
}

static bool name_parts(gltf_build_t *b)
{
    char label[NAME_ROOM];
    uint32_t len;
    bool ok = true;

    for (uint32_t k = 0; k < KINDS; k++)
        for (size_t i = 0; ok && i < count_of(b->data, k); i++) {
            len = label_of(b->data, k, i, label);
            b->parts[k][i] = get_part(b, k, label, len);
            ok = b->parts[k][i] != nullptr;
        }
    b->fallback = nullptr;
    if (ok && needs_default(b->data))
        b->fallback = get_part(b, KIND_MATERIAL, "material/default", 16);
    return (ok && (b->fallback || !needs_default(b->data)));
}

bool plan_parts(gltf_build_t *b)
{
    if (!take_arrays(b)) {
        say_path(b->api, b->file->entity, ": no room for the parse");
        return (false);
    }
    if (!name_parts(b)) {
        say_path(b->api, b->file->entity, ": no room for its parts");
        return (false);
    }
    return (true);
}
