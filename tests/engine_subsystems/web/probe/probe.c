/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_web.h"

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
    ENUM(api, "ideatek:ideatek_web_method_t", ideatek_web_method_t);
    ENUM(api, "ideatek:ideatek_web_state_t", ideatek_web_state_t);
    ENUM(api, "ideatek:ideatek_web_error_t", ideatek_web_error_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:WebRequest", ideatek_web_request_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:WebRequest", ideatek_web_request_t);
    STRUCT(api, "ideatek:WebResponse", ideatek_web_response_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_web_method_t", ideatek_web_method_t);
    api->attach(api, id, STRUCT(api, "ideatek:WebRequest",
        ideatek_web_request_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_web_method_t", ideatek_web_method_t);
    ENUM(api, "ideatek:ideatek_web_state_t", ideatek_web_state_t);
    ENUM(api, "ideatek:ideatek_web_error_t", ideatek_web_error_t);
    STRUCT(api, "ideatek:WebRequest", ideatek_web_request_t);
    api->attach(api, id, STRUCT(api, "ideatek:WebResponse",
        ideatek_web_response_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
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
