/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "files_inside.h"

static const ideatek_file_result_t STOPPED = {
    .status = IDEATEK_FILE_STATUS_NOT_FOUND };

uint32_t start_watch(uint64_t entity, const char *path)
{
    char full[FILES_PATH];
    files_watch_t *w;

    if (files_module.watch_count == FILES_WATCHES
        || !path_join(full, sizeof(full), files_module.content, path))
        return (IDEATEK_FILE_STATUS_FAILED);
    w = &files_module.watches[files_module.watch_count];
    w->watch = os_watch_start(full, FILES_DEPTH);
    if (!w->watch)
        return (IDEATEK_FILE_STATUS_FAILED);
    w->entity = entity;
    w->changed = false;
    memcpy(w->path, path, strlen(path) + 1);
    files_module.watch_count++;
    return (IDEATEK_FILE_STATUS_OK);
}

void stop_watch(uint64_t entity)
{
    uint32_t kept = 0;
    bool hit;

    for (uint32_t i = 0; i < files_module.watch_count; i++) {
        hit = files_module.watches[i].entity == entity;
        if (hit)
            os_watch_stop(files_module.watches[i].watch);
        files_module.watches[kept] = files_module.watches[i];
        kept += !hit;
    }
    files_module.watch_count = kept;
}

static int take_lines(const bridge_api_t *api, files_watch_t *w)
{
    files_half_t *half = &files_module.half;
    char line[FILES_PATH];
    const char *name = "";
    const char *names[3] = { "", "", "" };
    bool room = half->size - half->used >= FILES_PATH;
    int got = room ? os_watch_take(w->watch, &name) : 0;
    uint32_t len;

    while (got > 0) {
        names[1] = name;
        len = path_join(line, sizeof(line), w->path, names[got & 1])
            * (uint32_t)strlen(line);
        line[len] = '\n';
        put_bytes(api, line, len + 1);
        room = half->size - half->used >= FILES_PATH;
        got = room ? os_watch_take(w->watch, &name) : 0;
    }
    return (got);
}

static bool poll_watch(const bridge_api_t *api, files_watch_t *w)
{
    files_half_t *half = &files_module.half;
    uint32_t start = half->used;
    int got = take_lines(api, w);
    ideatek_file_change_t change = { .paths = { .at = half->at
        + half->side * half->size + start, .len = half->used - start } };

    if (w->changed)
        api->detach(api, w->entity, CHANGE);
    if (change.paths.len)
        BRIDGE_NATIVE_ATTACH(api, w->entity, CHANGE, change);
    w->changed = change.paths.len > 0;
    if (got == -1) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "the watch of %s stopped",
            w->path);
        BRIDGE_NATIVE_ATTACH(api, w->entity, RESULT, STOPPED);
        os_watch_stop(w->watch);
    }
    return (got != -1);
}

void poll_watches(const bridge_api_t *api)
{
    uint32_t kept = 0;
    bool alive;

    for (uint32_t i = 0; i < files_module.watch_count; i++) {
        alive = poll_watch(api, &files_module.watches[i]);
        files_module.watches[kept] = files_module.watches[i];
        kept += alive;
    }
    files_module.watch_count = kept;
}
