/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "files_inside.h"

files_job_t *free_job(void)
{
    for (uint32_t i = 0; i < FILES_QUEUE; i++)
        if (files_module.jobs[i].state == FILES_FREE)
            return (&files_module.jobs[i]);
    return (nullptr);
}

void put_early(const bridge_api_t *api, uint64_t entity, uint32_t status,
    bool keep)
{
    files_early_t early = { .entity = entity, .status = status,
        .keep = keep };

    if (files_module.early_count == FILES_EARLY) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no result for %llu: %u results wait for the next round",
            (unsigned long long)entity, FILES_EARLY);
        return;
    }
    files_module.early[files_module.early_count++] = early;
}

static void drop_early(uint64_t entity)
{
    uint32_t kept = 0;

    for (uint32_t i = 0; i < files_module.early_count; i++) {
        files_module.early[kept] = files_module.early[i];
        kept += files_module.early[i].entity != entity;
    }
    files_module.early_count = kept;
}

void cancel_job(uint64_t entity)
{
    files_job_t *job;
    uint32_t state;
    bool hit;

    for (uint32_t i = 0; i < FILES_QUEUE; i++) {
        job = &files_module.jobs[i];
        state = job->state;
        hit = (job->entity == entity) & (state != FILES_FREE);
        job->state = hit * (state >= FILES_RUNNING) * FILES_CANCELED
            + !hit * state;
    }
    drop_early(entity);
    stop_watch(entity);
}
