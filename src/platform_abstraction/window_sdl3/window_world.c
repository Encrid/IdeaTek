/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

uint64_t spawn_entity(const bridge_api_t *api)
{
    window_module.spawned++;
    return (api->spawn(api));
}

bool has_room(const bridge_api_t *api, uint32_t spawns, uint32_t bytes)
{
    const bridge_header_t *h = bridge_native_get_header(api);

    return (window_module.spawned + spawns <= WINDOW_SPAWNS
        && h->changes_left >= bytes);
}
