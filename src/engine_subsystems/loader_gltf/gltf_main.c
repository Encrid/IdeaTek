/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

gltf_module_t gltf_module;

BRIDGE_NATIVE(init)
{
    declare_types(api);
    declare_outputs(api);
    spawn_loader(api);
    add_observers(api);
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
