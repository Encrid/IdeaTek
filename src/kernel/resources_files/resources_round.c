/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static void step_all(const bridge_api_t *api)
{
    const res_resource_t *r;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        if (r->entity && r->step == STEP_NEW)
            step_resource(api, i);
    }
}

BRIDGE_NATIVE(round)
{
    if (!res_module.started)
        start_resources(api);
    if (res_module.broken)
        return (1);
    res_module.round++;
    turn_tags(api);
    tick_time(api);
    take_results(api);
    place_uses(api);
    place_labels(api);
    start_reloads(api);
    step_all(api);
    start_reads(api);
    start_ranges(api);
    finish_loads(api);
    check_owners(api);
    free_dead(api);
    fold_states(api);
    return (0);
}
