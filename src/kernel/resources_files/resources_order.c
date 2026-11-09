/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static uint32_t best_waiting(void)
{
    const res_resource_t *r;
    uint32_t best = RES_NONE;
    uint8_t top = 0;
    bool better;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        better = r->entity && r->step == STEP_READ
            && (best == RES_NONE || r->priority > top);
        best = better ? i : best;
        top = better ? r->priority : top;
    }
    return (best);
}

void start_reads(const bridge_api_t *api)
{
    uint32_t best = best_waiting();
    res_resource_t *r;

    while (best != RES_NONE && start_read(api, best, READ_FILE)) {
        r = &res_module.resources[best];
        r->step = r->step == STEP_READ ? STEP_READING : r->step;
        best = best_waiting();
    }
}
