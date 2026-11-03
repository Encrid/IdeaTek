/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_files.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

static void declare_lists(const bridge_api_t *api)
{
    ENUM(api, "ideatek:ideatek_storage_t", ideatek_storage_t);
    ENUM(api, "ideatek:ideatek_file_action_t", ideatek_file_action_t);
    ENUM(api, "ideatek:ideatek_file_status_t", ideatek_file_status_t);
}

static uint32_t declare_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:FileRequest", ideatek_file_request_t));
}

static uint32_t declare_result(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:FileResult", ideatek_file_result_t));
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    uint32_t at = api->alloc(api, 9);
    ideatek_file_request_t request = { .path = { .at = at, .len = 9 },
        .storage = IDEATEK_STORAGE_USER, .action = IDEATEK_FILE_ACTION_READ };

    memcpy(api->shared_base + at, "probe.txt", 9);
    declare_lists(api);
    declare_result(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_request(api), request);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    ideatek_file_result_t result = { .status = IDEATEK_FILE_STATUS_NOT_FOUND };

    declare_lists(api);
    declare_request(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_result(api), result);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_request(api);
    declare_lists(api);
#elif defined(ROLE_GAME)
    ask(api);
#elif defined(ROLE_FILES)
    report(api);
#else
    declare_lists(api);
    declare_request(api);
    declare_result(api);
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
