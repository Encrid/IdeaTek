/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <cstddef>
#include <cstdio>

#define GNS_FLAT_TYPES_ONLY
#include "gns_flat.h"

#include <steam/isteamnetworkingmessages.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

#define SAME_SIZE(ours, theirs) \
    static_assert(sizeof(ours) == sizeof(theirs), #ours); \
    static_assert(alignof(ours) == alignof(theirs), #ours)
#define SAME_FIELD(ours, mine, theirs, its) \
    static_assert(offsetof(ours, mine) == offsetof(theirs, its), #mine)

SAME_SIZE(gns_ip_t, SteamNetworkingIPAddr);
SAME_FIELD(gns_ip_t, port, SteamNetworkingIPAddr, m_port);
SAME_SIZE(gns_identity_t, SteamNetworkingIdentity);
SAME_FIELD(gns_identity_t, type, SteamNetworkingIdentity, m_eType);
SAME_FIELD(gns_identity_t, size, SteamNetworkingIdentity, m_cbSize);
SAME_FIELD(gns_identity_t, u, SteamNetworkingIdentity, m_szGenericString);
SAME_SIZE(gns_config_t, SteamNetworkingConfigValue_t);
SAME_FIELD(gns_config_t, value, SteamNetworkingConfigValue_t, m_eValue);
SAME_FIELD(gns_config_t, type, SteamNetworkingConfigValue_t, m_eDataType);
SAME_FIELD(gns_config_t, u, SteamNetworkingConfigValue_t, m_val);
SAME_SIZE(gns_info_t, SteamNetConnectionInfo_t);
SAME_FIELD(gns_info_t, identity_remote, SteamNetConnectionInfo_t,
    m_identityRemote);
SAME_FIELD(gns_info_t, user_data, SteamNetConnectionInfo_t, m_nUserData);
SAME_FIELD(gns_info_t, listen_socket, SteamNetConnectionInfo_t,
    m_hListenSocket);
SAME_FIELD(gns_info_t, addr_remote, SteamNetConnectionInfo_t, m_addrRemote);
SAME_FIELD(gns_info_t, state, SteamNetConnectionInfo_t, m_eState);
SAME_FIELD(gns_info_t, end_reason, SteamNetConnectionInfo_t, m_eEndReason);
SAME_FIELD(gns_info_t, end_debug, SteamNetConnectionInfo_t, m_szEndDebug);
SAME_FIELD(gns_info_t, description, SteamNetConnectionInfo_t,
    m_szConnectionDescription);
SAME_FIELD(gns_info_t, flags, SteamNetConnectionInfo_t, m_nFlags);
SAME_SIZE(gns_status_t, SteamNetConnectionRealTimeStatus_t);
SAME_FIELD(gns_status_t, state, SteamNetConnectionRealTimeStatus_t, m_eState);
SAME_FIELD(gns_status_t, ping, SteamNetConnectionRealTimeStatus_t, m_nPing);
SAME_FIELD(gns_status_t, quality_local, SteamNetConnectionRealTimeStatus_t,
    m_flConnectionQualityLocal);
SAME_FIELD(gns_status_t, quality_remote, SteamNetConnectionRealTimeStatus_t,
    m_flConnectionQualityRemote);
SAME_FIELD(gns_status_t, out_packets, SteamNetConnectionRealTimeStatus_t,
    m_flOutPacketsPerSec);
SAME_FIELD(gns_status_t, out_bytes, SteamNetConnectionRealTimeStatus_t,
    m_flOutBytesPerSec);
SAME_FIELD(gns_status_t, in_packets, SteamNetConnectionRealTimeStatus_t,
    m_flInPacketsPerSec);
SAME_FIELD(gns_status_t, in_bytes, SteamNetConnectionRealTimeStatus_t,
    m_flInBytesPerSec);
SAME_FIELD(gns_status_t, send_rate, SteamNetConnectionRealTimeStatus_t,
    m_nSendRateBytesPerSecond);
SAME_FIELD(gns_status_t, pending_unreliable,
    SteamNetConnectionRealTimeStatus_t, m_cbPendingUnreliable);
SAME_FIELD(gns_status_t, pending_reliable, SteamNetConnectionRealTimeStatus_t,
    m_cbPendingReliable);
SAME_FIELD(gns_status_t, sent_unacked_reliable,
    SteamNetConnectionRealTimeStatus_t, m_cbSentUnackedReliable);
SAME_FIELD(gns_status_t, queue_time, SteamNetConnectionRealTimeStatus_t,
    m_usecQueueTime);
SAME_FIELD(gns_status_t, max_jitter, SteamNetConnectionRealTimeStatus_t,
    m_usecMaxJitter);
SAME_SIZE(gns_lane_t, SteamNetConnectionRealTimeLaneStatus_t);
SAME_FIELD(gns_lane_t, pending_unreliable,
    SteamNetConnectionRealTimeLaneStatus_t, m_cbPendingUnreliable);
SAME_FIELD(gns_lane_t, pending_reliable,
    SteamNetConnectionRealTimeLaneStatus_t, m_cbPendingReliable);
SAME_FIELD(gns_lane_t, sent_unacked_reliable,
    SteamNetConnectionRealTimeLaneStatus_t, m_cbSentUnackedReliable);
SAME_FIELD(gns_lane_t, queue_time, SteamNetConnectionRealTimeLaneStatus_t,
    m_usecQueueTime);
SAME_SIZE(gns_message_t, SteamNetworkingMessage_t);
SAME_FIELD(gns_message_t, data, SteamNetworkingMessage_t, m_pData);
SAME_FIELD(gns_message_t, size, SteamNetworkingMessage_t, m_cbSize);
SAME_FIELD(gns_message_t, connection, SteamNetworkingMessage_t, m_conn);
SAME_FIELD(gns_message_t, identity_peer, SteamNetworkingMessage_t,
    m_identityPeer);
SAME_FIELD(gns_message_t, connection_user_data, SteamNetworkingMessage_t,
    m_nConnUserData);
SAME_FIELD(gns_message_t, time_received, SteamNetworkingMessage_t,
    m_usecTimeReceived);
SAME_FIELD(gns_message_t, message_number, SteamNetworkingMessage_t,
    m_nMessageNumber);
SAME_FIELD(gns_message_t, free_data, SteamNetworkingMessage_t, m_pfnFreeData);
SAME_FIELD(gns_message_t, release, SteamNetworkingMessage_t, m_pfnRelease);
SAME_FIELD(gns_message_t, channel, SteamNetworkingMessage_t, m_nChannel);
SAME_FIELD(gns_message_t, flags, SteamNetworkingMessage_t, m_nFlags);
SAME_FIELD(gns_message_t, user_data, SteamNetworkingMessage_t, m_nUserData);
SAME_FIELD(gns_message_t, lane, SteamNetworkingMessage_t, m_idxLane);
SAME_SIZE(gns_changed_t, SteamNetConnectionStatusChangedCallback_t);
SAME_FIELD(gns_changed_t, connection,
    SteamNetConnectionStatusChangedCallback_t, m_hConn);
SAME_FIELD(gns_changed_t, info, SteamNetConnectionStatusChangedCallback_t,
    m_info);
SAME_FIELD(gns_changed_t, old_state,
    SteamNetConnectionStatusChangedCallback_t, m_eOldState);
SAME_SIZE(gns_session_request_t, SteamNetworkingMessagesSessionRequest_t);
SAME_FIELD(gns_session_request_t, identity_remote,
    SteamNetworkingMessagesSessionRequest_t, m_identityRemote);
SAME_SIZE(gns_session_failed_t, SteamNetworkingMessagesSessionFailed_t);
SAME_FIELD(gns_session_failed_t, info,
    SteamNetworkingMessagesSessionFailed_t, m_info);

static_assert(GNS_STATE_CONNECTED
    == k_ESteamNetworkingConnectionState_Connected);
static_assert(GNS_STATE_PROBLEM
    == k_ESteamNetworkingConnectionState_ProblemDetectedLocally);
static_assert(GNS_IDENTITY_IP == k_ESteamNetworkingIdentityType_IPAddress);
static_assert(GNS_FLAG_LOOPBACK
    == k_nSteamNetworkConnectionInfoFlags_LoopbackBuffers);
static_assert(GNS_FLAG_RELAYED == k_nSteamNetworkConnectionInfoFlags_Relayed);
static_assert(GNS_SEND_RELIABLE == k_nSteamNetworkingSend_Reliable);
static_assert(GNS_SEND_AUTO_RESTART
    == k_nSteamNetworkingSend_AutoRestartBrokenSession);
static_assert(GNS_RESULT_OK == k_EResultOK);
static_assert(GNS_END_MISC_GENERIC == k_ESteamNetConnectionEnd_Misc_Generic);
static_assert(GNS_CONFIG_TIMEOUT_INITIAL
    == k_ESteamNetworkingConfig_TimeoutInitial);
static_assert(GNS_CONFIG_RECV_MAX_MESSAGE
    == k_ESteamNetworkingConfig_RecvMaxMessageSize);
static_assert(GNS_CONFIG_USER_DATA
    == k_ESteamNetworkingConfig_ConnectionUserData);
static_assert(GNS_CONFIG_FAKE_JITTER_RECV_PCT
    == k_ESteamNetworkingConfig_FakePacketJitter_Recv_Pct);
static_assert(GNS_CONFIG_ICE_IMPLEMENTATION
    == k_ESteamNetworkingConfig_P2P_Transport_ICE_Implementation);
static_assert(GNS_CONFIG_CALLBACK_SIGNALING
    == k_ESteamNetworkingConfig_Callback_CreateConnectionSignaling);
static_assert(GNS_CONFIG_TYPE_INT64 == k_ESteamNetworkingConfig_Int64);
static_assert(GNS_LOG_EVERYTHING
    == k_ESteamNetworkingSocketsDebugOutputType_Everything);
static_assert(GNS_MAX_MESSAGE == k_cbMaxSteamNetworkingSocketsMessageSizeSend);
static_assert(GNS_ERR_MSG == k_cchMaxSteamNetworkingErrMsg);

int main(int argc, char **)
{
    if (argc > 1) {
        std::printf("layout not checked\n");
        return (1);
    }
    std::printf("layout ok\n");
    return (0);
}
