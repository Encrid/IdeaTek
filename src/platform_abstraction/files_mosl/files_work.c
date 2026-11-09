/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "files_inside.h"

static const files_action_t ACTIONS[4] = { do_read, do_write, do_list,
    do_remove };

static uint64_t job_key(const files_job_t *job)
{
    uint64_t key = ((uint64_t)job->request.priority << 32)
        | (UINT32_MAX - job->round);

    return ((key + 1) * (job->state == FILES_WAITING));
}

static files_job_t *pick_job(bool *ended)
{
    files_job_t *jobs = files_module.jobs;
    uint64_t best_key = 0;
    uint64_t key;
    uint32_t at = 0;
    bool found = false;
    bool better;

    os_mutex_lock(files_module.lock);
    *ended = files_module.stop;
    for (uint32_t i = 0; i < FILES_QUEUE; i++) {
        key = job_key(&jobs[i]);
        better = key > best_key;
        best_key = better * key + !better * best_key;
        at = better * i + !better * at;
        found |= better;
    }
    found &= !files_module.stop;
    jobs[at].state = found * FILES_RUNNING + !found * jobs[at].state;
    os_mutex_unlock(files_module.lock);
    return (found ? &jobs[at] : nullptr);
}

static uint32_t run_job(files_job_t *job)
{
    const char *storages[2] = { files_module.content, files_module.user };

    return (ACTIONS[job->request.action & 3](job,
        storages[job->request.storage & 1]));
}

static void finish_job(files_job_t *job, uint32_t status)
{
    void *table[2] = { nullptr, nullptr };
    bool canceled;

    os_mutex_lock(files_module.lock);
    canceled = job->state == FILES_CANCELED;
    table[canceled] = job->data;
    free(table[1]);
    job->data = table[0];
    job->status = status;
    job->state = !canceled * FILES_DONE;
    files_module.done[files_module.done_count] = (uint32_t)(job
        - files_module.jobs);
    files_module.done_count += !canceled;
    os_mutex_unlock(files_module.lock);
}

void work_loop([[maybe_unused]] void *arg)
{
    files_job_t *job;
    bool ended = false;

    while (!ended) {
        job = pick_job(&ended);
        if (job)
            finish_job(job, run_job(job));
        else if (!ended)
            os_event_wait(files_module.wake, FILES_WAIT);
    }
}
