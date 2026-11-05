/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_changes.h"

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
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_durability_t", ideatek_durability_t);
    ENUM(api, "ideatek:ideatek_change_condition_t", ideatek_change_condition_t);
    ENUM(api, "ideatek:ideatek_recording_action_t", ideatek_recording_action_t);
    ENUM(api, "ideatek:ideatek_recording_status_t", ideatek_recording_status_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ChangeTag", ideatek_change_tag_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:EntityId", ideatek_entity_id_t);
    STRUCT(api, "ideatek:ChangeTag", ideatek_change_tag_t);
    STRUCT(api, "ideatek:RowChanges", ideatek_row_changes_t);
    STRUCT(api, "ideatek:RowsIn", ideatek_rows_in_t);
    STRUCT(api, "ideatek:StoreState", ideatek_store_state_t);
    STRUCT(api, "ideatek:Recording", ideatek_recording_t);
    STRUCT(api, "ideatek:RecordingState", ideatek_recording_state_t);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_durability_t", ideatek_durability_t);
    ENUM(api, "ideatek:ideatek_change_condition_t", ideatek_change_condition_t);
    STRUCT(api, "ideatek:RowsIn", ideatek_rows_in_t);
    STRUCT(api, "ideatek:EntityId", ideatek_entity_id_t);
    api->attach(api, id, STRUCT(api, "ideatek:ChangeTag", ideatek_change_tag_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RowChanges",
        ideatek_row_changes_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_ids(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    api->attach(api, id, STRUCT(api, "ideatek:EntityId", ideatek_entity_id_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_consumer(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_durability_t", ideatek_durability_t);
    ENUM(api, "ideatek:ideatek_change_condition_t", ideatek_change_condition_t);
    ENUM(api, "ideatek:ideatek_recording_action_t", ideatek_recording_action_t);
    ENUM(api, "ideatek:ideatek_recording_status_t", ideatek_recording_status_t);
    STRUCT(api, "ideatek:ChangeTag", ideatek_change_tag_t);
    STRUCT(api, "ideatek:RowChanges", ideatek_row_changes_t);
    STRUCT(api, "ideatek:EntityId", ideatek_entity_id_t);
    STRUCT(api, "ideatek:Recording", ideatek_recording_t);
    api->attach(api, id, STRUCT(api, "ideatek:RowsIn", ideatek_rows_in_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:StoreState",
        ideatek_store_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RecordingState",
        ideatek_recording_state_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_recording_action_t", ideatek_recording_action_t);
    ENUM(api, "ideatek:ideatek_recording_status_t", ideatek_recording_status_t);
    STRUCT(api, "ideatek:RecordingState", ideatek_recording_state_t);
    STRUCT(api, "ideatek:StoreState", ideatek_store_state_t);
    api->attach(api, id, STRUCT(api, "ideatek:Recording", ideatek_recording_t),
        BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_MODULE)
    role_module(api);
#elif defined(ROLE_IDS)
    role_ids(api);
#elif defined(ROLE_CONSUMER)
    role_consumer(api);
#elif defined(ROLE_GAME)
    role_game(api);
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
