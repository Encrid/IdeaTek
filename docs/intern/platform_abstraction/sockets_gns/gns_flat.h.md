# src/platform_abstraction/sockets_gns/gns_flat.h

This header is the C face of GameNetworkingSockets: the structs and the functions of its flat API that the module uses.

## The rule

The headers of GameNetworkingSockets are C++. Its flat API is `extern "C"`, but some arguments are references.
This header declares each struct with the layout of `steamnetworkingtypes.h`, and each function with its name, with a pointer in the place of a reference.
A reference is a pointer in the ABI of each target of the project. The program `gns_layout` of the tests proves each layout against the headers of GameNetworkingSockets.

No source includes a header of GameNetworkingSockets. Only this header names its types, with the prefix `gns_`.

## The structs

| Struct | Of GameNetworkingSockets | Packing |
| --- | --- | --- |
| `gns_ip_t` | `SteamNetworkingIPAddr` | 1 |
| `gns_identity_t` | `SteamNetworkingIdentity` | 1 |
| `gns_config_t` | `SteamNetworkingConfigValue_t` | The pack of the system. |
| `gns_info_t` | `SteamNetConnectionInfo_t` | The pack of the system. |
| `gns_status_t` | `SteamNetConnectionRealTimeStatus_t` | The pack of the system. |
| `gns_lane_t` | `SteamNetConnectionRealTimeLaneStatus_t` | The pack of the system. |
| `gns_message_t` | `SteamNetworkingMessage_t` | The pack of the system. |
| `gns_changed_t` | `SteamNetConnectionStatusChangedCallback_t` | The pack of the system. |
| `gns_session_request_t` | `SteamNetworkingMessagesSessionRequest_t` | 1 |
| `gns_session_failed_t` | `SteamNetworkingMessagesSessionFailed_t` | 1 |

The pack of the system is 8 on Windows and 4 on Linux and macOS. `steamnetworkingtypes.h` packs its structs with `VALVE_CALLBACK_PACK_LARGE` or `VALVE_CALLBACK_PACK_SMALL` by the system, as the Steamworks SDK does. So a struct with a field of 8 bytes after a field of 4 bytes has a different size and different offsets on Windows.

The fields keep the names of GameNetworkingSockets without `m_`, in snake case. `gns_layout` proves the size, the alignment and each offset on each of the 6 targets.
`gns_identity_t` holds `type`, `size` and a union of 128 bytes: the text of a generic string, the bytes of a generic identity, or `gns_ip_t`.

## The constants

The header gives the values of GameNetworkingSockets that the module uses, as `constexpr`:

- The states of a connection, `GNS_STATE_*`: none, connecting, finding route, connected, closed by peer, and problem detected locally. The 2 ends are below 0.
- The types of an identity, `GNS_IDENTITY_*`: invalid, IP address, generic string, generic bytes.
- The flags of a connection, `GNS_FLAG_*`: unauthenticated, unencrypted, loopback, fast, relayed.
- The send flags, `GNS_SEND_*`: no Nagle, no delay, reliable, auto restart broken session. The unreliable send is 0.
- `GNS_RESULT_OK`, 1, the result of a call that worked.
- The config values, `GNS_CONFIG_*`: each one that `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md` names, the 3 options of a connection, and the 4 callbacks 201, 204, 205 and 206. `GNS_CONFIG_TYPE_INT32` and `GNS_CONFIG_TYPE_INT64` type an option.
- The end reasons `GNS_END_APP_MIN` (1000), `GNS_END_APP_MAX` (2999) and `GNS_END_MISC_GENERIC` (5001).
- The levels of the debug output, `GNS_LOG_*`, 0 to 8.
- `GNS_MAX_MESSAGE`, 524288, and `GNS_ERR_MSG`, 1024, the bytes of an error message.

`gns_layout` proves each constant against the enum of GameNetworkingSockets.

## The functions

The header declares the functions of the flat API that the module calls, with their names and with C types. A reference becomes a pointer.
So the tools of the fail run and of the leaks run see each call by its name: `docs/contributing/tests.md`, *Failed calls*.

