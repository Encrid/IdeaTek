/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_services.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_service_state(void)
{
    SIZE(ideatek_service_state_t);
}

static void show_platform(void)
{
    SIZE(ideatek_platform_t);
}

static void show_sign_status(void)
{
    SIZE(ideatek_sign_status_t);
}

static void show_platform_user(void)
{
    SIZE(ideatek_platform_user_t);
    AT(ideatek_platform_user_t, account);
    AT(ideatek_platform_user_t, name);
    AT(ideatek_platform_user_t, platform);
    AT(ideatek_platform_user_t, status);
}

static void show_sign_in_flags(void)
{
    SIZE(ideatek_sign_in_flags_t);
}

static void show_sign_in_request(void)
{
    SIZE(ideatek_sign_in_request_t);
    AT(ideatek_sign_in_request_t, flags);
}

static void show_ticket_purpose(void)
{
    SIZE(ideatek_ticket_purpose_t);
}

static void show_ticket_request(void)
{
    SIZE(ideatek_ticket_request_t);
    AT(ideatek_ticket_request_t, user);
    AT(ideatek_ticket_request_t, identity);
    AT(ideatek_ticket_request_t, purpose);
}

static void show_ticket(void)
{
    SIZE(ideatek_ticket_t);
    AT(ideatek_ticket_t, data);
    AT(ideatek_ticket_t, state);
}

static void show_ticket_check(void)
{
    SIZE(ideatek_ticket_check_t);
    AT(ideatek_ticket_check_t, account);
    AT(ideatek_ticket_check_t, data);
}

static void show_ticket_result(void)
{
    SIZE(ideatek_ticket_result_t);
    AT(ideatek_ticket_result_t, account);
    AT(ideatek_ticket_result_t, owner);
    AT(ideatek_ticket_result_t, state);
}

static void show_achievement_state(void)
{
    SIZE(ideatek_achievement_state_t);
    AT(ideatek_achievement_state_t, user);
    AT(ideatek_achievement_state_t, time);
    AT(ideatek_achievement_state_t, name);
    AT(ideatek_achievement_state_t, progress);
    AT(ideatek_achievement_state_t, unlocked);
}

static void show_achievement_progress(void)
{
    SIZE(ideatek_achievement_progress_t);
    AT(ideatek_achievement_progress_t, user);
    AT(ideatek_achievement_progress_t, name);
    AT(ideatek_achievement_progress_t, progress);
}

static void show_stat_state(void)
{
    SIZE(ideatek_stat_state_t);
    AT(ideatek_stat_state_t, user);
    AT(ideatek_stat_state_t, value);
    AT(ideatek_stat_state_t, name);
}

static void show_stat_mode(void)
{
    SIZE(ideatek_stat_mode_t);
}

static void show_stat_update(void)
{
    SIZE(ideatek_stat_update_t);
    AT(ideatek_stat_update_t, user);
    AT(ideatek_stat_update_t, value);
    AT(ideatek_stat_update_t, name);
    AT(ideatek_stat_update_t, mode);
}

static void show_score_method(void)
{
    SIZE(ideatek_score_method_t);
}

static void show_score_upload(void)
{
    SIZE(ideatek_score_upload_t);
    AT(ideatek_score_upload_t, user);
    AT(ideatek_score_upload_t, score);
    AT(ideatek_score_upload_t, board);
    AT(ideatek_score_upload_t, method);
}

static void show_score_result(void)
{
    SIZE(ideatek_score_result_t);
    AT(ideatek_score_result_t, rank);
    AT(ideatek_score_result_t, state);
}

static void show_score_range(void)
{
    SIZE(ideatek_score_range_t);
}

static void show_score_query(void)
{
    SIZE(ideatek_score_query_t);
    AT(ideatek_score_query_t, user);
    AT(ideatek_score_query_t, board);
    AT(ideatek_score_query_t, start);
    AT(ideatek_score_query_t, count);
    AT(ideatek_score_query_t, range);
}

