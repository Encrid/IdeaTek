/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef GNS_FLAT_H
    #define GNS_FLAT_H

    #include <stddef.h>
    #include <stdint.h>

typedef struct gns_sockets_t gns_sockets_t;
typedef struct gns_utils_t gns_utils_t;
typedef struct gns_messages_t gns_messages_t;
typedef struct gns_signaling_t gns_signaling_t;

    #pragma pack(push, 1)

typedef struct {
    uint8_t bytes[16];
    uint16_t port;
} gns_ip_t;

typedef struct {
    int32_t type;
    int32_t size;
    union {
        char text[128];
        uint8_t bytes[128];
        gns_ip_t ip;
        uint64_t align;
    } u;
} gns_identity_t;

    #pragma pack(pop)

    #if defined(_WIN32)
        #pragma pack(push, 8)
    #else
        #pragma pack(push, 4)
    #endif

typedef struct {
    int32_t value;
    int32_t type;
    union {
        int32_t i32;
        int64_t i64;
        float f;
        const char *text;
        void *pointer;
    } u;
} gns_config_t;

typedef struct {
    gns_identity_t identity_remote;
    int64_t user_data;
    uint32_t listen_socket;
    gns_ip_t addr_remote;
    uint16_t pad1;
    uint32_t pop_remote;
    uint32_t pop_relay;
    int32_t state;
    int32_t end_reason;
    char end_debug[128];
    char description[128];
    int32_t flags;
    uint32_t reserved[63];
} gns_info_t;

typedef struct {
    int32_t state;
    int32_t ping;
    float quality_local;
    float quality_remote;
    float out_packets;
    float out_bytes;
    float in_packets;
    float in_bytes;
    int32_t send_rate;
    int32_t pending_unreliable;
    int32_t pending_reliable;
    int32_t sent_unacked_reliable;
    int64_t queue_time;
    int32_t max_jitter;
    uint32_t reserved[15];
} gns_status_t;

typedef struct {
    int32_t pending_unreliable;
    int32_t pending_reliable;
    int32_t sent_unacked_reliable;
    int32_t pad1;
    int64_t queue_time;
    uint32_t reserved[10];
} gns_lane_t;

typedef struct gns_message_t gns_message_t;

struct gns_message_t {
    void *data;
    int32_t size;
    uint32_t connection;
    gns_identity_t identity_peer;
    int64_t connection_user_data;
    int64_t time_received;
    int64_t message_number;
    void (*free_data)(gns_message_t *message);
    void (*release)(gns_message_t *message);
    int32_t channel;
    int32_t flags;
    int64_t user_data;
    uint16_t lane;
    uint16_t pad1;
};

typedef struct {
    uint32_t connection;
    gns_info_t info;
    int32_t old_state;
} gns_changed_t;

    #pragma pack(pop)

    #pragma pack(push, 1)

typedef struct {
    gns_identity_t identity_remote;
} gns_session_request_t;

typedef struct {
    gns_info_t info;
} gns_session_failed_t;

    #pragma pack(pop)

typedef void (*gns_changed_fn_t)(gns_changed_t *changed);
typedef void (*gns_session_request_fn_t)(gns_session_request_t *request);
typedef void (*gns_session_failed_fn_t)(gns_session_failed_t *failed);
typedef bool (*gns_send_signal_fn_t)(void *context, uint32_t connection,
    const gns_info_t *info, const void *bytes, int len);
typedef void (*gns_release_fn_t)(void *context);
typedef gns_signaling_t *(*gns_on_connect_fn_t)(void *context,
    uint32_t connection, const gns_identity_t *peer, int virtual_port);
typedef void (*gns_rejection_fn_t)(void *context, const gns_identity_t *peer,
    const void *bytes, int len);
typedef gns_signaling_t *(*gns_create_signaling_fn_t)(gns_sockets_t *sockets,
    const gns_identity_t *peer, int local_port, int remote_port);
typedef void (*gns_output_fn_t)(int level, const char *text);

