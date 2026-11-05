/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_resources.h"
#include "ideatek_localization.h"

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
    ENUM(api, "ideatek:ideatek_text_syntax_t", ideatek_text_syntax_t);
    ENUM(api, "ideatek:ideatek_text_arg_t", ideatek_text_arg_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:StringTable", ideatek_string_table_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Locale", ideatek_locale_t);
    STRUCT(api, "ideatek:StringTable", ideatek_string_table_t);
    STRUCT(api, "ideatek:TextKey", ideatek_text_key_t);
    STRUCT(api, "ideatek:LocalizedText", ideatek_localized_text_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:LocalizedText", ideatek_localized_text_t);
    api->attach(api, id, STRUCT(api, "ideatek:Locale", ideatek_locale_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:TextKey", ideatek_text_key_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_loader(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_text_syntax_t", ideatek_text_syntax_t);
    api->attach(api, id, STRUCT(api, "ideatek:StringTable",
        ideatek_string_table_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_text_syntax_t", ideatek_text_syntax_t);
    STRUCT(api, "ideatek:Locale", ideatek_locale_t);
    STRUCT(api, "ideatek:StringTable", ideatek_string_table_t);
    STRUCT(api, "ideatek:TextKey", ideatek_text_key_t);
    api->attach(api, id, STRUCT(api, "ideatek:LocalizedText",
        ideatek_localized_text_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api,
        "ideatek:ResourceVariable", ideatek_resource_variable_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_LOADER)
    role_loader(api);
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