static void show_score_entry(void)
{
    SIZE(ideatek_score_entry_t);
    AT(ideatek_score_entry_t, query);
    AT(ideatek_score_entry_t, account);
    AT(ideatek_score_entry_t, score);
    AT(ideatek_score_entry_t, name);
    AT(ideatek_score_entry_t, rank);
}

static void show_cloud_state(void)
{
    SIZE(ideatek_cloud_state_t);
    AT(ideatek_cloud_state_t, user);
    AT(ideatek_cloud_state_t, quota_bytes);
    AT(ideatek_cloud_state_t, free_bytes);
    AT(ideatek_cloud_state_t, enabled);
}

static void show_friend_status(void)
{
    SIZE(ideatek_friend_status_t);
}

static void show_friend(void)
{
    SIZE(ideatek_friend_t);
    AT(ideatek_friend_t, user);
    AT(ideatek_friend_t, account);
    AT(ideatek_friend_t, name);
    AT(ideatek_friend_t, presence);
    AT(ideatek_friend_t, status);
    AT(ideatek_friend_t, in_game);
}

static void show_joinable(void)
{
    SIZE(ideatek_joinable_t);
}

static void show_presence(void)
{
    SIZE(ideatek_presence_t);
    AT(ideatek_presence_t, user);
    AT(ideatek_presence_t, text);
    AT(ideatek_presence_t, join);
    AT(ideatek_presence_t, joinable);
}

static void show_invite_request(void)
{
    SIZE(ideatek_invite_request_t);
    AT(ideatek_invite_request_t, user);
    AT(ideatek_invite_request_t, account);
}

static void show_join_requested(void)
{
    SIZE(ideatek_join_requested_t);
    AT(ideatek_join_requested_t, user);
    AT(ideatek_join_requested_t, join);
    AT(ideatek_join_requested_t, order);
}

static void show_lobby_kind(void)
{
    SIZE(ideatek_lobby_kind_t);
}

static void show_lobby(void)
{
    SIZE(ideatek_lobby_t);
    AT(ideatek_lobby_t, user);
    AT(ideatek_lobby_t, max_members);
    AT(ideatek_lobby_t, kind);
    AT(ideatek_lobby_t, joinable);
}

static void show_lobby_join(void)
{
    SIZE(ideatek_lobby_join_t);
    AT(ideatek_lobby_join_t, user);
    AT(ideatek_lobby_join_t, id);
}

static void show_lobby_state(void)
{
    SIZE(ideatek_lobby_state_t);
}

static void show_lobby_state_record(void)
{
    SIZE(ideatek_lobby_state_record_t);
    AT(ideatek_lobby_state_record_t, id);
    AT(ideatek_lobby_state_record_t, owner);
    AT(ideatek_lobby_state_record_t, state);
}

static void show_lobby_member(void)
{
    SIZE(ideatek_lobby_member_t);
    AT(ideatek_lobby_member_t, lobby);
    AT(ideatek_lobby_member_t, account);
    AT(ideatek_lobby_member_t, name);
}

static void show_lobby_value(void)
{
    SIZE(ideatek_lobby_value_t);
    AT(ideatek_lobby_value_t, lobby);
    AT(ideatek_lobby_value_t, key);
    AT(ideatek_lobby_value_t, value);
}

static void show_lobby_set(void)
{
    SIZE(ideatek_lobby_set_t);
    AT(ideatek_lobby_set_t, lobby);
    AT(ideatek_lobby_set_t, key);
    AT(ideatek_lobby_set_t, value);
    AT(ideatek_lobby_set_t, order);
}

static void show_lobby_search(void)
{
    SIZE(ideatek_lobby_search_t);
    AT(ideatek_lobby_search_t, user);
    AT(ideatek_lobby_search_t, max);
    AT(ideatek_lobby_search_t, free_slots);
}

static void show_compare(void)
{
    SIZE(ideatek_compare_t);
}