constexpr int32_t GNS_STATE_NONE = 0;
constexpr int32_t GNS_STATE_CONNECTING = 1;
constexpr int32_t GNS_STATE_FINDING_ROUTE = 2;
constexpr int32_t GNS_STATE_CONNECTED = 3;
constexpr int32_t GNS_STATE_CLOSED_BY_PEER = 4;
constexpr int32_t GNS_STATE_PROBLEM = 5;

constexpr int32_t GNS_IDENTITY_INVALID = 0;
constexpr int32_t GNS_IDENTITY_IP = 1;
constexpr int32_t GNS_IDENTITY_STRING = 2;
constexpr int32_t GNS_IDENTITY_BYTES = 3;

constexpr int32_t GNS_FLAG_UNAUTHENTICATED = 1;
constexpr int32_t GNS_FLAG_UNENCRYPTED = 2;
constexpr int32_t GNS_FLAG_LOOPBACK = 4;
constexpr int32_t GNS_FLAG_FAST = 8;
constexpr int32_t GNS_FLAG_RELAYED = 16;

constexpr int GNS_SEND_NO_NAGLE = 1;
constexpr int GNS_SEND_NO_DELAY = 4;
constexpr int GNS_SEND_RELIABLE = 8;
constexpr int GNS_SEND_AUTO_RESTART = 32;

constexpr int GNS_RESULT_OK = 1;
constexpr int GNS_END_APP_MIN = 1000;
constexpr int GNS_END_APP_MAX = 2999;
constexpr int GNS_END_MISC_GENERIC = 5001;

constexpr int GNS_CONFIG_FAKE_LOSS_SEND = 2;
constexpr int GNS_CONFIG_FAKE_LOSS_RECV = 3;
constexpr int GNS_CONFIG_FAKE_LAG_SEND = 4;
constexpr int GNS_CONFIG_FAKE_LAG_RECV = 5;
constexpr int GNS_CONFIG_FAKE_REORDER_SEND = 6;
constexpr int GNS_CONFIG_FAKE_REORDER_RECV = 7;
constexpr int GNS_CONFIG_FAKE_REORDER_TIME = 8;
constexpr int GNS_CONFIG_SEND_BUFFER = 9;
constexpr int GNS_CONFIG_SEND_RATE_MIN = 10;
constexpr int GNS_CONFIG_SEND_RATE_MAX = 11;
constexpr int GNS_CONFIG_NAGLE_TIME = 12;
constexpr int GNS_CONFIG_ALLOW_WITHOUT_AUTH = 23;
constexpr int GNS_CONFIG_TIMEOUT_INITIAL = 24;
constexpr int GNS_CONFIG_TIMEOUT_CONNECTED = 25;
constexpr int GNS_CONFIG_FAKE_DUP_SEND = 26;
constexpr int GNS_CONFIG_FAKE_DUP_RECV = 27;
constexpr int GNS_CONFIG_FAKE_DUP_TIME_MAX = 28;
constexpr int GNS_CONFIG_MTU = 32;
constexpr int GNS_CONFIG_UNENCRYPTED = 34;
constexpr int GNS_CONFIG_SYMMETRIC = 37;
constexpr int GNS_CONFIG_USER_DATA = 40;
constexpr int GNS_CONFIG_FAKE_RATE_SEND = 42;
constexpr int GNS_CONFIG_FAKE_BURST_SEND = 43;
constexpr int GNS_CONFIG_FAKE_RATE_RECV = 44;
constexpr int GNS_CONFIG_FAKE_BURST_RECV = 45;
constexpr int GNS_CONFIG_RECV_BUFFER = 47;
constexpr int GNS_CONFIG_RECV_BUFFER_MESSAGES = 48;
constexpr int GNS_CONFIG_RECV_MAX_MESSAGE = 49;
constexpr int GNS_CONFIG_LOCALHOST_WITHOUT_AUTH = 52;
constexpr int GNS_CONFIG_FAKE_JITTER_SEND_AVG = 53;
constexpr int GNS_CONFIG_FAKE_JITTER_SEND_MAX = 54;
constexpr int GNS_CONFIG_FAKE_JITTER_SEND_PCT = 55;
constexpr int GNS_CONFIG_FAKE_JITTER_RECV_AVG = 56;
constexpr int GNS_CONFIG_FAKE_JITTER_RECV_MAX = 57;
constexpr int GNS_CONFIG_FAKE_JITTER_RECV_PCT = 58;
constexpr int GNS_CONFIG_STUN_SERVERS = 103;
constexpr int GNS_CONFIG_ICE_ENABLE = 104;
constexpr int GNS_CONFIG_ICE_PENALTY = 105;
constexpr int GNS_CONFIG_TURN_SERVERS = 107;
constexpr int GNS_CONFIG_TURN_USERS = 108;
constexpr int GNS_CONFIG_TURN_PASSWORDS = 109;
constexpr int GNS_CONFIG_ICE_IMPLEMENTATION = 110;
constexpr int GNS_CONFIG_CALLBACK_CHANGED = 201;
constexpr int GNS_CONFIG_CALLBACK_SESSION_REQUEST = 204;
constexpr int GNS_CONFIG_CALLBACK_SESSION_FAILED = 205;
constexpr int GNS_CONFIG_CALLBACK_SIGNALING = 206;

