/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "files_inside.h"

static bool copy_path(const bridge_api_t *api, bridge_text_t text, char *out)
{
    bool fits = text.len < FILES_PATH;
    uint32_t len = fits * text.len;

    memcpy(out, api->shared_base + text.at, len);
    out[len] = '\0';
    return (fits);
}

static uint32_t judge(const ideatek_file_request_t *r, const char *path)
{
    bool content = r->storage == IDEATEK_STORAGE_CONTENT;
    bool user = r->storage == IDEATEK_STORAGE_USER;
    bool changes = (r->action == IDEATEK_FILE_ACTION_WRITE)
        | (r->action == IDEATEK_FILE_ACTION_REMOVE);
    bool watch = r->action == IDEATEK_FILE_ACTION_WATCH;
    bool known = r->action <= IDEATEK_FILE_ACTION_WATCH;
    bool denied = !path_is_allowed(path) | !known | !(content | user)
        | (content & changes) | (user & watch);
    bool missing = content & !files_module.has_content;
    bool unsupported = user & !files_module.has_user;

    if (denied)
        return (IDEATEK_FILE_STATUS_DENIED);
    if (missing)
        return (IDEATEK_FILE_STATUS_NOT_FOUND);
    if (unsupported)
        return (IDEATEK_FILE_STATUS_UNSUPPORTED);
    return (IDEATEK_FILE_STATUS_OK);
}

static void fill_job(const bridge_api_t *api, files_job_t *job,
    const ideatek_file_request_t *r, const char *path)
{
    job->request = *r;
    job->bytes = api->shared_base + r->data.at;
    memcpy(job->path, path, strlen(path) + 1);
    job->data = nullptr;
    job->data_len = 0;
    job->size = 0;
    job->status = IDEATEK_FILE_STATUS_OK;
    job->round = files_module.round;
    job->state = FILES_WAITING;
}

static void queue_job(const bridge_api_t *api, uint64_t entity)
{
    uint32_t at = bridge_native_find_value(api, entity, REQUEST);
    const ideatek_file_request_t *r = (const void *)(api->shared_base + at);
    char path[FILES_PATH];
    bool fits = copy_path(api, r->path, path);
    uint32_t status = fits ? judge(r, path) : IDEATEK_FILE_STATUS_DENIED;
    bool watch = r->action == IDEATEK_FILE_ACTION_WATCH;
    files_job_t *job = nullptr;

    if (status == IDEATEK_FILE_STATUS_OK && watch)
        status = start_watch(entity, path);
    else if (status == IDEATEK_FILE_STATUS_OK) {
        job = free_job();
        status = job ? status : IDEATEK_FILE_STATUS_FAILED;
    }
    if (job) {
        job->entity = entity;
        fill_job(api, job, r, path);
    } else
        put_early(api, entity, status);
}

BRIDGE_NATIVE(request)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        files_module.request_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);

    os_mutex_lock(files_module.lock);
    for (uint32_t i = 0; i < list->count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE)
            cancel_job(t[i].id);
        else
            queue_job(api, t[i].id);
    }
    os_mutex_unlock(files_module.lock);
    os_event_set(files_module.wake);
    return (0);
}
