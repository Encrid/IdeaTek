/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_services.h"

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
    ENUM(api, "ideatek:ideatek_service_state_t", ideatek_service_state_t);
    ENUM(api, "ideatek:ideatek_platform_t", ideatek_platform_t);
    ENUM(api, "ideatek:ideatek_sign_status_t", ideatek_sign_status_t);
    MASK(api, "ideatek:ideatek_sign_in_flags_t", ideatek_sign_in_flags_t);
    ENUM(api, "ideatek:ideatek_ticket_purpose_t", ideatek_ticket_purpose_t);
    ENUM(api, "ideatek:ideatek_stat_mode_t", ideatek_stat_mode_t);
    ENUM(api, "ideatek:ideatek_score_method_t", ideatek_score_method_t);
    ENUM(api, "ideatek:ideatek_score_range_t", ideatek_score_range_t);
    ENUM(api, "ideatek:ideatek_friend_status_t", ideatek_friend_status_t);
    ENUM(api, "ideatek:ideatek_joinable_t", ideatek_joinable_t);
    ENUM(api, "ideatek:ideatek_lobby_kind_t", ideatek_lobby_kind_t);
    ENUM(api, "ideatek:ideatek_lobby_state_t", ideatek_lobby_state_t);
    ENUM(api, "ideatek:ideatek_compare_t", ideatek_compare_t);
    MASK(api, "ideatek:ideatek_voice_channel_flags_t",
        ideatek_voice_channel_flags_t);
    MASK(api, "ideatek:ideatek_store_flags_t", ideatek_store_flags_t);
    MASK(api, "ideatek:ideatek_ugc_state_t", ideatek_ugc_state_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:PlatformUser", ideatek_platform_user_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:PlatformUser", ideatek_platform_user_t);
    STRUCT(api, "ideatek:DeviceUser", ideatek_device_user_t);
    STRUCT(api, "ideatek:SignInRequest", ideatek_sign_in_request_t);
    STRUCT(api, "ideatek:TicketRequest", ideatek_ticket_request_t);
    STRUCT(api, "ideatek:Ticket", ideatek_ticket_t);
    STRUCT(api, "ideatek:TicketCheck", ideatek_ticket_check_t);
    STRUCT(api, "ideatek:TicketResult", ideatek_ticket_result_t);
    STRUCT(api, "ideatek:AchievementState", ideatek_achievement_state_t);
    STRUCT(api, "ideatek:AchievementProgress", ideatek_achievement_progress_t);
    STRUCT(api, "ideatek:StatState", ideatek_stat_state_t);
    STRUCT(api, "ideatek:StatUpdate", ideatek_stat_update_t);
    STRUCT(api, "ideatek:ScoreUpload", ideatek_score_upload_t);
    STRUCT(api, "ideatek:ScoreResult", ideatek_score_result_t);
    STRUCT(api, "ideatek:ScoreQuery", ideatek_score_query_t);
    STRUCT(api, "ideatek:ScoreEntry", ideatek_score_entry_t);
    STRUCT(api, "ideatek:CloudState", ideatek_cloud_state_t);
    STRUCT(api, "ideatek:Friend", ideatek_friend_t);
    STRUCT(api, "ideatek:Presence", ideatek_presence_t);
    STRUCT(api, "ideatek:InviteRequest", ideatek_invite_request_t);
    STRUCT(api, "ideatek:JoinRequested", ideatek_join_requested_t);
    STRUCT(api, "ideatek:Lobby", ideatek_lobby_t);
    STRUCT(api, "ideatek:LobbyJoin", ideatek_lobby_join_t);
    STRUCT(api, "ideatek:LobbyState", ideatek_lobby_state_record_t);
    STRUCT(api, "ideatek:LobbyMember", ideatek_lobby_member_t);
    STRUCT(api, "ideatek:LobbyValue", ideatek_lobby_value_t);
    STRUCT(api, "ideatek:LobbySet", ideatek_lobby_set_t);
    STRUCT(api, "ideatek:LobbySearch", ideatek_lobby_search_t);
    STRUCT(api, "ideatek:LobbyFilter", ideatek_lobby_filter_t);
    STRUCT(api, "ideatek:LobbyFound", ideatek_lobby_found_t);
    STRUCT(api, "ideatek:VoiceChannel", ideatek_voice_channel_t);
    STRUCT(api, "ideatek:VoiceMember", ideatek_voice_member_t);
    STRUCT(api, "ideatek:VoiceSettings", ideatek_voice_settings_t);
    STRUCT(api, "ideatek:Entitlement", ideatek_entitlement_t);
    STRUCT(api, "ideatek:InstallRequest", ideatek_install_request_t);
    STRUCT(api, "ideatek:StoreRequest", ideatek_store_request_t);
    STRUCT(api, "ideatek:PlatformOverlay", ideatek_platform_overlay_t);
    STRUCT(api, "ideatek:UgcItem", ideatek_ugc_item_t);
    STRUCT(api, "ideatek:UgcSubscribe", ideatek_ugc_subscribe_t);
    STRUCT(api, "ideatek:UgcPublish", ideatek_ugc_publish_t);
    STRUCT(api, "ideatek:UgcResult", ideatek_ugc_result_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    MASK(api, "ideatek:ideatek_sign_in_flags_t", ideatek_sign_in_flags_t);
    ENUM(api, "ideatek:ideatek_ticket_purpose_t", ideatek_ticket_purpose_t);
    ENUM(api, "ideatek:ideatek_stat_mode_t", ideatek_stat_mode_t);
    ENUM(api, "ideatek:ideatek_score_method_t", ideatek_score_method_t);
    ENUM(api, "ideatek:ideatek_score_range_t", ideatek_score_range_t);
    ENUM(api, "ideatek:ideatek_joinable_t", ideatek_joinable_t);
    ENUM(api, "ideatek:ideatek_lobby_kind_t", ideatek_lobby_kind_t);
    ENUM(api, "ideatek:ideatek_compare_t", ideatek_compare_t);
    MASK(api, "ideatek:ideatek_voice_channel_flags_t",
        ideatek_voice_channel_flags_t);
    MASK(api, "ideatek:ideatek_store_flags_t", ideatek_store_flags_t);
    api->attach(api, id, STRUCT(api, "ideatek:SignInRequest",
        ideatek_sign_in_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:TicketRequest",
        ideatek_ticket_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:TicketCheck",
        ideatek_ticket_check_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AchievementProgress",
        ideatek_achievement_progress_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:StatUpdate",
        ideatek_stat_update_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ScoreUpload",
        ideatek_score_upload_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ScoreQuery",
        ideatek_score_query_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Presence", ideatek_presence_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InviteRequest",
        ideatek_invite_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Lobby", ideatek_lobby_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyJoin", ideatek_lobby_join_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbySet", ideatek_lobby_set_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbySearch",
        ideatek_lobby_search_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyFilter",
        ideatek_lobby_filter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VoiceChannel",
        ideatek_voice_channel_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VoiceSettings",
        ideatek_voice_settings_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:InstallRequest",
        ideatek_install_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:StoreRequest",
        ideatek_store_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UgcSubscribe",
        ideatek_ugc_subscribe_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UgcPublish",
        ideatek_ugc_publish_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_service_state_t", ideatek_service_state_t);
    ENUM(api, "ideatek:ideatek_platform_t", ideatek_platform_t);
    ENUM(api, "ideatek:ideatek_sign_status_t", ideatek_sign_status_t);
    MASK(api, "ideatek:ideatek_sign_in_flags_t", ideatek_sign_in_flags_t);
    ENUM(api, "ideatek:ideatek_ticket_purpose_t", ideatek_ticket_purpose_t);
    ENUM(api, "ideatek:ideatek_stat_mode_t", ideatek_stat_mode_t);
    ENUM(api, "ideatek:ideatek_score_method_t", ideatek_score_method_t);
    ENUM(api, "ideatek:ideatek_score_range_t", ideatek_score_range_t);
    ENUM(api, "ideatek:ideatek_friend_status_t", ideatek_friend_status_t);
    ENUM(api, "ideatek:ideatek_joinable_t", ideatek_joinable_t);
    ENUM(api, "ideatek:ideatek_lobby_kind_t", ideatek_lobby_kind_t);
    ENUM(api, "ideatek:ideatek_lobby_state_t", ideatek_lobby_state_t);
    ENUM(api, "ideatek:ideatek_compare_t", ideatek_compare_t);
    MASK(api, "ideatek:ideatek_voice_channel_flags_t",
        ideatek_voice_channel_flags_t);
    MASK(api, "ideatek:ideatek_store_flags_t", ideatek_store_flags_t);
    MASK(api, "ideatek:ideatek_ugc_state_t", ideatek_ugc_state_t);
    STRUCT(api, "ideatek:SignInRequest", ideatek_sign_in_request_t);
    STRUCT(api, "ideatek:TicketRequest", ideatek_ticket_request_t);
    STRUCT(api, "ideatek:TicketCheck", ideatek_ticket_check_t);
    STRUCT(api, "ideatek:AchievementProgress", ideatek_achievement_progress_t);
    STRUCT(api, "ideatek:StatUpdate", ideatek_stat_update_t);
    STRUCT(api, "ideatek:ScoreUpload", ideatek_score_upload_t);
    STRUCT(api, "ideatek:ScoreQuery", ideatek_score_query_t);
    STRUCT(api, "ideatek:Presence", ideatek_presence_t);
    STRUCT(api, "ideatek:InviteRequest", ideatek_invite_request_t);
    STRUCT(api, "ideatek:Lobby", ideatek_lobby_t);
    STRUCT(api, "ideatek:LobbyJoin", ideatek_lobby_join_t);
    STRUCT(api, "ideatek:LobbySet", ideatek_lobby_set_t);
    STRUCT(api, "ideatek:LobbySearch", ideatek_lobby_search_t);
    STRUCT(api, "ideatek:LobbyFilter", ideatek_lobby_filter_t);
    STRUCT(api, "ideatek:VoiceChannel", ideatek_voice_channel_t);
    STRUCT(api, "ideatek:VoiceSettings", ideatek_voice_settings_t);
    STRUCT(api, "ideatek:InstallRequest", ideatek_install_request_t);
    STRUCT(api, "ideatek:StoreRequest", ideatek_store_request_t);
    STRUCT(api, "ideatek:UgcSubscribe", ideatek_ugc_subscribe_t);
    STRUCT(api, "ideatek:UgcPublish", ideatek_ugc_publish_t);
    api->attach(api, id, STRUCT(api, "ideatek:PlatformUser",
        ideatek_platform_user_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:DeviceUser",
        ideatek_device_user_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Ticket", ideatek_ticket_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:TicketResult",
        ideatek_ticket_result_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AchievementState",
        ideatek_achievement_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:StatState", ideatek_stat_state_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ScoreResult",
        ideatek_score_result_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ScoreEntry",
        ideatek_score_entry_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CloudState",
        ideatek_cloud_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Friend", ideatek_friend_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:JoinRequested",
        ideatek_join_requested_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyState",
        ideatek_lobby_state_record_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyMember",
        ideatek_lobby_member_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyValue",
        ideatek_lobby_value_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LobbyFound",
        ideatek_lobby_found_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VoiceMember",
        ideatek_voice_member_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Entitlement",
        ideatek_entitlement_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PlatformOverlay",
        ideatek_platform_overlay_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UgcItem", ideatek_ugc_item_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UgcResult", ideatek_ugc_result_t),
        BRIDGE_ZEROED);
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
