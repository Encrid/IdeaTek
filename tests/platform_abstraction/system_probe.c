/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_system.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_level(const bridge_api_t *api)
{
    ENUM(api, "ideatek:ideatek_message_level_t", ideatek_message_level_t);
}

[[maybe_unused]] static uint32_t declare_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:SystemState", ideatek_system_state_t));
}

[[maybe_unused]] static uint32_t declare_url(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:UrlRequest", ideatek_url_request_t));
}

[[maybe_unused]] static uint32_t declare_message(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MessageRequest", ideatek_message_request_t));
}

[[maybe_unused]] static bridge_text_t put(const bridge_api_t *api,
    const char *bytes, uint32_t len)
{
    uint32_t at = api->alloc(api, len);

    memcpy(api->shared_base + at, bytes, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    uint64_t entity = api->spawn(api);
    ideatek_url_request_t url = { .url = put(api, "https://example.org", 19) };
    ideatek_message_request_t message = { .title = put(api, "probe", 5),
        .message = put(api, "probe", 5),
        .level = IDEATEK_MESSAGE_LEVEL_INFORMATION };

    declare_level(api);
    BRIDGE_NATIVE_ATTACH(api, entity, declare_url(api), url);
    BRIDGE_NATIVE_ATTACH(api, entity, declare_message(api), message);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    ideatek_system_state_t state = { .locales = put(api, "fr-CA,en", 8) };

    declare_level(api);
    declare_url(api);
    declare_message(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_state(api), state);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_message(api);
    declare_level(api);
#elif defined(ROLE_GAME)
    ask(api);
#elif defined(ROLE_SYSTEM)
    report(api);
#elif defined(ROLE_READER)
    declare_state(api);
#else
    declare_level(api);
    declare_state(api);
    declare_url(api);
    declare_message(api);
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
