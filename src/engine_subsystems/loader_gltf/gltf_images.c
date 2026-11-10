/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

const cgltf_image *image_of(const cgltf_texture *t)
{
    return (t->image ? t->image : t->basisu_image);
}

static bool has_second_sampler(const cgltf_data *data, size_t index)
{
    const cgltf_image *image = &data->images[index];
    const cgltf_sampler *first = nullptr;
    bool seen = false;

    for (size_t i = 0; i < data->textures_count; i++) {
        if (image_of(&data->textures[i]) != image)
            continue;
        if (seen && data->textures[i].sampler != first)
            return (true);
        first = seen ? first : data->textures[i].sampler;
        seen = true;
    }
    return (false);
}

void say_images(gltf_build_t *b)
{
    uint64_t id = b->file->entity;

    for (size_t i = 0; i < b->data->images_count; i++) {
        if (!is_external(&b->data->images[i]))
            say_index(b->api, id, ": the image %zu is not a KTX 2.0 file of "
                "its own", i);
        else if (has_second_sampler(b->data, i))
            say_index(b->api, id, ": the image %zu has a second sampler, "
                "which has no effect", i);
    }
}