constexpr int GNS_CONFIG_TYPE_INT32 = 1;
constexpr int GNS_CONFIG_TYPE_INT64 = 2;

constexpr int GNS_LOG_NONE = 0;
constexpr int GNS_LOG_BUG = 1;
constexpr int GNS_LOG_ERROR = 2;
constexpr int GNS_LOG_IMPORTANT = 3;
constexpr int GNS_LOG_WARNING = 4;
constexpr int GNS_LOG_EVERYTHING = 8;

constexpr uint32_t GNS_MAX_MESSAGE = 524288;
constexpr uint32_t GNS_ERR_MSG = 1024;

    #if !defined(GNS_FLAT_TYPES_ONLY)

bool GameNetworkingSockets_Init(const gns_identity_t *identity, char *error);
void GameNetworkingSockets_Kill(void);
void SteamNetworkingSockets_SetManualPollMode(bool manual);
void SteamNetworkingSockets_Poll(int most_wait);
gns_sockets_t *SteamAPI_SteamNetworkingSockets_v009(void);
gns_utils_t *SteamAPI_SteamNetworkingUtils_v003(void);
gns_messages_t *SteamAPI_SteamNetworkingMessages_v002(void);

uint32_t SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP(
    gns_sockets_t *sockets, const gns_ip_t *address, int count,
    const gns_config_t *options);
uint32_t SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P(
    gns_sockets_t *sockets, int virtual_port, int count,
    const gns_config_t *options);
bool SteamAPI_ISteamNetworkingSockets_CloseListenSocket(gns_sockets_t *sockets,
    uint32_t listen);
uint32_t SteamAPI_ISteamNetworkingSockets_ConnectByIPAddress(
    gns_sockets_t *sockets, const gns_ip_t *address, int count,
    const gns_config_t *options);
uint32_t SteamAPI_ISteamNetworkingSockets_ConnectP2PCustomSignaling(
    gns_sockets_t *sockets, gns_signaling_t *signaling,
    const gns_identity_t *peer, int virtual_port, int count,
    const gns_config_t *options);
bool SteamAPI_ISteamNetworkingSockets_CreateSocketPair(gns_sockets_t *sockets,
    uint32_t *first, uint32_t *second, bool network,
    const gns_identity_t *identity_first,
    const gns_identity_t *identity_second);
int SteamAPI_ISteamNetworkingSockets_AcceptConnection(gns_sockets_t *sockets,
    uint32_t connection);
bool SteamAPI_ISteamNetworkingSockets_CloseConnection(gns_sockets_t *sockets,
    uint32_t connection, int reason, const char *text, bool linger);
int SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection(
    gns_sockets_t *sockets, uint32_t connection, gns_message_t **messages,
    int most);
void SteamAPI_SteamNetworkingMessage_t_Release(gns_message_t *message);
int SteamAPI_ISteamNetworkingSockets_FlushMessagesOnConnection(
    gns_sockets_t *sockets, uint32_t connection);
