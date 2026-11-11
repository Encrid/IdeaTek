/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "files_inside.h"

void publish(const bridge_api_t *api, uint64_t entity,
    ideatek_file_result_t result, bool keep)
{
    BRIDGE_NATIVE_ATTACH(api, entity, RESULT, result);
    files_module.gone[files_module.gone_count] = entity;
    files_module.gone_count += !keep;
}

void publish_job(const bridge_api_t *api, files_job_t *job)
{
    files_half_t *half = &files_module.half;
    bool canceled = job->state == FILES_CANCELED;
    bool fits = job->data_len <= half->size - half->used;
    uint32_t status = fits ? job->status : IDEATEK_FILE_STATUS_TOO_BIG;
    bool ok = status == IDEATEK_FILE_STATUS_OK;
    ideatek_file_result_t result = { .size = job->size,
        .status = (ideatek_file_status_t)status };

    result.data = put_bytes(api, job->data ? job->data : "",
        (fits & !canceled & ok) * job->data_len);
    if (!canceled)
        publish(api, job->entity, result, false);
    free(job->data);
    job->data = nullptr;
    job->state = FILES_FREE;
}

static void take_results(const bridge_api_t *api)
{
    files_early_t *early = files_module.early;
    uint32_t bytes = CHANGE_BYTES(ideatek_file_result_t);
    ideatek_file_result_t result = {};
    uint32_t i = 0;

    os_mutex_lock(files_module.lock);
    for (; i < files_module.early_count && early[i].due <= files_module.round
        && has_room(api, bytes); i++) {
        result.status = (ideatek_file_status_t)early[i].status;
        publish(api, early[i].entity, result, early[i].keep);
    }
    memmove(early, early + i, (files_module.early_count - i) * sizeof(*early));
    files_module.early_count -= i;
    take_jobs(api, bytes);
    os_mutex_unlock(files_module.lock);
}

BRIDGE_NATIVE(results)
{
    if (!files_module.started)
        start_files(api);
    if (files_module.broken)
        return (1);
    files_module.round++;
    turn_results(api);
    take_results(api);
    poll_watches(api);
    return (0);
}
