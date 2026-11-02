/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#if defined(ROLE_TABLE)
    #define STORAGE BRIDGE_TABLE
#else
    #define STORAGE BRIDGE_SINGLETON
#endif

#define DECLARE(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, STORAGE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void start(const bridge_api_t *api)
{
    ideatek_frame_t frame = { .index = 1 };
    ideatek_tick_t tick = { .step = 1.0 / 60.0 };

    BRIDGE_NATIVE_ATTACH(api, 0, DECLARE(api, "ideatek:Frame",
        ideatek_frame_t), frame);
    BRIDGE_NATIVE_ATTACH(api, 0, DECLARE(api, "ideatek:Tick",
        ideatek_tick_t), tick);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_CLOCK)
    start(api);
#else
    DECLARE(api, "ideatek:Frame", ideatek_frame_t);
    DECLARE(api, "ideatek:Tick", ideatek_tick_t);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
