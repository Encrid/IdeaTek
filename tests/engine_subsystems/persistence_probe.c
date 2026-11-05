/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_changes.h"
#include "ideatek_persistence.h"

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
    ENUM(api, "ideatek:ideatek_save_action_t", ideatek_save_action_t);
    ENUM(api, "ideatek:ideatek_save_state_t", ideatek_save_state_t);
    ENUM(api, "ideatek:ideatek_save_error_t", ideatek_save_error_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:SaveRequest", ideatek_save_request_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:SaveRequest", ideatek_save_request_t);
    STRUCT(api, "ideatek:Saved", ideatek_saved_t);
    STRUCT(api, "ideatek:SaveReady", ideatek_save_ready_t);
    STRUCT(api, "ideatek:SaveRefuse", ideatek_save_refuse_t);
    STRUCT(api, "ideatek:SaveResult", ideatek_save_result_t);
    STRUCT(api, "ideatek:SaveInfo", ideatek_save_info_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_save_action_t", ideatek_save_action_t);
    api->attach(api, id, STRUCT(api, "ideatek:SaveRequest",
        ideatek_save_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Saved", ideatek_saved_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_participant(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_save_action_t", ideatek_save_action_t);
    STRUCT(api, "ideatek:SaveRequest", ideatek_save_request_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:RowsIn", ideatek_rows_in_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:EntityId", ideatek_entity_id_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:SaveReady", ideatek_save_ready_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SaveRefuse",
        ideatek_save_refuse_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_saver(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_durability_t", ideatek_durability_t);
    ENUM(api, "ideatek:ideatek_change_condition_t", ideatek_change_condition_t);
    ENUM(api, "ideatek:ideatek_save_action_t", ideatek_save_action_t);
    ENUM(api, "ideatek:ideatek_save_state_t", ideatek_save_state_t);
    ENUM(api, "ideatek:ideatek_save_error_t", ideatek_save_error_t);
    STRUCT(api, "ideatek:SaveRequest", ideatek_save_request_t);
    STRUCT(api, "ideatek:Saved", ideatek_saved_t);
    STRUCT(api, "ideatek:SaveReady", ideatek_save_ready_t);
    STRUCT(api, "ideatek:SaveRefuse", ideatek_save_refuse_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ChangeTag", ideatek_change_tag_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:RowChanges", ideatek_row_changes_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:EntityId", ideatek_entity_id_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:RowsIn",
        ideatek_rows_in_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SaveResult",
        ideatek_save_result_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SaveInfo", ideatek_save_info_t),
        BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_PARTICIPANT)
    role_participant(api);
#elif defined(ROLE_SAVER)
    role_saver(api);
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
