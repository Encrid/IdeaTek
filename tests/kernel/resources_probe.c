/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_files.h"
#include "ideatek_localization.h"
#include "ideatek_resources.h"

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
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_load_state_t", ideatek_load_state_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ResourceLoad", ideatek_resource_load_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:ResourceLoad", ideatek_resource_load_t);
    STRUCT(api, "ideatek:ResourceAdd", ideatek_resource_add_t);
    STRUCT(api, "ideatek:ResourceHandle", ideatek_resource_handle_t);
    STRUCT(api, "ideatek:Resource", ideatek_resource_t);
    STRUCT(api, "ideatek:ResourceLoader", ideatek_resource_loader_t);
    STRUCT(api, "ideatek:ResourceBytes", ideatek_resource_bytes_t);
    STRUCT(api, "ideatek:ResourceRead", ideatek_resource_read_t);
    STRUCT(api, "ideatek:ResourceOutcome", ideatek_resource_outcome_t);
    STRUCT(api, "ideatek:ResourceLabel", ideatek_resource_label_t);
    STRUCT(api, "ideatek:RawBytes", ideatek_raw_bytes_t);
    STRUCT(api, "ideatek:ResourceSource", ideatek_resource_source_t);
    STRUCT(api, "ideatek:ResourceSourceRead", ideatek_resource_source_read_t);
    STRUCT(api, "ideatek:ResourceSourceResult",
        ideatek_resource_source_result_t);
    TAG(api, "ideatek:ResourceReload");
    TAG(api, "ideatek:ResourceModified");
    TAG(api, "ideatek:ResourceReady");
    TAG(api, "ideatek:ResourceFailed");
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_load_state_t", ideatek_load_state_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
    STRUCT(api, "ideatek:ResourceHandle", ideatek_resource_handle_t);
    STRUCT(api, "ideatek:Resource", ideatek_resource_t);
    STRUCT(api, "ideatek:ResourceLabel", ideatek_resource_label_t);
    TAG(api, "ideatek:ResourceModified");
    TAG(api, "ideatek:ResourceReady");
    TAG(api, "ideatek:ResourceFailed");
    api->attach(api, id, STRUCT(api, "ideatek:ResourceAdd",
        ideatek_resource_add_t), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:ResourceReload"), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceLoad",
        ideatek_resource_load_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_loader(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_load_state_t", ideatek_load_state_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
    STRUCT(api, "ideatek:ResourceHandle", ideatek_resource_handle_t);
    STRUCT(api, "ideatek:Resource", ideatek_resource_t);
    STRUCT(api, "ideatek:ResourceBytes", ideatek_resource_bytes_t);
    STRUCT(api, "ideatek:RawBytes", ideatek_raw_bytes_t);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceLoad",
        ideatek_resource_load_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceAdd",
        ideatek_resource_add_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceRead",
        ideatek_resource_read_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceLabel",
        ideatek_resource_label_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceLoader",
        ideatek_resource_loader_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_source(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
    STRUCT(api, "ideatek:ResourceSourceRead", ideatek_resource_source_read_t);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceSourceResult",
        ideatek_resource_source_result_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceSource",
        ideatek_resource_source_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_storage_t", ideatek_storage_t);
    ENUM(api, "ideatek:ideatek_file_action_t", ideatek_file_action_t);
    ENUM(api, "ideatek:ideatek_file_status_t", ideatek_file_status_t);
    ENUM(api, "ideatek:ideatek_load_state_t", ideatek_load_state_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Locale", ideatek_locale_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    STRUCT(api, "ideatek:ResourceLoad", ideatek_resource_load_t);
    STRUCT(api, "ideatek:ResourceAdd", ideatek_resource_add_t);
    STRUCT(api, "ideatek:ResourceLoader", ideatek_resource_loader_t);
    STRUCT(api, "ideatek:ResourceLabel", ideatek_resource_label_t);
    STRUCT(api, "ideatek:ResourceSource", ideatek_resource_source_t);
    STRUCT(api, "ideatek:ResourceSourceResult",
        ideatek_resource_source_result_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:FileResult", ideatek_file_result_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:FileChange", ideatek_file_change_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:FileRequest",
        ideatek_file_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceHandle",
        ideatek_resource_handle_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Resource", ideatek_resource_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceRead",
        ideatek_resource_read_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RawBytes", ideatek_raw_bytes_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ResourceSourceRead",
        ideatek_resource_source_read_t), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:ResourceReload"), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:ResourceModified"), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:ResourceReady"), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:ResourceFailed"), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_LOADER)
    role_loader(api);
#elif defined(ROLE_SOURCE)
    role_source(api);
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
