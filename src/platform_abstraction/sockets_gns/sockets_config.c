/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>

#include "sockets_inside.h"

static const char RANGE_TIME[] = "1 to 2147483647";
static const char RANGE_BUFFER[] = "4096 to 268435456";
static const char RANGE_PERCENT[] = "0 to 100";
static const char RANGE_DELAY[] = "0 to 5000";
static const char RANGE_JITTER[] = "0 to 2000";
static const char RANGE_RATE[] = "0 to 1073741824";
static const char RANGE_BURST[] = "0 to 1048576";

const sockets_number_t SOCKETS_NUMBERS[] = {
    SOCKETS_NUMBER(message_bytes, 1048576, 1073741824, -1, false,
        "1048576 to 1073741824, an even number"),
    SOCKETS_NUMBER(timeout_initial, 1, 2147483647, GNS_CONFIG_TIMEOUT_INITIAL,
        false, RANGE_TIME),
    SOCKETS_NUMBER(timeout_connected, 1, 2147483647,
        GNS_CONFIG_TIMEOUT_CONNECTED, false, RANGE_TIME),
    SOCKETS_NUMBER(send_buffer, 4096, 268435456, GNS_CONFIG_SEND_BUFFER, false,
        RANGE_BUFFER),
    SOCKETS_NUMBER(recv_buffer, 4096, 268435456, GNS_CONFIG_RECV_BUFFER, false,
        "4096 to 268435456, more than recv_max_message"),
    SOCKETS_NUMBER(recv_buffer_messages, 2, 268435456,
        GNS_CONFIG_RECV_BUFFER_MESSAGES, false, "2 to 268435456"),
    SOCKETS_NUMBER(recv_max_message, 64, 268435456,
        GNS_CONFIG_RECV_MAX_MESSAGE, false, "64 to 268435456"),
    SOCKETS_NUMBER(nagle_time, 0, 20000, GNS_CONFIG_NAGLE_TIME, false,
        "0 to 20000"),
    SOCKETS_NUMBER(mtu, 200, 1300, GNS_CONFIG_MTU, false, "200 to 1300"),
    SOCKETS_NUMBER(ice_penalty, 0, 2147483647, GNS_CONFIG_ICE_PENALTY, false,
        "0 to 2147483647"),
    SOCKETS_NUMBER(fake_loss_send, 0, 100, GNS_CONFIG_FAKE_LOSS_SEND, true,
        RANGE_PERCENT),
    SOCKETS_NUMBER(fake_loss_recv, 0, 100, GNS_CONFIG_FAKE_LOSS_RECV, true,
        RANGE_PERCENT),
    SOCKETS_NUMBER(fake_lag_send, 0, 5000, GNS_CONFIG_FAKE_LAG_SEND, false,
        RANGE_DELAY),
    SOCKETS_NUMBER(fake_lag_recv, 0, 5000, GNS_CONFIG_FAKE_LAG_RECV, false,
        RANGE_DELAY),
    SOCKETS_NUMBER(fake_reorder_send, 0, 100, GNS_CONFIG_FAKE_REORDER_SEND,
        true, RANGE_PERCENT),
    SOCKETS_NUMBER(fake_reorder_recv, 0, 100, GNS_CONFIG_FAKE_REORDER_RECV,
        true, RANGE_PERCENT),
    SOCKETS_NUMBER(fake_reorder_time, 0, 5000, GNS_CONFIG_FAKE_REORDER_TIME,
        false, RANGE_DELAY),
    SOCKETS_NUMBER(fake_duplicate_send, 0, 100, GNS_CONFIG_FAKE_DUP_SEND, true,
        RANGE_PERCENT),
    SOCKETS_NUMBER(fake_duplicate_recv, 0, 100, GNS_CONFIG_FAKE_DUP_RECV, true,
        RANGE_PERCENT),
    SOCKETS_NUMBER(fake_duplicate_time_max, 0, 5000,
        GNS_CONFIG_FAKE_DUP_TIME_MAX, false, RANGE_DELAY),
    SOCKETS_NUMBER(fake_jitter_send_avg, 0, 2000,
        GNS_CONFIG_FAKE_JITTER_SEND_AVG, true, RANGE_JITTER),
    SOCKETS_NUMBER(fake_jitter_recv_avg, 0, 2000,
        GNS_CONFIG_FAKE_JITTER_RECV_AVG, true, RANGE_JITTER),
    SOCKETS_NUMBER(fake_jitter_send_max, 0, 5000,
        GNS_CONFIG_FAKE_JITTER_SEND_MAX, true, RANGE_DELAY),
    SOCKETS_NUMBER(fake_jitter_recv_max, 0, 5000,
        GNS_CONFIG_FAKE_JITTER_RECV_MAX, true, RANGE_DELAY),
    SOCKETS_NUMBER(fake_jitter_send_pct, 0, 100,
        GNS_CONFIG_FAKE_JITTER_SEND_PCT, true, RANGE_PERCENT),
    SOCKETS_NUMBER(fake_jitter_recv_pct, 0, 100,
        GNS_CONFIG_FAKE_JITTER_RECV_PCT, true, RANGE_PERCENT),
    SOCKETS_NUMBER(fake_rate_send, 0, 1073741824, GNS_CONFIG_FAKE_RATE_SEND,
        false, RANGE_RATE),
    SOCKETS_NUMBER(fake_rate_recv, 0, 1073741824, GNS_CONFIG_FAKE_RATE_RECV,
        false, RANGE_RATE),
    SOCKETS_NUMBER(fake_burst_send, 0, 1048576, GNS_CONFIG_FAKE_BURST_SEND,
        false, RANGE_BURST),
    SOCKETS_NUMBER(fake_burst_recv, 0, 1048576, GNS_CONFIG_FAKE_BURST_RECV,
        false, RANGE_BURST),
    SOCKETS_NUMBER(max_connections, 1, 4096, -1, false, "1 to 4096"),
    SOCKETS_NUMBER(max_listen_sockets, 1, 256, -1, false, "1 to 256"),
    SOCKETS_NUMBER(max_addresses, 1, 1024, -1, false, "1 to 1024"),
    SOCKETS_NUMBER(session_channels, 1, 1024, -1, false, "1 to 1024"),
};

