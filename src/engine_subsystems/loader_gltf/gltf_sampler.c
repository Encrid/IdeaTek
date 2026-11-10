/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "gltf_inside.h"

static const char *filter_of(int gl, bool mip)
{
    bool linear = mip ? gl == 9986 || gl == 9987 : gl == 9729 || gl == 9985
        || gl == 9987;

    return (linear ? "linear" : "nearest");
}

static const char *wrap_of(int gl)
{
    if (gl == 33071)
        return ("clamp_to_edge");
    if (gl == 33648)
        return ("mirrored_repeat");
    return ("repeat");
}

static const cgltf_sampler *sampler_of(const cgltf_data *data,
    const cgltf_image *image)
{
    const cgltf_texture *t;

    for (size_t i = 0; i < data->textures_count; i++) {
        t = &data->textures[i];
        if (image_of(t) == image)
            return (t->sampler);
    }
    return (nullptr);
}

uint32_t sampler_settings(const cgltf_data *data, const cgltf_image *image,
    char *out)
{
    const cgltf_sampler *s = sampler_of(data, image);
    int minify;
    int magnify;

    out[0] = '\0';
    if (!s)
        return (0);
    minify = s->min_filter ? (int)s->min_filter : 9987;
    magnify = s->mag_filter ? (int)s->mag_filter : 9729;
    snprintf(out, NAME_ROOM, "min_filter = \"%s\"\nmag_filter = \"%s\"\n"
        "mip_filter = \"%s\"\nwrap_u = \"%s\"\nwrap_v = \"%s\"\n",
        filter_of(minify, false), filter_of(magnify, false),
        filter_of(minify, true), wrap_of(s->wrap_s), wrap_of(s->wrap_t));
    return ((uint32_t)strlen(out));
}

bool is_external(const cgltf_image *image)
{
    const char *uri = image->uri;
    size_t len = uri ? strlen(uri) : 0;
    bool ktx2 = len > 5 && !strcmp(uri + len - 5, ".ktx2");

    ktx2 |= image->mime_type && !strcmp(image->mime_type, "image/ktx2");
    return (uri && strncmp(uri, "data:", 5) && ktx2);
}
