# src/platform_abstraction/sockets_gns/gns_flat.h

This header is the C face of GameNetworkingSockets: the structs and the functions of its flat API that the module uses.

## The rule

The headers of GameNetworkingSockets are C++. Its flat API is `extern "C"`, but some arguments are references.
This header declares each struct with the layout of `steamnetworkingtypes.h`, and each function with its name, with a pointer in the place of a reference.
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

The header declares the functions of the flat API that the module calls, with their names and with C types. A reference becomes a pointer.
So the tools of the fail run and of the leaks run see each call by its name: `docs/contributing/tests.md`, *Failed calls* and *The leaks*.

| Group | Functions of the flat API |
| --- | --- |
| The life | `GameNetworkingSockets_Init`, `GameNetworkingSockets_Kill`, `SteamAPI_SteamNetworkingSockets_v009`, `SteamAPI_SteamNetworkingUtils_v003`, `SteamAPI_SteamNetworkingMessages_v002` |
| The listen sockets | `CreateListenSocketIP`, `CreateListenSocketP2P`, `CloseListenSocket` |
| The connections | `ConnectByIPAddress`, `ConnectP2PCustomSignaling`, `CreateSocketPair`, `AcceptConnection`, `CloseConnection` |
| The messages | `SendMessageToConnection`, `ReceiveMessagesOnConnection`, `SteamAPI_SteamNetworkingMessage_t_Release` |
| The states | `GetConnectionInfo`, `GetConnectionRealTimeStatus`, `GetDetailedConnectionStatus`, `ConfigureConnectionLanes` |
| The user data | `SetConnectionUserData`, `GetConnectionUserData` |
| The identity | `GetIdentity`, `GetCertificateRequest`, `SetCertificate`, `SteamAPI_SteamNetworkingIdentity_ParseString`, `SteamAPI_SteamNetworkingIdentity_ToString` |
| The signals | `RunCallbacks`, `CreateCustomSignaling`, `ReceivedP2PCustomSignal2` |
| The config | `SetGlobalConfigValueInt32`, `SetGlobalConfigValueFloat`, `SetGlobalConfigValueString`, `SetGlobalConfigValuePtr`, `SetConnectionConfigValueInt32`, `SetDebugOutputFunction` |
| The sessions | `SendMessageToUser`, `ReceiveMessagesOnChannel`, `AcceptSessionWithUser`, `CloseSessionWithUser`, `CloseChannelWithUser`, `GetSessionConnectionInfo` |

A function of the groups 2 to 9 has the prefix `SteamAPI_ISteamNetworkingSockets_` or `SteamAPI_ISteamNetworkingUtils_`, and a function of the sessions has `SteamAPI_ISteamNetworkingMessages_`.
The interfaces are opaque: `gns_sockets_t`, `gns_utils_t`, `gns_messages_t` and `gns_signaling_t` are structs with no body.

## The callbacks

| Type | Of GameNetworkingSockets | Called by |
| --- | --- | --- |
| `gns_changed_fn` | `FnSteamNetConnectionStatusChanged` | `RunCallbacks`, on the thread of the phases. |
| `gns_session_request_fn`, `gns_session_failed_fn` | `FnSteamNetworkingMessagesSessionRequest`, `FnSteamNetworkingMessagesSessionFailed` | `RunCallbacks`. |
| `gns_send_signal_fn`, `gns_release_fn` | `FSteamNetworkingSocketsCustomSignaling_SendSignal`, `_Release` | The thread of the network, at any time. |
| `gns_on_connect_fn`, `gns_rejection_fn` | `FSteamNetworkingCustomSignalingRecvContext_OnConnectRequest`, `_SendRejectionSignal` | `ReceivedP2PCustomSignal2`, on the thread of the phases. |
| `gns_create_signaling_fn` | `FnSteamNetworkingSocketsCreateConnectionSignaling` | A session or a P2P connect, on the thread of the phases. |
| `gns_output_fn` | `FSteamNetworkingSocketsDebugOutput` | Any thread. |

The callbacks that the thread of the network calls write into a ring under a lock: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
