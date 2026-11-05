/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"
#include "ideatek_changes.h"
#include "ideatek_networking.h"

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
    ENUM(api, "ideatek:ideatek_net_role_t", ideatek_net_role_t);
    ENUM(api, "ideatek:ideatek_peer_state_t", ideatek_peer_state_t);
    ENUM(api, "ideatek:ideatek_admit_t", ideatek_admit_t);
    ENUM(api, "ideatek:ideatek_net_authority_t", ideatek_net_authority_t);
    ENUM(api, "ideatek:ideatek_relevance_mode_t", ideatek_relevance_mode_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:NetSession", ideatek_net_session_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:NetSession", ideatek_net_session_t);
    STRUCT(api, "ideatek:NetPeer", ideatek_net_peer_t);
    STRUCT(api, "ideatek:NetJoin", ideatek_net_join_t);
    STRUCT(api, "ideatek:NetJoinEvent", ideatek_net_join_event_t);
    STRUCT(api, "ideatek:NetAdmit", ideatek_net_admit_t);
    STRUCT(api, "ideatek:Replicated", ideatek_replicated_t);
    STRUCT(api, "ideatek:NetViewer", ideatek_net_viewer_t);
    STRUCT(api, "ideatek:NetRelevance", ideatek_net_relevance_t);
    STRUCT(api, "ideatek:NetCommand", ideatek_net_command_t);
    STRUCT(api, "ideatek:NetCommandIn", ideatek_net_command_in_t);
    STRUCT(api, "ideatek:NetSessionState", ideatek_net_session_state_t);
    STRUCT(api, "ideatek:NetPeerGroups", ideatek_net_peer_groups_t);
    STRUCT(api, "ideatek:NetPredictedSpawn", ideatek_net_predicted_spawn_t);
    STRUCT(api, "ideatek:NetRollback", ideatek_net_rollback_t);
    STRUCT(api, "ideatek:NetAuthorityChanged", ideatek_net_authority_changed_t);
    STRUCT(api, "ideatek:NetHostChanged", ideatek_net_host_changed_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_net_role_t", ideatek_net_role_t);
    ENUM(api, "ideatek:ideatek_admit_t", ideatek_admit_t);
    ENUM(api, "ideatek:ideatek_net_authority_t", ideatek_net_authority_t);
    ENUM(api, "ideatek:ideatek_relevance_mode_t", ideatek_relevance_mode_t);
    api->attach(api, id, STRUCT(api, "ideatek:NetSession",
        ideatek_net_session_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetJoin", ideatek_net_join_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetAdmit", ideatek_net_admit_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Replicated",
        ideatek_replicated_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetViewer", ideatek_net_viewer_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetRelevance",
        ideatek_net_relevance_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetCommand",
        ideatek_net_command_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetPeerGroups",
        ideatek_net_peer_groups_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetPredictedSpawn",
        ideatek_net_predicted_spawn_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_change_kind_t", ideatek_change_kind_t);
    ENUM(api, "ideatek:ideatek_durability_t", ideatek_durability_t);
    ENUM(api, "ideatek:ideatek_change_condition_t", ideatek_change_condition_t);
    ENUM(api, "ideatek:ideatek_net_role_t", ideatek_net_role_t);
    ENUM(api, "ideatek:ideatek_peer_state_t", ideatek_peer_state_t);
    ENUM(api, "ideatek:ideatek_admit_t", ideatek_admit_t);
    ENUM(api, "ideatek:ideatek_net_authority_t", ideatek_net_authority_t);
    ENUM(api, "ideatek:ideatek_relevance_mode_t", ideatek_relevance_mode_t);
    STRUCT(api, "ideatek:NetSession", ideatek_net_session_t);
    STRUCT(api, "ideatek:NetJoin", ideatek_net_join_t);
    STRUCT(api, "ideatek:NetAdmit", ideatek_net_admit_t);
    STRUCT(api, "ideatek:Replicated", ideatek_replicated_t);
    STRUCT(api, "ideatek:NetViewer", ideatek_net_viewer_t);
    STRUCT(api, "ideatek:NetRelevance", ideatek_net_relevance_t);
    STRUCT(api, "ideatek:NetCommand", ideatek_net_command_t);
    STRUCT(api, "ideatek:NetPeerGroups", ideatek_net_peer_groups_t);
    STRUCT(api, "ideatek:NetPredictedSpawn", ideatek_net_predicted_spawn_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Tick", ideatek_tick_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:RowChanges", ideatek_row_changes_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ChangeTag", ideatek_change_tag_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:EntityId", ideatek_entity_id_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:NetPeer", ideatek_net_peer_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetJoinEvent",
        ideatek_net_join_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetCommandIn",
        ideatek_net_command_in_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetSessionState",
        ideatek_net_session_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetRollback",
        ideatek_net_rollback_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetAuthorityChanged",
        ideatek_net_authority_changed_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NetHostChanged",
        ideatek_net_host_changed_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:RowsIn",
        ideatek_rows_in_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
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