static void show_lobby_filter(void)
{
    SIZE(ideatek_lobby_filter_t);
    AT(ideatek_lobby_filter_t, search);
    AT(ideatek_lobby_filter_t, key);
    AT(ideatek_lobby_filter_t, value);
    AT(ideatek_lobby_filter_t, compare);
    AT(ideatek_lobby_filter_t, numeric);
}

static void show_lobby_found(void)
{
    SIZE(ideatek_lobby_found_t);
    AT(ideatek_lobby_found_t, search);
    AT(ideatek_lobby_found_t, id);
    AT(ideatek_lobby_found_t, members);
    AT(ideatek_lobby_found_t, max_members);
}

static void show_voice_channel_flags(void)
{
    SIZE(ideatek_voice_channel_flags_t);
}

static void show_voice_channel(void)
{
    SIZE(ideatek_voice_channel_t);
    AT(ideatek_voice_channel_t, user);
    AT(ideatek_voice_channel_t, bus);
    AT(ideatek_voice_channel_t, name);
    AT(ideatek_voice_channel_t, flags);
}

static void show_voice_member(void)
{
    SIZE(ideatek_voice_member_t);
    AT(ideatek_voice_member_t, channel);
    AT(ideatek_voice_member_t, account);
    AT(ideatek_voice_member_t, talking);
}

static void show_voice_settings(void)
{
    SIZE(ideatek_voice_settings_t);
    AT(ideatek_voice_settings_t, speaker);
    AT(ideatek_voice_settings_t, volume);
    AT(ideatek_voice_settings_t, muted);
}

static void show_entitlement(void)
{
    SIZE(ideatek_entitlement_t);
    AT(ideatek_entitlement_t, user);
    AT(ideatek_entitlement_t, id);
    AT(ideatek_entitlement_t, name);
    AT(ideatek_entitlement_t, path);
    AT(ideatek_entitlement_t, owned);
    AT(ideatek_entitlement_t, installed);
}

static void show_install_request(void)
{
    SIZE(ideatek_install_request_t);
    AT(ideatek_install_request_t, entitlement);
}

static void show_store_flags(void)
{
    SIZE(ideatek_store_flags_t);
}

static void show_store_request(void)
{
    SIZE(ideatek_store_request_t);
    AT(ideatek_store_request_t, user);
    AT(ideatek_store_request_t, id);
    AT(ideatek_store_request_t, flags);
}

static void show_platform_overlay(void)
{
    SIZE(ideatek_platform_overlay_t);
    AT(ideatek_platform_overlay_t, active);
}

static void show_ugc_state(void)
{
    SIZE(ideatek_ugc_state_t);
}

static void show_ugc_item(void)
{
    SIZE(ideatek_ugc_item_t);
    AT(ideatek_ugc_item_t, user);
    AT(ideatek_ugc_item_t, bytes);
    AT(ideatek_ugc_item_t, id);
    AT(ideatek_ugc_item_t, title);
    AT(ideatek_ugc_item_t, path);
    AT(ideatek_ugc_item_t, state);
}

static void show_ugc_subscribe(void)
{
    SIZE(ideatek_ugc_subscribe_t);
    AT(ideatek_ugc_subscribe_t, user);
    AT(ideatek_ugc_subscribe_t, id);
    AT(ideatek_ugc_subscribe_t, subscribe);
}

static void show_ugc_publish(void)
{
    SIZE(ideatek_ugc_publish_t);
    AT(ideatek_ugc_publish_t, user);
    AT(ideatek_ugc_publish_t, id);
    AT(ideatek_ugc_publish_t, title);
    AT(ideatek_ugc_publish_t, folder);
    AT(ideatek_ugc_publish_t, note);
}

static void show_ugc_result(void)
{
    SIZE(ideatek_ugc_result_t);
    AT(ideatek_ugc_result_t, id);
    AT(ideatek_ugc_result_t, state);
}

