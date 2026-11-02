/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_window.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_enums(const bridge_api_t *api)
{
    ENUM(api, "ideatek:ideatek_place_t", ideatek_place_t);
    ENUM(api, "ideatek:ideatek_window_mode_t", ideatek_window_mode_t);
    ENUM(api, "ideatek:ideatek_surface_t", ideatek_surface_t);
}

[[maybe_unused]] static uint32_t declare_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowRequest", ideatek_window_request_t));
}

[[maybe_unused]] static uint32_t declare_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowState", ideatek_window_state_t));
}

[[maybe_unused]] static uint32_t declare_main(const bridge_api_t *api)
{
    return (TAG(api, "ideatek:MainWindow"));
}

[[maybe_unused]] static void open_window(const bridge_api_t *api)
{
    ideatek_window_request_t request = {
        .title = "IdeaTek", .width = 1280, .height = 720,
        .place = IDEATEK_PLACE_CENTER, .resizable = true
    };
    uint64_t id = api->spawn(api);

    declare_enums(api);
    BRIDGE_NATIVE_ATTACH(api, id, declare_request(api), request);
    api->attach(api, id, declare_main(api), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_request(api);
    declare_enums(api);
#elif defined(ROLE_GAME)
    open_window(api);
#elif defined(ROLE_WINDOW)
    declare_enums(api);
    declare_request(api);
    declare_main(api);
    declare_state(api);
#elif defined(ROLE_READER)
    declare_enums(api);
    declare_state(api);
#else
    declare_enums(api);
    declare_request(api);
    declare_state(api);
    declare_main(api);
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
