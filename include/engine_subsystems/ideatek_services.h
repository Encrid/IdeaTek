/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SERVICES_H
    #define BC_IDEATEK_SERVICES_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_SERVICE_STATE_VALUES { \
        IDEATEK_SERVICE_STATE_PENDING = 0, \
        IDEATEK_SERVICE_STATE_DONE = 1, \
        IDEATEK_SERVICE_STATE_FAILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_service_state_t,
    uint8_t, IDEATEK_SERVICE_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_service_state_t,
    uint8_t, IDEATEK_SERVICE_STATE_VALUES);

    #define IDEATEK_PLATFORM_VALUES { \
        IDEATEK_PLATFORM_NONE = 0, \
        IDEATEK_PLATFORM_STEAM = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_platform_t, uint8_t, IDEATEK_PLATFORM_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_platform_t, uint8_t, IDEATEK_PLATFORM_VALUES);

    #define IDEATEK_SIGN_STATUS_VALUES { \
        IDEATEK_SIGN_STATUS_SIGNED_OUT = 0, \
        IDEATEK_SIGN_STATUS_SIGNED_IN = 1, \
        IDEATEK_SIGN_STATUS_REDUCED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_sign_status_t,
    uint8_t, IDEATEK_SIGN_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_sign_status_t,
    uint8_t, IDEATEK_SIGN_STATUS_VALUES);

    #define IDEATEK_PLATFORM_USER_FIELDS { \
        uint64_t account; \
        bridge_text_t name; \
        ideatek_platform_t platform; \
        ideatek_sign_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_platform_user_t,
    IDEATEK_PLATFORM_USER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_platform_user_t,
    IDEATEK_PLATFORM_USER_FIELDS);

    #define IDEATEK_SIGN_IN_FLAGS_VALUES { \
        IDEATEK_SIGN_IN_FLAGS_SHOW_UI = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_sign_in_flags_t,
    uint8_t, IDEATEK_SIGN_IN_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_sign_in_flags_t,
    uint8_t, IDEATEK_SIGN_IN_FLAGS_VALUES);

    #define IDEATEK_SIGN_IN_REQUEST_FIELDS { \
        ideatek_sign_in_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sign_in_request_t,
    IDEATEK_SIGN_IN_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sign_in_request_t,
    IDEATEK_SIGN_IN_REQUEST_FIELDS);

    #define IDEATEK_TICKET_PURPOSE_VALUES { \
        IDEATEK_TICKET_PURPOSE_SERVER = 0, \
        IDEATEK_TICKET_PURPOSE_WEB = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ticket_purpose_t,
    uint8_t, IDEATEK_TICKET_PURPOSE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ticket_purpose_t,
    uint8_t, IDEATEK_TICKET_PURPOSE_VALUES);

    #define IDEATEK_TICKET_REQUEST_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t identity; \
        ideatek_ticket_purpose_t purpose; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ticket_request_t,
    IDEATEK_TICKET_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ticket_request_t,
    IDEATEK_TICKET_REQUEST_FIELDS);

    #define IDEATEK_TICKET_FIELDS { \
        bridge_text_t data; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ticket_t, IDEATEK_TICKET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ticket_t, IDEATEK_TICKET_FIELDS);

    #define IDEATEK_TICKET_CHECK_FIELDS { \
        uint64_t account; \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ticket_check_t, IDEATEK_TICKET_CHECK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ticket_check_t, IDEATEK_TICKET_CHECK_FIELDS);

    #define IDEATEK_TICKET_RESULT_FIELDS { \
        uint64_t account; \
        uint64_t owner; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ticket_result_t,
    IDEATEK_TICKET_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ticket_result_t,
    IDEATEK_TICKET_RESULT_FIELDS);

    #define IDEATEK_ACHIEVEMENT_STATE_FIELDS { \
        ideatek_entity_t user; \
        int64_t time; \
        bridge_text_t name; \
        float progress; \
        bool unlocked; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_achievement_state_t,
    IDEATEK_ACHIEVEMENT_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_achievement_state_t,
    IDEATEK_ACHIEVEMENT_STATE_FIELDS);

    #define IDEATEK_ACHIEVEMENT_PROGRESS_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t name; \
        float progress; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_achievement_progress_t,
    IDEATEK_ACHIEVEMENT_PROGRESS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_achievement_progress_t,
    IDEATEK_ACHIEVEMENT_PROGRESS_FIELDS);

    #define IDEATEK_STAT_STATE_FIELDS { \
        ideatek_entity_t user; \
        double value; \
        bridge_text_t name; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_stat_state_t, IDEATEK_STAT_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_stat_state_t, IDEATEK_STAT_STATE_FIELDS);

    #define IDEATEK_STAT_MODE_VALUES { \
        IDEATEK_STAT_MODE_SET = 0, \
        IDEATEK_STAT_MODE_ADD = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_stat_mode_t,
    uint8_t, IDEATEK_STAT_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_stat_mode_t,
    uint8_t, IDEATEK_STAT_MODE_VALUES);

    #define IDEATEK_STAT_UPDATE_FIELDS { \
        ideatek_entity_t user; \
        double value; \
        bridge_text_t name; \
        ideatek_stat_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_stat_update_t, IDEATEK_STAT_UPDATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_stat_update_t, IDEATEK_STAT_UPDATE_FIELDS);

    #define IDEATEK_SCORE_METHOD_VALUES { \
        IDEATEK_SCORE_METHOD_KEEP_BEST = 0, \
        IDEATEK_SCORE_METHOD_FORCE = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_score_method_t,
    uint8_t, IDEATEK_SCORE_METHOD_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_score_method_t,
    uint8_t, IDEATEK_SCORE_METHOD_VALUES);

    #define IDEATEK_SCORE_UPLOAD_FIELDS { \
        ideatek_entity_t user; \
        int64_t score; \
        bridge_text_t board; \
        ideatek_score_method_t method; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_score_upload_t, IDEATEK_SCORE_UPLOAD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_score_upload_t, IDEATEK_SCORE_UPLOAD_FIELDS);

    #define IDEATEK_SCORE_RESULT_FIELDS { \
        int32_t rank; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_score_result_t, IDEATEK_SCORE_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_score_result_t, IDEATEK_SCORE_RESULT_FIELDS);

    #define IDEATEK_SCORE_RANGE_VALUES { \
        IDEATEK_SCORE_RANGE_GLOBAL = 0, \
        IDEATEK_SCORE_RANGE_AROUND_USER = 1, \
        IDEATEK_SCORE_RANGE_FRIENDS = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_score_range_t,
    uint8_t, IDEATEK_SCORE_RANGE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_score_range_t,
    uint8_t, IDEATEK_SCORE_RANGE_VALUES);

    #define IDEATEK_SCORE_QUERY_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t board; \
        int32_t start; \
        uint32_t count; \
        ideatek_score_range_t range; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_score_query_t, IDEATEK_SCORE_QUERY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_score_query_t, IDEATEK_SCORE_QUERY_FIELDS);

    #define IDEATEK_SCORE_ENTRY_FIELDS { \
        ideatek_entity_t query; \
        uint64_t account; \
        int64_t score; \
        bridge_text_t name; \
        int32_t rank; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_score_entry_t, IDEATEK_SCORE_ENTRY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_score_entry_t, IDEATEK_SCORE_ENTRY_FIELDS);

    #define IDEATEK_CLOUD_STATE_FIELDS { \
        ideatek_entity_t user; \
        uint64_t quota_bytes; \
        uint64_t free_bytes; \
        bool enabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_cloud_state_t, IDEATEK_CLOUD_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_cloud_state_t, IDEATEK_CLOUD_STATE_FIELDS);

    #define IDEATEK_FRIEND_STATUS_VALUES { \
        IDEATEK_FRIEND_STATUS_OFFLINE = 0, \
        IDEATEK_FRIEND_STATUS_ONLINE = 1, \
        IDEATEK_FRIEND_STATUS_AWAY = 2, \
        IDEATEK_FRIEND_STATUS_BUSY = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_friend_status_t,
    uint8_t, IDEATEK_FRIEND_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_friend_status_t,
    uint8_t, IDEATEK_FRIEND_STATUS_VALUES);

    #define IDEATEK_FRIEND_FIELDS { \
        ideatek_entity_t user; \
        uint64_t account; \
        bridge_text_t name; \
        bridge_text_t presence; \
        ideatek_friend_status_t status; \
        bool in_game; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_friend_t, IDEATEK_FRIEND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_friend_t, IDEATEK_FRIEND_FIELDS);

    #define IDEATEK_JOINABLE_VALUES { \
        IDEATEK_JOINABLE_PUBLIC = 0, \
        IDEATEK_JOINABLE_FRIENDS = 1, \
        IDEATEK_JOINABLE_INVITE = 2, \
        IDEATEK_JOINABLE_PRIVATE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_joinable_t, uint8_t, IDEATEK_JOINABLE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_joinable_t, uint8_t, IDEATEK_JOINABLE_VALUES);

    #define IDEATEK_PRESENCE_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t text; \
        bridge_text_t join; \
        ideatek_joinable_t joinable; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_presence_t, IDEATEK_PRESENCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_presence_t, IDEATEK_PRESENCE_FIELDS);

    #define IDEATEK_INVITE_REQUEST_FIELDS { \
        ideatek_entity_t user; \
        uint64_t account; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_invite_request_t,
    IDEATEK_INVITE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_invite_request_t,
    IDEATEK_INVITE_REQUEST_FIELDS);

    #define IDEATEK_JOIN_REQUESTED_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t join; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_join_requested_t,
    IDEATEK_JOIN_REQUESTED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_join_requested_t,
    IDEATEK_JOIN_REQUESTED_FIELDS);

    #define IDEATEK_LOBBY_KIND_VALUES { \
        IDEATEK_LOBBY_KIND_PRIVATE = 0, \
        IDEATEK_LOBBY_KIND_FRIENDS = 1, \
        IDEATEK_LOBBY_KIND_PUBLIC = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_lobby_kind_t,
    uint8_t, IDEATEK_LOBBY_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_lobby_kind_t,
    uint8_t, IDEATEK_LOBBY_KIND_VALUES);

    #define IDEATEK_LOBBY_FIELDS { \
        ideatek_entity_t user; \
        uint32_t max_members; \
        ideatek_lobby_kind_t kind; \
        bool joinable; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_t, IDEATEK_LOBBY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_t, IDEATEK_LOBBY_FIELDS);

    #define IDEATEK_LOBBY_JOIN_FIELDS { \
        ideatek_entity_t user; \
        uint64_t id; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_join_t, IDEATEK_LOBBY_JOIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_join_t, IDEATEK_LOBBY_JOIN_FIELDS);

    #define IDEATEK_LOBBY_STATE_VALUES { \
        IDEATEK_LOBBY_STATE_PENDING = 0, \
        IDEATEK_LOBBY_STATE_JOINED = 1, \
        IDEATEK_LOBBY_STATE_FAILED = 2, \
        IDEATEK_LOBBY_STATE_LEFT = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_lobby_state_t,
    uint8_t, IDEATEK_LOBBY_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_lobby_state_t,
    uint8_t, IDEATEK_LOBBY_STATE_VALUES);

    #define IDEATEK_LOBBY_STATE_RECORD_FIELDS { \
        uint64_t id; \
        uint64_t owner; \
        ideatek_lobby_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_state_record_t,
    IDEATEK_LOBBY_STATE_RECORD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_state_record_t,
    IDEATEK_LOBBY_STATE_RECORD_FIELDS);

    #define IDEATEK_LOBBY_MEMBER_FIELDS { \
        ideatek_entity_t lobby; \
        uint64_t account; \
        bridge_text_t name; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_member_t, IDEATEK_LOBBY_MEMBER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_member_t, IDEATEK_LOBBY_MEMBER_FIELDS);

    #define IDEATEK_LOBBY_VALUE_FIELDS { \
        ideatek_entity_t lobby; \
        bridge_text_t key; \
        bridge_text_t value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_value_t, IDEATEK_LOBBY_VALUE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_value_t, IDEATEK_LOBBY_VALUE_FIELDS);

    #define IDEATEK_LOBBY_SET_FIELDS { \
        ideatek_entity_t lobby; \
        bridge_text_t key; \
        bridge_text_t value; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_set_t, IDEATEK_LOBBY_SET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_set_t, IDEATEK_LOBBY_SET_FIELDS);

    #define IDEATEK_LOBBY_SEARCH_FIELDS { \
        ideatek_entity_t user; \
        uint32_t max; \
        uint32_t free_slots; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_search_t, IDEATEK_LOBBY_SEARCH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_search_t, IDEATEK_LOBBY_SEARCH_FIELDS);

    #define IDEATEK_COMPARE_VALUES { \
        IDEATEK_COMPARE_EQUAL = 0, \
        IDEATEK_COMPARE_NOT_EQUAL = 1, \
        IDEATEK_COMPARE_LESS = 2, \
        IDEATEK_COMPARE_LESS_EQUAL = 3, \
        IDEATEK_COMPARE_GREATER = 4, \
        IDEATEK_COMPARE_GREATER_EQUAL = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_compare_t, uint8_t, IDEATEK_COMPARE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_compare_t, uint8_t, IDEATEK_COMPARE_VALUES);

    #define IDEATEK_LOBBY_FILTER_FIELDS { \
        ideatek_entity_t search; \
        bridge_text_t key; \
        bridge_text_t value; \
        ideatek_compare_t compare; \
        bool numeric; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_filter_t, IDEATEK_LOBBY_FILTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_filter_t, IDEATEK_LOBBY_FILTER_FIELDS);

    #define IDEATEK_LOBBY_FOUND_FIELDS { \
        ideatek_entity_t search; \
        uint64_t id; \
        uint32_t members; \
        uint32_t max_members; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lobby_found_t, IDEATEK_LOBBY_FOUND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lobby_found_t, IDEATEK_LOBBY_FOUND_FIELDS);

    #define IDEATEK_VOICE_CHANNEL_FLAGS_VALUES { \
        IDEATEK_VOICE_CHANNEL_FLAGS_TRANSMIT = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_voice_channel_flags_t,
    uint8_t, IDEATEK_VOICE_CHANNEL_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_voice_channel_flags_t,
    uint8_t, IDEATEK_VOICE_CHANNEL_FLAGS_VALUES);

    #define IDEATEK_VOICE_CHANNEL_FIELDS { \
        ideatek_entity_t user; \
        ideatek_entity_t bus; \
        bridge_text_t name; \
        ideatek_voice_channel_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_voice_channel_t,
    IDEATEK_VOICE_CHANNEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_voice_channel_t,
    IDEATEK_VOICE_CHANNEL_FIELDS);

    #define IDEATEK_VOICE_MEMBER_FIELDS { \
        ideatek_entity_t channel; \
        uint64_t account; \
        bool talking; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_voice_member_t, IDEATEK_VOICE_MEMBER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_voice_member_t, IDEATEK_VOICE_MEMBER_FIELDS);

    #define IDEATEK_VOICE_SETTINGS_FIELDS { \
        ideatek_entity_t speaker; \
        float volume; \
        bool muted; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_voice_settings_t,
    IDEATEK_VOICE_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_voice_settings_t,
    IDEATEK_VOICE_SETTINGS_FIELDS);

    #define IDEATEK_ENTITLEMENT_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t id; \
        bridge_text_t name; \
        bridge_text_t path; \
        bool owned; \
        bool installed; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_entitlement_t, IDEATEK_ENTITLEMENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_entitlement_t, IDEATEK_ENTITLEMENT_FIELDS);

    #define IDEATEK_INSTALL_REQUEST_FIELDS { \
        ideatek_entity_t entitlement; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_install_request_t,
    IDEATEK_INSTALL_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_install_request_t,
    IDEATEK_INSTALL_REQUEST_FIELDS);

    #define IDEATEK_STORE_FLAGS_VALUES { \
        IDEATEK_STORE_FLAGS_ADD_TO_CART = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_store_flags_t,
    uint8_t, IDEATEK_STORE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_store_flags_t,
    uint8_t, IDEATEK_STORE_FLAGS_VALUES);

    #define IDEATEK_STORE_REQUEST_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t id; \
        ideatek_store_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_store_request_t,
    IDEATEK_STORE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_store_request_t,
    IDEATEK_STORE_REQUEST_FIELDS);

    #define IDEATEK_PLATFORM_OVERLAY_FIELDS { \
        bool active; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_platform_overlay_t,
    IDEATEK_PLATFORM_OVERLAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_platform_overlay_t,
    IDEATEK_PLATFORM_OVERLAY_FIELDS);

    #define IDEATEK_UGC_STATE_VALUES { \
        IDEATEK_UGC_STATE_SUBSCRIBED = 0x01, \
        IDEATEK_UGC_STATE_INSTALLED = 0x02, \
        IDEATEK_UGC_STATE_NEEDS_UPDATE = 0x04, \
        IDEATEK_UGC_STATE_DOWNLOADING = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_ugc_state_t,
    uint8_t, IDEATEK_UGC_STATE_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_ugc_state_t,
    uint8_t, IDEATEK_UGC_STATE_VALUES);

    #define IDEATEK_UGC_ITEM_FIELDS { \
        ideatek_entity_t user; \
        uint64_t bytes; \
        bridge_text_t id; \
        bridge_text_t title; \
        bridge_text_t path; \
        ideatek_ugc_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ugc_item_t, IDEATEK_UGC_ITEM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ugc_item_t, IDEATEK_UGC_ITEM_FIELDS);

    #define IDEATEK_UGC_SUBSCRIBE_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t id; \
        bool subscribe; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ugc_subscribe_t,
    IDEATEK_UGC_SUBSCRIBE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ugc_subscribe_t,
    IDEATEK_UGC_SUBSCRIBE_FIELDS);

    #define IDEATEK_UGC_PUBLISH_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t id; \
        bridge_text_t title; \
        bridge_text_t folder; \
        bridge_text_t note; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ugc_publish_t, IDEATEK_UGC_PUBLISH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ugc_publish_t, IDEATEK_UGC_PUBLISH_FIELDS);

    #define IDEATEK_UGC_RESULT_FIELDS { \
        bridge_text_t id; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ugc_result_t, IDEATEK_UGC_RESULT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ugc_result_t, IDEATEK_UGC_RESULT_FIELDS);

    #define IDEATEK_GAME_SERVER_FIELDS { \
        bridge_text_t name; \
        bridge_text_t map; \
        bridge_text_t tags; \
        bridge_text_t address; \
        uint32_t players; \
        uint32_t max_players; \
        uint32_t bots; \
        bool password; \
        bool dedicated; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_game_server_t, IDEATEK_GAME_SERVER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_game_server_t, IDEATEK_GAME_SERVER_FIELDS);

    #define IDEATEK_SERVER_LIST_VALUES { \
        IDEATEK_SERVER_LIST_INTERNET = 0, \
        IDEATEK_SERVER_LIST_LAN = 1, \
        IDEATEK_SERVER_LIST_FAVORITES = 2, \
        IDEATEK_SERVER_LIST_HISTORY = 3, \
        IDEATEK_SERVER_LIST_FRIENDS = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_server_list_t,
    uint8_t, IDEATEK_SERVER_LIST_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_server_list_t,
    uint8_t, IDEATEK_SERVER_LIST_VALUES);

    #define IDEATEK_SERVER_SEARCH_FIELDS { \
        bridge_text_t filter; \
        ideatek_server_list_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_server_search_t,
    IDEATEK_SERVER_SEARCH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_server_search_t,
    IDEATEK_SERVER_SEARCH_FIELDS);

    #define IDEATEK_SERVER_FOUND_FIELDS { \
        ideatek_entity_t search; \
        bridge_text_t name; \
        bridge_text_t map; \
        bridge_text_t tags; \
        bridge_text_t address; \
        uint32_t players; \
        uint32_t max_players; \
        uint32_t bots; \
        uint32_t ping; \
        bool password; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_server_found_t, IDEATEK_SERVER_FOUND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_server_found_t, IDEATEK_SERVER_FOUND_FIELDS);

    #define IDEATEK_PARENTAL_VALUES { \
        IDEATEK_PARENTAL_CHAT = 0x01, \
        IDEATEK_PARENTAL_UGC = 0x02, \
        IDEATEK_PARENTAL_ONLINE = 0x04, \
        IDEATEK_PARENTAL_STORE = 0x08, \
        IDEATEK_PARENTAL_PROFILES = 0x10 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_parental_t,
    uint8_t, IDEATEK_PARENTAL_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_parental_t,
    uint8_t, IDEATEK_PARENTAL_VALUES);

    #define IDEATEK_PARENTAL_LIMITS_FIELDS { \
        ideatek_parental_t blocked; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_parental_limits_t,
    IDEATEK_PARENTAL_LIMITS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_parental_limits_t,
    IDEATEK_PARENTAL_LIMITS_FIELDS);

    #define IDEATEK_TEXT_CONTEXT_VALUES { \
        IDEATEK_TEXT_CONTEXT_CONTENT = 0, \
        IDEATEK_TEXT_CONTEXT_CHAT = 1, \
        IDEATEK_TEXT_CONTEXT_NAME = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_context_t,
    uint8_t, IDEATEK_TEXT_CONTEXT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_context_t,
    uint8_t, IDEATEK_TEXT_CONTEXT_VALUES);

    #define IDEATEK_TEXT_FILTER_FIELDS { \
        ideatek_entity_t user; \
        bridge_text_t text; \
        ideatek_text_context_t context; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_filter_t, IDEATEK_TEXT_FILTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_filter_t, IDEATEK_TEXT_FILTER_FIELDS);

    #define IDEATEK_TEXT_FILTERED_FIELDS { \
        bridge_text_t text; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_filtered_t,
    IDEATEK_TEXT_FILTERED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_filtered_t,
    IDEATEK_TEXT_FILTERED_FIELDS);

    #define IDEATEK_OVERLAY_PAGE_VALUES { \
        IDEATEK_OVERLAY_PAGE_FRIENDS = 0, \
        IDEATEK_OVERLAY_PAGE_PROFILE = 1, \
        IDEATEK_OVERLAY_PAGE_INVITE = 2, \
        IDEATEK_OVERLAY_PAGE_ACHIEVEMENTS = 3, \
        IDEATEK_OVERLAY_PAGE_SETTINGS = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_overlay_page_t,
    uint8_t, IDEATEK_OVERLAY_PAGE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_overlay_page_t,
    uint8_t, IDEATEK_OVERLAY_PAGE_VALUES);

    #define IDEATEK_OVERLAY_REQUEST_FIELDS { \
        uint64_t account; \
        ideatek_entity_t lobby; \
        ideatek_overlay_page_t page; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_overlay_request_t,
    IDEATEK_OVERLAY_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_overlay_request_t,
    IDEATEK_OVERLAY_REQUEST_FIELDS);

    #define IDEATEK_PLATFORM_INFO_FIELDS { \
        bridge_text_t language; \
        bridge_text_t branch; \
        bool steam_deck; \
        bool big_picture; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_platform_info_t,
    IDEATEK_PLATFORM_INFO_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_platform_info_t,
    IDEATEK_PLATFORM_INFO_FIELDS);

    #define IDEATEK_TIMELINE_MODE_VALUES { \
        IDEATEK_TIMELINE_MODE_PLAYING = 0, \
        IDEATEK_TIMELINE_MODE_STAGING = 1, \
        IDEATEK_TIMELINE_MODE_MENUS = 2, \
        IDEATEK_TIMELINE_MODE_LOADING = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_timeline_mode_t,
    uint8_t, IDEATEK_TIMELINE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_timeline_mode_t,
    uint8_t, IDEATEK_TIMELINE_MODE_VALUES);

    #define IDEATEK_TIMELINE_MODE_RECORD_FIELDS { \
        ideatek_timeline_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_timeline_mode_record_t,
    IDEATEK_TIMELINE_MODE_RECORD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_timeline_mode_record_t,
    IDEATEK_TIMELINE_MODE_RECORD_FIELDS);

    #define IDEATEK_TIMELINE_CLIP_VALUES { \
        IDEATEK_TIMELINE_CLIP_NONE = 0, \
        IDEATEK_TIMELINE_CLIP_STANDARD = 1, \
        IDEATEK_TIMELINE_CLIP_FEATURED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_timeline_clip_t,
    uint8_t, IDEATEK_TIMELINE_CLIP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_timeline_clip_t,
    uint8_t, IDEATEK_TIMELINE_CLIP_VALUES);

    #define IDEATEK_TIMELINE_EVENT_FIELDS { \
        bridge_text_t title; \
        bridge_text_t description; \
        bridge_text_t icon; \
        float start_offset; \
        float duration; \
        uint32_t priority; \
        ideatek_timeline_clip_t clip; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_timeline_event_t,
    IDEATEK_TIMELINE_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_timeline_event_t,
    IDEATEK_TIMELINE_EVENT_FIELDS);

    #define IDEATEK_SCREENSHOT_REQUEST_FIELDS { \
        bridge_text_t location; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_screenshot_request_t,
    IDEATEK_SCREENSHOT_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_screenshot_request_t,
    IDEATEK_SCREENSHOT_REQUEST_FIELDS);

    #define IDEATEK_SCREENSHOT_TAKEN_FIELDS { \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_screenshot_taken_t,
    IDEATEK_SCREENSHOT_TAKEN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_screenshot_taken_t,
    IDEATEK_SCREENSHOT_TAKEN_FIELDS);

    #define IDEATEK_AVATAR_SIZE_VALUES { \
        IDEATEK_AVATAR_SIZE_SMALL = 0, \
        IDEATEK_AVATAR_SIZE_MEDIUM = 1, \
        IDEATEK_AVATAR_SIZE_LARGE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_avatar_size_t,
    uint8_t, IDEATEK_AVATAR_SIZE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_avatar_size_t,
    uint8_t, IDEATEK_AVATAR_SIZE_VALUES);

    #define IDEATEK_AVATAR_REQUEST_FIELDS { \
        uint64_t account; \
        ideatek_avatar_size_t size; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_avatar_request_t,
    IDEATEK_AVATAR_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_avatar_request_t,
    IDEATEK_AVATAR_REQUEST_FIELDS);

    #define IDEATEK_AVATAR_FIELDS { \
        bridge_text_t pixels; \
        uint32_t width; \
        uint32_t height; \
        ideatek_service_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_avatar_t, IDEATEK_AVATAR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_avatar_t, IDEATEK_AVATAR_FIELDS);

#endif