static void show_game_server(void)
{
    SIZE(ideatek_game_server_t);
    AT(ideatek_game_server_t, name);
    AT(ideatek_game_server_t, map);
    AT(ideatek_game_server_t, tags);
    AT(ideatek_game_server_t, address);
    AT(ideatek_game_server_t, players);
    AT(ideatek_game_server_t, max_players);
    AT(ideatek_game_server_t, bots);
    AT(ideatek_game_server_t, password);
    AT(ideatek_game_server_t, dedicated);
}

static void show_server_list(void)
{
    SIZE(ideatek_server_list_t);
}

static void show_server_search(void)
{
    SIZE(ideatek_server_search_t);
    AT(ideatek_server_search_t, filter);
    AT(ideatek_server_search_t, kind);
}

static void show_server_found(void)
{
    SIZE(ideatek_server_found_t);
    AT(ideatek_server_found_t, search);
    AT(ideatek_server_found_t, name);
    AT(ideatek_server_found_t, map);
    AT(ideatek_server_found_t, tags);
    AT(ideatek_server_found_t, address);
    AT(ideatek_server_found_t, players);
    AT(ideatek_server_found_t, max_players);
    AT(ideatek_server_found_t, bots);
    AT(ideatek_server_found_t, ping);
    AT(ideatek_server_found_t, password);
}

static void show_parental(void)
{
    SIZE(ideatek_parental_t);
}

static void show_parental_limits(void)
{
    SIZE(ideatek_parental_limits_t);
    AT(ideatek_parental_limits_t, blocked);
}

static void show_text_context(void)
{
    SIZE(ideatek_text_context_t);
}

static void show_text_filter(void)
{
    SIZE(ideatek_text_filter_t);
    AT(ideatek_text_filter_t, user);
    AT(ideatek_text_filter_t, text);
    AT(ideatek_text_filter_t, context);
}

static void show_text_filtered(void)
{
    SIZE(ideatek_text_filtered_t);
    AT(ideatek_text_filtered_t, text);
    AT(ideatek_text_filtered_t, state);
}

static void show_overlay_page(void)
{
    SIZE(ideatek_overlay_page_t);
}

static void show_overlay_request(void)
{
    SIZE(ideatek_overlay_request_t);
    AT(ideatek_overlay_request_t, account);
    AT(ideatek_overlay_request_t, lobby);
    AT(ideatek_overlay_request_t, page);
}

static void show_platform_info(void)
{
    SIZE(ideatek_platform_info_t);
    AT(ideatek_platform_info_t, language);
    AT(ideatek_platform_info_t, branch);
    AT(ideatek_platform_info_t, steam_deck);
    AT(ideatek_platform_info_t, big_picture);
}

static void show_timeline_mode(void)
{
    SIZE(ideatek_timeline_mode_t);
}

static void show_timeline_mode_record(void)
{
    SIZE(ideatek_timeline_mode_record_t);
    AT(ideatek_timeline_mode_record_t, mode);
}

static void show_timeline_clip(void)
{
    SIZE(ideatek_timeline_clip_t);
}

static void show_timeline_event(void)
{
    SIZE(ideatek_timeline_event_t);
    AT(ideatek_timeline_event_t, title);
    AT(ideatek_timeline_event_t, description);
    AT(ideatek_timeline_event_t, icon);
    AT(ideatek_timeline_event_t, start_offset);
    AT(ideatek_timeline_event_t, duration);
    AT(ideatek_timeline_event_t, priority);
    AT(ideatek_timeline_event_t, clip);
}

static void show_screenshot_request(void)
{
    SIZE(ideatek_screenshot_request_t);
    AT(ideatek_screenshot_request_t, location);
}

static void show_screenshot_taken(void)
{
    SIZE(ideatek_screenshot_taken_t);
    AT(ideatek_screenshot_taken_t, order);
}

static void show_avatar_size(void)
{
    SIZE(ideatek_avatar_size_t);
}

static void show_avatar_request(void)
{
    SIZE(ideatek_avatar_request_t);
    AT(ideatek_avatar_request_t, account);
    AT(ideatek_avatar_request_t, size);
}