const uint32_t SOCKETS_NUMBER_COUNT = sizeof(SOCKETS_NUMBERS)
    / sizeof(SOCKETS_NUMBERS[0]);

static void apply_numbers(const bridge_api_t *api, const sockets_settings_t *s)
{
    const sockets_number_t *n;
    uint32_t value;
    bool ok;

    for (uint32_t i = 0; i < SOCKETS_NUMBER_COUNT; i++) {
        n = &SOCKETS_NUMBERS[i];
        if (n->config < 0)
            continue;
        value = *(const uint32_t *)((const char *)s + n->offset);
        ok = n->as_float
            ? SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueFloat(
                sockets_module.utils, n->config, (float)value)
            : SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(
                sockets_module.utils, n->config, (int32_t)value);
        if (!ok)
            bridge_native_say(api, BRIDGE_LOG_WARN,
                "%s: refused by GameNetworkingSockets", n->name);
    }
}

static void apply_words(void)
{
    gns_utils_t *u = sockets_module.utils;
    char **t = sockets_module.texts;

    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(u,
        GNS_CONFIG_ALLOW_WITHOUT_AUTH, (int32_t)word_number(t[TEXT_ALLOW],
            ALLOW_WORDS, ALLOW_COUNT));
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(u,
        GNS_CONFIG_LOCALHOST_WITHOUT_AUTH,
        (int32_t)word_number(t[TEXT_ALLOW_LOCALHOST], ALLOW_WORDS,
            ALLOW_COUNT));
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(u,
        GNS_CONFIG_UNENCRYPTED, (int32_t)word_number(t[TEXT_UNENCRYPTED],
            UNENCRYPTED_WORDS, UNENCRYPTED_COUNT));
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(u,
        GNS_CONFIG_ICE_ENABLE, ice_mask(t[TEXT_ICE]));
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(u,
        GNS_CONFIG_ICE_IMPLEMENTATION,
        (int32_t)word_number(t[TEXT_ICE_IMPLEMENTATION], ICE_WORDS,
            ICE_COUNT));
}

static void apply_texts(void)
{
    gns_utils_t *u = sockets_module.utils;
    char **t = sockets_module.texts;

    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueString(u,
        GNS_CONFIG_STUN_SERVERS, t[TEXT_STUN]);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueString(u,
        GNS_CONFIG_TURN_SERVERS, t[TEXT_TURN]);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueString(u,
        GNS_CONFIG_TURN_USERS, t[TEXT_TURN_USERS]);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueString(u,
        GNS_CONFIG_TURN_PASSWORDS, t[TEXT_TURN_PASSWORDS]);
}

static void apply_callbacks(void)
{
    gns_utils_t *u = sockets_module.utils;

    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValuePtr(u,
        GNS_CONFIG_CALLBACK_CHANGED, (void *)on_changed);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValuePtr(u,
        GNS_CONFIG_CALLBACK_SESSION_REQUEST, (void *)on_session_request);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValuePtr(u,
        GNS_CONFIG_CALLBACK_SESSION_FAILED, (void *)on_session_failed);
    SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValuePtr(u,
        GNS_CONFIG_CALLBACK_SIGNALING, (void *)create_signaling);
}

void apply_settings(const bridge_api_t *api)
{
    const sockets_settings_t *s = bridge_native_get_single(api, SETTINGS);

    apply_numbers(api, s);
    apply_words();
    apply_texts();
    apply_callbacks();
}