gns_message_t *SteamAPI_ISteamNetworkingUtils_AllocateMessage(
    gns_utils_t *utils, int len);
void SteamAPI_ISteamNetworkingSockets_SendMessages(gns_sockets_t *sockets,
    int count, gns_message_t **messages, int64_t *results,
    bool delete_failed);
bool SteamAPI_ISteamNetworkingSockets_GetConnectionInfo(gns_sockets_t *sockets,
    uint32_t connection, gns_info_t *info);
int SteamAPI_ISteamNetworkingSockets_GetConnectionRealTimeStatus(
    gns_sockets_t *sockets, uint32_t connection, gns_status_t *status,
    int lanes, gns_lane_t *lane_status);
int SteamAPI_ISteamNetworkingSockets_GetDetailedConnectionStatus(
    gns_sockets_t *sockets, uint32_t connection, char *text, int size);
int SteamAPI_ISteamNetworkingSockets_ConfigureConnectionLanes(
    gns_sockets_t *sockets, uint32_t connection, int count,
    const int *priorities, const uint16_t *weights);
bool SteamAPI_ISteamNetworkingSockets_SetConnectionUserData(
    gns_sockets_t *sockets, uint32_t connection, int64_t user_data);
bool SteamAPI_ISteamNetworkingSockets_GetCertificateRequest(
    gns_sockets_t *sockets, int *len, void *bytes, char *error);
bool SteamAPI_ISteamNetworkingSockets_SetCertificate(gns_sockets_t *sockets,
    const void *bytes, int len, char *error);
void SteamAPI_ISteamNetworkingSockets_RunCallbacks(gns_sockets_t *sockets);
gns_signaling_t *SteamAPI_ISteamNetworkingSockets_CreateCustomSignaling(
    void *context, gns_send_signal_fn_t send, gns_release_fn_t release);
bool SteamAPI_ISteamNetworkingSockets_ReceivedP2PCustomSignal2(
    gns_sockets_t *sockets, const void *bytes, int len, void *context,
    gns_on_connect_fn_t on_connect, gns_rejection_fn_t rejection);

bool SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueInt32(
    gns_utils_t *utils, int value, int32_t number);
bool SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueFloat(
    gns_utils_t *utils, int value, float number);
bool SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValueString(
    gns_utils_t *utils, int value, const char *text);
bool SteamAPI_ISteamNetworkingUtils_SetGlobalConfigValuePtr(gns_utils_t *utils,
    int value, void *pointer);
void SteamAPI_ISteamNetworkingUtils_SetDebugOutputFunction(gns_utils_t *utils,
    int level, gns_output_fn_t output);

int SteamAPI_ISteamNetworkingMessages_SendMessageToUser(
    gns_messages_t *messages, const gns_identity_t *peer, const void *bytes,
    uint32_t len, int flags, int channel);
int SteamAPI_ISteamNetworkingMessages_ReceiveMessagesOnChannel(
    gns_messages_t *messages, int channel, gns_message_t **out, int most);
bool SteamAPI_ISteamNetworkingMessages_AcceptSessionWithUser(
    gns_messages_t *messages, const gns_identity_t *peer);
bool SteamAPI_ISteamNetworkingMessages_CloseSessionWithUser(
    gns_messages_t *messages, const gns_identity_t *peer);
bool SteamAPI_ISteamNetworkingMessages_CloseChannelWithUser(
    gns_messages_t *messages, const gns_identity_t *peer, int channel);
int SteamAPI_ISteamNetworkingMessages_GetSessionConnectionInfo(
    gns_messages_t *messages, const gns_identity_t *peer, gns_info_t *info,
    gns_status_t *status);

bool SteamAPI_SteamNetworkingIdentity_ParseString(gns_identity_t *identity,
    size_t size, const char *text);
void SteamAPI_SteamNetworkingIdentity_ToString(const gns_identity_t *identity,
    char *text, size_t size);

    #endif

#endif
