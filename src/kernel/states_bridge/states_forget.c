/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_inside.h"

static void drop_change(const bridge_api_t *api, uint64_t id,
    [[maybe_unused]] uint32_t table, [[maybe_unused]] uint32_t row)
{
    api->detach(api, id, CHANGE);
}

BRIDGE_NATIVE(forget)
{
    walk_query(api, states_module.forget, drop_change);
    return (0);
}
