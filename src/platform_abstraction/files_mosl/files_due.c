/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "files_inside.h"

static constexpr uint32_t MOST_ROUNDS = 100000;

static int by_seq(const void *a, const void *b)
{
    uint32_t x = files_module.jobs[*(const uint32_t *)a].seq;
    uint32_t y = files_module.jobs[*(const uint32_t *)b].seq;

    return ((x > y) - (x < y));
}

static void wait_due(const bridge_api_t *api)
{
    files_job_t *jobs = files_module.jobs;
    bool late;

    for (uint32_t i = 0; i < FILES_QUEUE; i++) {
        late = (jobs[i].state == FILES_WAITING)
            | (jobs[i].state == FILES_RUNNING);
        if (late && jobs[i].due <= files_module.round)
            bridge_native_say(api, BRIDGE_LOG_INFO, "round %u waits for the "
                "result of %llu", files_module.round,
                (unsigned long long)jobs[i].entity);
        while (late && jobs[i].due <= files_module.round) {
            os_mutex_unlock(files_module.lock);
            os_event_wait(files_module.finished, FILES_WAIT);
            os_mutex_lock(files_module.lock);
            late = (jobs[i].state == FILES_WAITING)
                | (jobs[i].state == FILES_RUNNING);
        }
    }
}

void take_jobs(const bridge_api_t *api, uint32_t bytes)
{
    uint32_t *done = files_module.done;
    uint32_t kept = 0;
    files_job_t *job;

    if (files_module.rounds) {
        wait_due(api);
        qsort(done, files_module.done_count, sizeof(*done), by_seq);
    }
    for (uint32_t k = 0; k < files_module.done_count; k++) {
        job = &files_module.jobs[done[k]];
        if (job->due <= files_module.round && has_room(api, bytes))
            publish_job(api, job);
        else
            done[kept++] = done[k];
    }
    files_module.done_count = kept;
}

bool take_rounds(const bridge_api_t *api, const files_settings_t *s)
{
    bool ok = s->deterministic_rounds <= MOST_ROUNDS;

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "deterministic_rounds: %u "
            "is outside 0 to %u", s->deterministic_rounds, MOST_ROUNDS);
    files_module.rounds = ok * s->deterministic_rounds;
    return (ok);
}