| Group | Functions of the flat API |
| --- | --- |
| The life | `GameNetworkingSockets_Init`, `GameNetworkingSockets_Kill`, `SteamNetworkingSockets_SetManualPollMode`, `SteamNetworkingSockets_Poll`, `SteamAPI_SteamNetworkingSockets_v009`, `SteamAPI_SteamNetworkingUtils_v003`, `SteamAPI_SteamNetworkingMessages_v002` |
| The listen sockets | `CreateListenSocketIP`, `CreateListenSocketP2P`, `CloseListenSocket` |
| The connections | `ConnectByIPAddress`, `ConnectP2PCustomSignaling`, `CreateSocketPair`, `AcceptConnection`, `CloseConnection` |
| The messages | `AllocateMessage`, `SendMessages`, `FlushMessagesOnConnection`, `ReceiveMessagesOnConnection`, `SteamAPI_SteamNetworkingMessage_t_Release` |
| The states | `GetConnectionInfo`, `GetConnectionRealTimeStatus`, `GetDetailedConnectionStatus`, `ConfigureConnectionLanes`, `SetConnectionUserData` |
| The identity | `GetCertificateRequest`, `SetCertificate`, `SteamAPI_SteamNetworkingIdentity_ParseString`, `SteamAPI_SteamNetworkingIdentity_ToString` |
| The signals | `RunCallbacks`, `CreateCustomSignaling`, `ReceivedP2PCustomSignal2` |
| The config | `SetGlobalConfigValueInt32`, `SetGlobalConfigValueFloat`, `SetGlobalConfigValueString`, `SetGlobalConfigValuePtr`, `SetDebugOutputFunction` |
| The sessions | `SendMessageToUser`, `ReceiveMessagesOnChannel`, `AcceptSessionWithUser`, `CloseSessionWithUser`, `CloseChannelWithUser`, `GetSessionConnectionInfo` |

A function of the groups 2 to 7 has the prefix `SteamAPI_ISteamNetworkingSockets_`, except `AllocateMessage` and the config, which have `SteamAPI_ISteamNetworkingUtils_`. A function of the sessions has `SteamAPI_ISteamNetworkingMessages_`.
`SendMessages` takes a message of `AllocateMessage` with its lane, which `SendMessageToConnection` cannot take, and frees it.
The interfaces are opaque: `gns_sockets_t`, `gns_utils_t`, `gns_messages_t` and `gns_signaling_t` are structs with no body.
The prototypes keep the arguments of GameNetworkingSockets, so F5 of the style excepts the header.

## The callbacks

| Type | Of GameNetworkingSockets | Called by |
| --- | --- | --- |
| `gns_changed_fn_t` | `FnSteamNetConnectionStatusChanged` | `RunCallbacks`. |
| `gns_session_request_fn_t`, `gns_session_failed_fn_t` | `FnSteamNetworkingMessagesSessionRequest`, `FnSteamNetworkingMessagesSessionFailed` | `RunCallbacks`. |
| `gns_send_signal_fn_t`, `gns_release_fn_t` | `FSteamNetworkingSocketsCustomSignaling_SendSignal`, `_Release` | A connect, a send, or `SteamNetworkingSockets_Poll`. |
| `gns_on_connect_fn_t`, `gns_rejection_fn_t` | `FSteamNetworkingCustomSignalingRecvContext_OnConnectRequest`, `_SendRejectionSignal` | `ReceivedP2PCustomSignal2`. The module gives no rejection callback: `docs/intern/platform_abstraction/sockets_gns/sockets_signals_in.c.md`. |
| `gns_create_signaling_fn_t` | `FnSteamNetworkingSocketsCreateConnectionSignaling` | A session that sends its first message. |
| `gns_output_fn_t` | `FSteamNetworkingSocketsDebugOutput` | Any call of GameNetworkingSockets. |

GameNetworkingSockets runs in its manual poll mode: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*. So it calls each callback on the thread of the phases, from a call of the module.
The callbacks that cannot attach write into a ring: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