static void show_avatar(void)
{
    SIZE(ideatek_avatar_t);
    AT(ideatek_avatar_t, pixels);
    AT(ideatek_avatar_t, width);
    AT(ideatek_avatar_t, height);
    AT(ideatek_avatar_t, state);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_service_state_t_desc, ideatek_platform_t_desc,
        ideatek_sign_status_t_desc, ideatek_platform_user_t_desc,
        ideatek_sign_in_flags_t_desc, ideatek_sign_in_request_t_desc,
        ideatek_ticket_purpose_t_desc, ideatek_ticket_request_t_desc,
        ideatek_ticket_t_desc, ideatek_ticket_check_t_desc,
        ideatek_ticket_result_t_desc, ideatek_achievement_state_t_desc,
        ideatek_achievement_progress_t_desc, ideatek_stat_state_t_desc,
        ideatek_stat_mode_t_desc, ideatek_stat_update_t_desc,
        ideatek_score_method_t_desc, ideatek_score_upload_t_desc,
        ideatek_score_result_t_desc, ideatek_score_range_t_desc,
        ideatek_score_query_t_desc, ideatek_score_entry_t_desc,
        ideatek_cloud_state_t_desc, ideatek_friend_status_t_desc,
        ideatek_friend_t_desc, ideatek_joinable_t_desc, ideatek_presence_t_desc,
        ideatek_invite_request_t_desc, ideatek_join_requested_t_desc,
        ideatek_lobby_kind_t_desc, ideatek_lobby_t_desc,
        ideatek_lobby_join_t_desc, ideatek_lobby_state_t_desc,
        ideatek_lobby_state_record_t_desc, ideatek_lobby_member_t_desc,
        ideatek_lobby_value_t_desc, ideatek_lobby_set_t_desc,
        ideatek_lobby_search_t_desc, ideatek_compare_t_desc,
        ideatek_lobby_filter_t_desc, ideatek_lobby_found_t_desc,
        ideatek_voice_channel_flags_t_desc, ideatek_voice_channel_t_desc,
        ideatek_voice_member_t_desc, ideatek_voice_settings_t_desc,
        ideatek_entitlement_t_desc, ideatek_install_request_t_desc,
        ideatek_store_flags_t_desc, ideatek_store_request_t_desc,
        ideatek_platform_overlay_t_desc, ideatek_ugc_state_t_desc,
        ideatek_ugc_item_t_desc, ideatek_ugc_subscribe_t_desc,
        ideatek_ugc_publish_t_desc, ideatek_ugc_result_t_desc,
        ideatek_game_server_t_desc, ideatek_server_list_t_desc,
        ideatek_server_search_t_desc, ideatek_server_found_t_desc,
        ideatek_parental_t_desc, ideatek_parental_limits_t_desc,
        ideatek_text_context_t_desc, ideatek_text_filter_t_desc,
        ideatek_text_filtered_t_desc, ideatek_overlay_page_t_desc,
        ideatek_overlay_request_t_desc, ideatek_platform_info_t_desc,
        ideatek_timeline_mode_t_desc, ideatek_timeline_mode_record_t_desc,
        ideatek_timeline_clip_t_desc, ideatek_timeline_event_t_desc,
        ideatek_screenshot_request_t_desc, ideatek_screenshot_taken_t_desc,
        ideatek_avatar_size_t_desc, ideatek_avatar_request_t_desc,
        ideatek_avatar_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_SERVICE_STATE_VALUES), TEXT_OF(IDEATEK_PLATFORM_VALUES),
        TEXT_OF(IDEATEK_SIGN_STATUS_VALUES),
        TEXT_OF(IDEATEK_PLATFORM_USER_FIELDS),
        TEXT_OF(IDEATEK_SIGN_IN_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SIGN_IN_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_TICKET_PURPOSE_VALUES),
        TEXT_OF(IDEATEK_TICKET_REQUEST_FIELDS), TEXT_OF(IDEATEK_TICKET_FIELDS),
        TEXT_OF(IDEATEK_TICKET_CHECK_FIELDS),
        TEXT_OF(IDEATEK_TICKET_RESULT_FIELDS),
        TEXT_OF(IDEATEK_ACHIEVEMENT_STATE_FIELDS),
        TEXT_OF(IDEATEK_ACHIEVEMENT_PROGRESS_FIELDS),
        TEXT_OF(IDEATEK_STAT_STATE_FIELDS), TEXT_OF(IDEATEK_STAT_MODE_VALUES),
        TEXT_OF(IDEATEK_STAT_UPDATE_FIELDS),
        TEXT_OF(IDEATEK_SCORE_METHOD_VALUES),
        TEXT_OF(IDEATEK_SCORE_UPLOAD_FIELDS),
        TEXT_OF(IDEATEK_SCORE_RESULT_FIELDS),
        TEXT_OF(IDEATEK_SCORE_RANGE_VALUES),
        TEXT_OF(IDEATEK_SCORE_QUERY_FIELDS),
        TEXT_OF(IDEATEK_SCORE_ENTRY_FIELDS),
        TEXT_OF(IDEATEK_CLOUD_STATE_FIELDS),
        TEXT_OF(IDEATEK_FRIEND_STATUS_VALUES), TEXT_OF(IDEATEK_FRIEND_FIELDS),
        TEXT_OF(IDEATEK_JOINABLE_VALUES), TEXT_OF(IDEATEK_PRESENCE_FIELDS),
        TEXT_OF(IDEATEK_INVITE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_JOIN_REQUESTED_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_KIND_VALUES), TEXT_OF(IDEATEK_LOBBY_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_JOIN_FIELDS), TEXT_OF(IDEATEK_LOBBY_STATE_VALUES),
        TEXT_OF(IDEATEK_LOBBY_STATE_RECORD_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_MEMBER_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_VALUE_FIELDS), TEXT_OF(IDEATEK_LOBBY_SET_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_SEARCH_FIELDS), TEXT_OF(IDEATEK_COMPARE_VALUES),
        TEXT_OF(IDEATEK_LOBBY_FILTER_FIELDS),
        TEXT_OF(IDEATEK_LOBBY_FOUND_FIELDS),
        TEXT_OF(IDEATEK_VOICE_CHANNEL_FLAGS_VALUES),
        TEXT_OF(IDEATEK_VOICE_CHANNEL_FIELDS),
        TEXT_OF(IDEATEK_VOICE_MEMBER_FIELDS),
        TEXT_OF(IDEATEK_VOICE_SETTINGS_FIELDS),
        TEXT_OF(IDEATEK_ENTITLEMENT_FIELDS),
        TEXT_OF(IDEATEK_INSTALL_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_STORE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_STORE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_PLATFORM_OVERLAY_FIELDS),
        TEXT_OF(IDEATEK_UGC_STATE_VALUES), TEXT_OF(IDEATEK_UGC_ITEM_FIELDS),
        TEXT_OF(IDEATEK_UGC_SUBSCRIBE_FIELDS),
        TEXT_OF(IDEATEK_UGC_PUBLISH_FIELDS), TEXT_OF(IDEATEK_UGC_RESULT_FIELDS),
        TEXT_OF(IDEATEK_GAME_SERVER_FIELDS),
        TEXT_OF(IDEATEK_SERVER_LIST_VALUES),
        TEXT_OF(IDEATEK_SERVER_SEARCH_FIELDS),
        TEXT_OF(IDEATEK_SERVER_FOUND_FIELDS), TEXT_OF(IDEATEK_PARENTAL_VALUES),
        TEXT_OF(IDEATEK_PARENTAL_LIMITS_FIELDS),
        TEXT_OF(IDEATEK_TEXT_CONTEXT_VALUES),
        TEXT_OF(IDEATEK_TEXT_FILTER_FIELDS),
        TEXT_OF(IDEATEK_TEXT_FILTERED_FIELDS),
        TEXT_OF(IDEATEK_OVERLAY_PAGE_VALUES),
        TEXT_OF(IDEATEK_OVERLAY_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_PLATFORM_INFO_FIELDS),
        TEXT_OF(IDEATEK_TIMELINE_MODE_VALUES),
        TEXT_OF(IDEATEK_TIMELINE_MODE_RECORD_FIELDS),
        TEXT_OF(IDEATEK_TIMELINE_CLIP_VALUES),
        TEXT_OF(IDEATEK_TIMELINE_EVENT_FIELDS),
        TEXT_OF(IDEATEK_SCREENSHOT_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_SCREENSHOT_TAKEN_FIELDS),
        TEXT_OF(IDEATEK_AVATAR_SIZE_VALUES),
        TEXT_OF(IDEATEK_AVATAR_REQUEST_FIELDS), TEXT_OF(IDEATEK_AVATAR_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "service_state", show_service_state }, { "platform", show_platform },
        { "sign_status", show_sign_status },
        { "platform_user", show_platform_user },
        { "sign_in_flags", show_sign_in_flags },
        { "sign_in_request", show_sign_in_request },
        { "ticket_purpose", show_ticket_purpose },
        { "ticket_request", show_ticket_request }, { "ticket", show_ticket },
        { "ticket_check", show_ticket_check },
        { "ticket_result", show_ticket_result },
        { "achievement_state", show_achievement_state },
        { "achievement_progress", show_achievement_progress },
        { "stat_state", show_stat_state }, { "stat_mode", show_stat_mode },
        { "stat_update", show_stat_update },
        { "score_method", show_score_method },
        { "score_upload", show_score_upload },
        { "score_result", show_score_result },
        { "score_range", show_score_range },
        { "score_query", show_score_query },
        { "score_entry", show_score_entry },
        { "cloud_state", show_cloud_state },
        { "friend_status", show_friend_status }, { "friend", show_friend },
        { "joinable", show_joinable }, { "presence", show_presence },
        { "invite_request", show_invite_request },
        { "join_requested", show_join_requested },
        { "lobby_kind", show_lobby_kind }, { "lobby", show_lobby },
        { "lobby_join", show_lobby_join }, { "lobby_state", show_lobby_state },
        { "lobby_state_record", show_lobby_state_record },
        { "lobby_member", show_lobby_member },
        { "lobby_value", show_lobby_value }, { "lobby_set", show_lobby_set },
        { "lobby_search", show_lobby_search }, { "compare", show_compare },
        { "lobby_filter", show_lobby_filter },
        { "lobby_found", show_lobby_found },
        { "voice_channel_flags", show_voice_channel_flags },
        { "voice_channel", show_voice_channel },
        { "voice_member", show_voice_member },
        { "voice_settings", show_voice_settings },
        { "entitlement", show_entitlement },
        { "install_request", show_install_request },
        { "store_flags", show_store_flags },
        { "store_request", show_store_request },
        { "platform_overlay", show_platform_overlay },
        { "ugc_state", show_ugc_state }, { "ugc_item", show_ugc_item },
        { "ugc_subscribe", show_ugc_subscribe },
        { "ugc_publish", show_ugc_publish }, { "ugc_result", show_ugc_result },
        { "game_server", show_game_server },
        { "server_list", show_server_list },
        { "server_search", show_server_search },
        { "server_found", show_server_found }, { "parental", show_parental },
        { "parental_limits", show_parental_limits },
        { "text_context", show_text_context },
        { "text_filter", show_text_filter },
        { "text_filtered", show_text_filtered },
        { "overlay_page", show_overlay_page },
        { "overlay_request", show_overlay_request },
        { "platform_info", show_platform_info },
        { "timeline_mode", show_timeline_mode },
        { "timeline_mode_record", show_timeline_mode_record },
        { "timeline_clip", show_timeline_clip },
        { "timeline_event", show_timeline_event },
        { "screenshot_request", show_screenshot_request },
        { "screenshot_taken", show_screenshot_taken },
        { "avatar_size", show_avatar_size },
        { "avatar_request", show_avatar_request }, { "avatar", show_avatar },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
