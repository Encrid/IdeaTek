# src/platform_abstraction/sockets_gns/sockets_inside.h

This header holds the state of the sockets module, its slots, its limits and its shared names.

## The state

The state is 1 global variable, `sockets_module`, as in the other modules. G4 of the style excepts it.
The thread of the network of GameNetworkingSockets writes `signals` and `lines` under `lock`. The worker thread of the names shares `names` under `lock`. The thread of the phases owns each other field.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `sockets`, `utils`, `messages` | The 3 interfaces of GameNetworkingSockets. |
| `self` | The sockets entity. |
| `identity` | The identity of the module, as `gns_identity_t`. |
| `certificate_request` | The 512 bytes of the arena for the request of a certificate. |
| `connections`, `connection_count` | The connections and the sessions, as `sockets_connection_t`, up to `max_connections`. |
| `listens`, `listen_count` | The listen sockets, as `sockets_listen_t`, up to `max_listen_sockets`. |
| `names`, `name_count` | The names that wait for their addresses, as `sockets_name_t`, up to `max_addresses`. |
| `thread`, `lock`, `wake`, `stop` | The worker thread of the names, the mutex of the shared fields, the event that wakes the thread, and the flag that ends it. |
| `half` | The 2 halves of `message_bytes`, as `sockets_half_t`. |
| `signals` | The ring of the signals that the thread of the network asks to send, as `sockets_ring_t`. |
| `lines` | The ring of the lines of GameNetworkingSockets, as `sockets_ring_t`. |
| `gone`, `gone_count` | The entities that the next round destroys: the closed connections and the results of the names. |
| `round` | The count of the rounds. |
| `lists` | The lists of the 9 observers, ordered as `docs/intern/platform_abstraction/sockets_gns/sockets_declare.c.md` declares them. |
| `channels` | The value of `session_channels`. |

## The connections

`sockets_connection_t` is 1 connection or 1 session from its first entity to its destroy:

| Field | Value |
| --- | --- |
| `entity` | The entity of the connection. |
| `handle` | The handle of GameNetworkingSockets. 0 for a session. |
| `peer` | The identity of the peer, as `gns_identity_t`. A session finds its connection by it. |
| `last` | The last `ideatek_connection_state_t` attached, for the compare. |
| `lanes` | The count of the lanes. 1 at first. |
| `failed_sends` | The messages that GameNetworkingSockets refused in the round. |
| `session` | `true` for a session. |
| `closed` | `true` after the close of the game. The next round destroys the entity. |

A slot is free when `entity` is 0. `docs/intern/platform_abstraction/sockets_gns/sockets_table.c.md` finds and takes the slots.

`sockets_listen_t` is 1 listen socket: its entity, its handle, and its kind.

`sockets_name_t` is 1 request of addresses: its entity, its name of up to `SOCKETS_NAME` bytes, and its addresses of up to `SOCKETS_ADDRESSES` with their count. It holds its status, and its state: `SOCKETS_FREE`, `SOCKETS_WAITING`, `SOCKETS_RUNNING` or `SOCKETS_DONE`.
`sockets_half_t` is the half of the module, shaped as in the files module. It keeps the place, the size, the bytes used and the current side.
`sockets_ring_t` is a ring of bytes in the arena: its place, its size, its head and its tail. A record of the ring is its length, then its bytes.

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `SOCKETS_NAME` | 256 | The bytes of a name, with its zero. A longer name gets `IDEATEK_ADDRESS_STATUS_FAILED`. |
| `SOCKETS_ADDRESSES` | 16 | The most addresses of a name. The OS can give more, and the rest is left. |
| `SOCKETS_SIGNAL_RING` | 65536 | The bytes of the ring of the signals. A signal that does not fit is refused, and GameNetworkingSockets sends it again. |
| `SOCKETS_LINE_RING` | 16384 | The bytes of the ring of the lines. A line that does not fit is lost. |
| `SOCKETS_LINE` | 256 | The most bytes of 1 line of GameNetworkingSockets. |
| `SOCKETS_TEXT` | 1024 | The bytes of a text setting, with its zero, and of a detailed status. |
| `SOCKETS_BATCH` | 64 | The messages that 1 call of GameNetworkingSockets gives. |
| `SOCKETS_STACK` | 262144 | The stack of the worker thread, in bytes. |
| `SOCKETS_WAIT` | 100000000 | The longest sleep of the worker thread between 2 looks at the names, in nanoseconds. |
| `CHANGE_HEAD` | 24 | The head of a change in `changes`. `CHANGE_SIZE` and `CHANGE_BYTES` add a value, rounded up to a multiple of 8. |

The tables take their room from the arena in the first round, with the values of the settings: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The slots

The slots keep the order of the declarations of the contract. The 11 scalar types are `ADDRESS_T`, `IDENTITY_T`, `SEND_T`, `FLAG_T`, `LISTEN_KIND_T`, `LISTEN_STATUS_T`, `ACTION_T`, `STATUS_T`, `AUTH_T`, `SESSION_ACTION_T` and `ADDRESS_STATUS_T`.
The components are `IDENTITY_REQUEST`, `IDENTITY_STATE`, `LISTEN_REQUEST`, `LISTEN_STATE`, `CONNECTION_REQUEST`, `INBOUND`, `CONNECTION_STATE`, `LANES_REQUEST`, `LANE_STATES`, `SESSION` and `SESSION_REQUEST`.
Then come `MESSAGES_OUT`, `MESSAGES_IN`, `SESSION_MESSAGES_OUT`, `SESSION_MESSAGES_IN`, `SIGNALS_OUT`, `SIGNALS_IN`, `ADDRESS_REQUEST` and `ADDRESS_RESULT`, then `SETTINGS`.
`sockets_settings_t` holds the settings of the contract in the order of its table, then the 5 settings of the module: `docs/extern/platform_abstraction/sockets_gns/readme.md`.

The header includes `gns_flat.h`, `sockets_values.h` and `ideatek_sockets.h`, and `os.h` of mosl for the thread, the mutex and the event.
