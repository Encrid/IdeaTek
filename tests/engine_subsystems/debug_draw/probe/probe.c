/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_debug_draw.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    ENUM(api, "ideatek:ideatek_debug_kind_t", ideatek_debug_kind_t);
    MASK(api, "ideatek:ideatek_debug_flags_t", ideatek_debug_flags_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:DebugDrawUngranted", ideatek_debug_draw_t);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:DebugDraw", ideatek_debug_draw_t);
    STRUCT(api, "ideatek:DebugView", ideatek_debug_view_t);
}

[[maybe_unused]] static void role_drawer(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:DebugView", ideatek_debug_view_t);
    api->attach(api, id, STRUCT(api, "ideatek:DebugDraw", ideatek_debug_draw_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    api->attach(api, id, STRUCT(api, "ideatek:DebugView", ideatek_debug_view_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:DebugDraw", ideatek_debug_draw_t);
    STRUCT(api, "ideatek:DebugView", ideatek_debug_view_t);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_DRAWER)
    role_drawer(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_MODULE)
    role_module(api);
#else
    declare_all(api);
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
