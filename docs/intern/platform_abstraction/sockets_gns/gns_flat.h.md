# src/platform_abstraction/sockets_gns/gns_flat.h

This header is the C face of GameNetworkingSockets: the structs and the functions of its flat API that the module uses.

## The rule

The headers of GameNetworkingSockets are C++. Its flat API is `extern "C"`, but some arguments are references.
This header declares each struct with the layout of `steamnetworkingtypes.h`, and each function with a pointer in the place of a reference.
A reference is a pointer in the ABI of each target of the project. The program `gns_layout` of the tests proves each layout against the headers of GameNetworkingSockets.

No source includes a header of GameNetworkingSockets. Only this header names its types, with the prefix `gns_`.

## The structs

| Struct | Of GameNetworkingSockets | Bytes | Packing |
| --- | --- | --- | --- |
| `gns_ip_t` | `SteamNetworkingIPAddr` | 18 | 1 |
| `gns_identity_t` | `SteamNetworkingIdentity` | 136 | 8 |
| `gns_config_t` | `SteamNetworkingConfigValue_t` | 16 | 8 |
| `gns_info_t` | `SteamNetConnectionInfo_t` | 696 | 8 |
| `gns_status_t` | `SteamNetConnectionRealTimeStatus_t` | 120 | 8 |
| `gns_lane_t` | `SteamNetConnectionRealTimeLaneStatus_t` | 64 | 8 |
| `gns_message_t` | `SteamNetworkingMessage_t` | 232 | 8 |
| `gns_changed_t` | `SteamNetConnectionStatusChangedCallback_t` | 704 | 8 |
| `gns_session_request_t` | `SteamNetworkingMessagesSessionRequest_t` | 136 | 8 |
| `gns_session_failed_t` | `SteamNetworkingMessagesSessionFailed_t` | 696 | 8 |

The fields keep the names of GameNetworkingSockets without `m_`, in snake case. The bytes are the ones of the 6 targets, which `gns_layout` proves.
`gns_identity_t` holds `type`, `size` and a union of 128 bytes: the text of a generic string, the bytes of a generic identity, or `gns_ip_t`.

## The constants

The header gives the values of GameNetworkingSockets that the module uses, as `constexpr`:

- The states of a connection, `GNS_STATE_*`: none, connecting, finding route, connected, closed by peer, problem detected locally, and the 2 ends.
- The types of an identity, `GNS_IDENTITY_*`: invalid, IP address, generic string, generic bytes.
- The flags of a connection, `GNS_FLAG_*`: unauthenticated, unencrypted, loopback, fast, relayed.
- The send flags, `GNS_SEND_*`: unreliable, no Nagle, no delay, reliable, auto restart broken session.
- The config values, `GNS_CONFIG_*`: each one that `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md` names, and the 3 callbacks 204, 205 and 206.
- The end reasons `GNS_END_MISC_GENERIC` (5001) and `GNS_END_APP_MIN` (1000).
- The levels of the debug output, `GNS_LOG_*`, 0 to 8.
- `GNS_MAX_MESSAGE`, 524288, and `GNS_ERR_MSG`, 1024, the bytes of an error message.

## The functions

| Function of the header | Of the flat API |
| --- | --- |
| `gns_init`, `gns_kill` | `GameNetworkingSockets_Init`, `GameNetworkingSockets_Kill` |
| `gns_sockets`, `gns_utils`, `gns_messages` | `SteamAPI_SteamNetworkingSockets_v009`, `SteamAPI_SteamNetworkingUtils_v003`, `SteamAPI_SteamNetworkingMessages_v002` |
| `gns_listen_ip`, `gns_listen_p2p`, `gns_close_listen` | `CreateListenSocketIP`, `CreateListenSocketP2P`, `CloseListenSocket` |
| `gns_connect_ip`, `gns_connect_p2p`, `gns_pair` | `ConnectByIPAddress`, `ConnectP2PCustomSignaling`, `CreateSocketPair` |
| `gns_accept`, `gns_close` | `AcceptConnection`, `CloseConnection` |
| `gns_send`, `gns_receive`, `gns_release` | `SendMessageToConnection`, `ReceiveMessagesOnConnection`, `SteamNetworkingMessage_t_Release` |
| `gns_info`, `gns_status`, `gns_detailed` | `GetConnectionInfo`, `GetConnectionRealTimeStatus`, `GetDetailedConnectionStatus` |
| `gns_lanes` | `ConfigureConnectionLanes` |
| `gns_set_user_data`, `gns_user_data` | `SetConnectionUserData`, `GetConnectionUserData` |
| `gns_identity`, `gns_certificate_request`, `gns_set_certificate` | `GetIdentity`, `GetCertificateRequest`, `SetCertificate` |
| `gns_run_callbacks` | `RunCallbacks` |
| `gns_signaling`, `gns_received_signal` | `CreateCustomSignaling`, `ReceivedP2PCustomSignal2` |
| `gns_set_int`, `gns_set_float`, `gns_set_text`, `gns_set_pointer` | `SetGlobalConfigValueInt32`, `SetGlobalConfigValueFloat`, `SetGlobalConfigValueString`, `SetGlobalConfigValuePtr` |
| `gns_set_connection_int` | `SetConnectionConfigValueInt32` |
| `gns_debug_output` | `SetDebugOutputFunction` |
| `gns_session_send`, `gns_session_receive` | `SendMessageToUser`, `ReceiveMessagesOnChannel` |
| `gns_session_accept`, `gns_session_close`, `gns_session_close_channel`, `gns_session_info` | `AcceptSessionWithUser`, `CloseSessionWithUser`, `CloseChannelWithUser`, `GetSessionConnectionInfo` |
| `gns_identity_parse`, `gns_identity_text` | `SteamNetworkingIdentity_ParseString`, `SteamNetworkingIdentity_ToString` |
| `gns_ip_parse`, `gns_ip_text` | `SteamNetworkingIPAddr_ParseString`, `SteamNetworkingIPAddr_ToString` |

Each function of the header is a macro of the name of the flat API, or a declaration with the C types. So the module calls `gns_*` only.
The interfaces are opaque: `gns_sockets_t`, `gns_utils_t`, `gns_messages_t` and `gns_signaling_t` are structs with no body.

## The callbacks

| Type | Of GameNetworkingSockets | Called by |
| --- | --- | --- |
| `gns_changed_fn` | `FnSteamNetConnectionStatusChanged` | `gns_run_callbacks`, on the thread of the phases. |
| `gns_session_request_fn`, `gns_session_failed_fn` | `FnSteamNetworkingMessagesSessionRequest`, `FnSteamNetworkingMessagesSessionFailed` | `gns_run_callbacks`. |
| `gns_send_signal_fn`, `gns_release_fn` | `FSteamNetworkingSocketsCustomSignaling_SendSignal`, `_Release` | The thread of the network, at any time. |
| `gns_on_connect_fn`, `gns_rejection_fn` | `FSteamNetworkingCustomSignalingRecvContext_OnConnectRequest`, `_SendRejectionSignal` | `gns_received_signal`, on the thread of the phases. |
| `gns_create_signaling_fn` | `FnSteamNetworkingSocketsCreateConnectionSignaling` | A session or a P2P connect, on the thread of the phases. |
| `gns_output_fn` | `FSteamNetworkingSocketsDebugOutput` | Any thread. |

The callbacks that the thread of the network calls write into a ring under a lock: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
