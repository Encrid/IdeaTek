/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static const char BASE64[] = ";base64,";

static bool decode(gltf_build_t *b, cgltf_buffer *buffer)
{
    cgltf_options o = parse_options();
    const char *at = strstr(buffer->uri, BASE64);

    if (at && cgltf_load_buffer_base64(&o, buffer->size, at + sizeof(BASE64)
        - 1, &buffer->data) == cgltf_result_success)
        return (true);
    say_path(b->api, b->file->entity, ": a buffer has no good base64 data");
    return (false);
}

static const void *bytes_of(gltf_build_t *b, const cgltf_buffer *buffer,
    size_t index, size_t *size)
{
    const gltf_dep_t *d;
    const gltf_cache_t *c;

    if (!buffer->uri) {
        *size = b->data->bin_size;
        return (b->data->bin);
    }
    d = find_dep_of(b->file->entity, DEP_BUFFER, index);
    c = find_cache(d->resource);
    *size = c->size;
    return (b->api->shared_base + c->at);
}

static bool point(gltf_build_t *b, cgltf_buffer *buffer, size_t index)
{
    size_t size = 0;
    const void *bytes = bytes_of(b, buffer, index, &size);

    if (size >= buffer->size) {
        buffer->data = (void *)bytes;
        return (true);
    }
    say_index(b->api, b->file->entity, ": the buffer %zu is shorter than its "
        "byteLength", index);
    return (false);
}

bool wire_buffers(gltf_build_t *b)
{
    cgltf_buffer *buffer;
    bool ok = true;

    for (size_t i = 0; ok && i < b->data->buffers_count; i++) {
        buffer = &b->data->buffers[i];
        if (buffer->uri && !strncmp(buffer->uri, "data:", 5))
            ok = decode(b, buffer);
        else
            ok = point(b, buffer, i);
    }
    return (ok);
}
