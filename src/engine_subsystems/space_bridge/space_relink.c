/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void relink_all(const bridge_api_t *api)
{
    place_list(api, LIST_RELINK);
    publish_moves(api);
}

BRIDGE_NATIVE(relink)
{
    return (run_started(api, relink_all));
}
