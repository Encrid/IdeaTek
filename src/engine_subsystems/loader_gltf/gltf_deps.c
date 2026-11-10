/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static gltf_dep_t *match_dep(uint64_t file, uint64_t hash)
{
    gltf_dep_t *free_one = nullptr;
    gltf_dep_t *d;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (d->file == file && d->hash == hash)
            return (d);
        free_one = !free_one && !d->file ? d : free_one;
    }
    return (free_one);
}

bool plan_one(gltf_deps_t *p, uint32_t kind, const char *uri, uint32_t index)
{
    char *path = p->room;
    uint32_t len = join_path(p->folder, p->folder_len, uri, path);
    gltf_dep_t *d = match_dep(p->f->entity, hash_text(path, len, kind + 1));

    if (!d)
        return (false);
    p->room += PATH_ROOM;
    if (!d->file) {
        *d = (gltf_dep_t){ .file = p->f->entity, .kind = kind,
            .hash = hash_text(path, len, kind + 1) };
        p->news[p->count++] = (gltf_new_t){ d, path, len, p->settings,
            p->settings_len };
    }
    d->kept = true;
    d->index = index;
    return (true);
}

static void find_folder(gltf_deps_t *p)
{
    uint32_t len = 0;

    p->folder = path_of(p->api, p->f->entity, &len);
    while (len && p->folder[len - 1] != '/')
        len--;
    p->folder_len = len;
}

static bool plan_images(gltf_deps_t *p, const cgltf_data *data)
{
    bool ok = true;

    for (size_t i = 0; ok && i < data->images_count; i++) {
        if (!is_external(&data->images[i]))
            continue;
        p->settings = p->room;
        p->settings_len = sampler_settings(data, &data->images[i], p->room);
        p->room += NAME_ROOM;
        ok = plan_one(p, DEP_IMAGE, data->images[i].uri, (uint32_t)i);
    }
    return (ok);
}

bool plan_deps(const bridge_api_t *api, gltf_file_t *f, cgltf_data *data)
{
    size_t count = data->buffers_count + data->images_count;
    gltf_deps_t p = { .api = api, .f = f, .settings = "",
        .news = parse_take(count * (sizeof(gltf_new_t) + PATH_ROOM
        + NAME_ROOM) + 16) };
    bool ok = p.news != nullptr;
    const char *uri;

    p.room = ok ? (char *)(p.news + count) : nullptr;
    find_folder(&p);
    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++)
        gltf_module.deps[k].kept &= gltf_module.deps[k].file != f->entity;
    for (size_t i = 0; ok && i < data->buffers_count; i++) {
        uri = data->buffers[i].uri;
        if (uri && strncmp(uri, "data:", 5))
            ok = plan_one(&p, DEP_BUFFER, uri, (uint32_t)i);
    }
    ok = ok && plan_images(&p, data);
    drop_deps(api, f);
    return (ok && spawn_deps(&p));
}
