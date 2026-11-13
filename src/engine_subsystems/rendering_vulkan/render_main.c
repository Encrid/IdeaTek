/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

render_module_t render_module;

BRIDGE_NATIVE(init)
{
    render_module = (render_module_t){};
    declare_types(api);
    add_runs(api);
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    free_objects();
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
