# src/platform_abstraction/sockets_gns/sockets_inside.h

This header holds the state of the sockets module, its slots, its limits and its shared names.

## The state

The state is 1 global variable, `sockets_module`, as in the other modules. G4 of the style excepts it.
The worker thread of the names shares `names` and `stop` under `lock`. The thread of the phases owns each other field. GameNetworkingSockets runs in its manual poll mode, so it calls the module on that thread only: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*.

| Field | Value |
| --- | --- |
| `api` | The api of Bridge, for the callbacks of GameNetworkingSockets, which get none. |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `sockets`, `utils`, `messages` | The 3 interfaces of GameNetworkingSockets. |
| `self` | The sockets entity. |
| `identity` | The identity of the module, as `gns_identity_t`. |
| `certificate_at` | The place of the `SOCKETS_CERTIFICATE` bytes of the arena for the request of a certificate. |
| `auth_status` | The status of the certificate, as `ideatek_auth_status_t`. |
| `connections`, `connection_max` | The connections and the sessions, as `sockets_connection_t`, up to `max_connections`. |
| `listens`, `listen_max` | The listen sockets, as `sockets_listen_t`, up to `max_listen_sockets`. |
| `names`, `name_max` | The names that wait for their addresses, as `sockets_name_t`, up to `max_addresses`. |
| `channels` | The value of `session_channels`. |
| `texts`, `debug_level` | The texts of the 9 text settings, by their `TEXT_*` index, and of `debug_output`. |
| `thread`, `lock`, `wake`, `stop` | The worker thread of the names, the mutex of the shared fields, the event that wakes the thread, and the flag that ends it. |
| `half` | The 2 halves of `message_bytes`, as `sockets_half_t`. |
| `signals` | The ring of the signals that GameNetworkingSockets asks to send, as `sockets_ring_t`. |
| `lines` | The ring of the lines of GameNetworkingSockets, as `sockets_ring_t`. |
| `gone`, `gone_count` | The entities that this round destroys. |
| `later`, `later_count` | The entities that the next round moves into `gone`: `docs/intern/platform_abstraction/sockets_gns/sockets_half.c.md`. |
| `lists` | The lists of the 9 observers, ordered as `docs/intern/platform_abstraction/sockets_gns/sockets_runs.c.md` declares them, by their `LIST_*` index. |

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
| `closed` | `true` after the close of the game, or the end of a session. A later round destroys the entity. |
| `symmetric` | `true` for a symmetric connection, which cannot linger at its close. |
| `forgotten` | `true` after an identity change: GameNetworkingSockets forgot the handle. |

A slot is free when `entity` is 0. `docs/intern/platform_abstraction/sockets_gns/sockets_table.c.md` finds and takes the slots.

`sockets_listen_t` is 1 listen socket: its entity, its handle, and its kind.

`sockets_name_t` is 1 request of addresses: its entity, its name of up to `SOCKETS_NAME` bytes, and its addresses of up to `SOCKETS_ADDRESSES` with their count. It holds its status, and its state: `SOCKETS_FREE`, `SOCKETS_WAITING`, `SOCKETS_RUNNING` or `SOCKETS_DONE`.
`sockets_half_t` is the half of the module, shaped as in the files module. It keeps the place, the size, the bytes used and the current side.
`sockets_ring_t` is in `sockets_values.h`, with the pure parts: `docs/intern/platform_abstraction/sockets_gns/sockets_ring.c.md`.

`sockets_number_t` is 1 row of the table of the number settings. Each row keeps the offset of the field, its range, its config value of GameNetworkingSockets, its name and the text of its range. `as_float` tells that the value goes to GameNetworkingSockets as a float. `SOCKETS_NUMBER` writes a row from a field name: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `SOCKETS_NAME` | 256 | The bytes of a name, with its zero. A longer name gets `IDEATEK_ADDRESS_STATUS_FAILED`. |
| `SOCKETS_ADDRESSES` | 16 | The most addresses of a name. The OS can give more, and the rest is left. |
| `SOCKETS_SIGNAL_RING` | 65536 | The bytes of the ring of the signals. A signal that does not fit is refused, and GameNetworkingSockets sends it again. |
| `SOCKETS_SIGNAL` | 4096 | The most bytes of 1 signal. |
| `SOCKETS_LINE_RING` | 16384 | The bytes of the ring of the lines. A line that does not fit is lost. |
| `SOCKETS_LINE` | 256 | The most bytes of 1 line of GameNetworkingSockets. |
| `SOCKETS_TEXT` | 1024 | The bytes of a text setting, with its zero, and of a detailed status. |
| `SOCKETS_TEXTS` | 9 | The text settings of the contract. |
| `SOCKETS_CERTIFICATE` | 512 | The bytes of the request of a certificate. |
| `SOCKETS_BATCH` | 64 | The messages that 1 call of GameNetworkingSockets gives. |
| `SOCKETS_STACK` | 262144 | The stack of the worker thread, in bytes. |
| `SOCKETS_WAIT` | 100000000 | The longest sleep of the worker thread between 2 looks at the names, in nanoseconds. |
| `CHANGE_HEAD` | 24 | The head of a change in `changes`. `CHANGE_SIZE` and `CHANGE_BYTES` add a value, rounded up to a multiple of 8. |
| `ON_REQUEST` | `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` | The events of an observer of a request. |

The tables take their room from the arena in the first round, with the values of the settings: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The slots

The slots keep the order of the declarations of the contract. The 12 scalar types are `ENTITY_T`, `ADDRESS_T`, `IDENTITY_T`, `SEND_T`, `FLAG_T`, `LISTEN_KIND_T`, `LISTEN_STATUS_T`, `ACTION_T`, `STATUS_T`, `AUTH_T`, `SESSION_ACTION_T` and `ADDRESS_STATUS_T`.
The components are `IDENTITY_REQUEST`, `IDENTITY_STATE`, `LISTEN_REQUEST`, `LISTEN_STATE`, `CONNECTION_REQUEST`, `INBOUND`, `CONNECTION_STATE`, `LANES_REQUEST`, `LANE_STATES`, `SESSION` and `SESSION_REQUEST`.
Then come `MESSAGES_OUT`, `MESSAGES_IN`, `SESSION_MESSAGES_OUT`, `SESSION_MESSAGES_IN`, `SIGNALS_OUT`, `SIGNALS_IN`, `ADDRESS_REQUEST` and `ADDRESS_RESULT`, then `SETTINGS`.
`SOCKETS_TYPE` declares a type of the contract, and `SOCKETS_OBSERVER` declares an observer on 1 component: `docs/intern/platform_abstraction/sockets_gns/sockets_declare.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_runs.c.md`.
`sockets_settings_t` holds the settings of the contract in the order of its table, then the 5 settings of the module: `docs/usage/platform_abstraction/sockets_gns.md`.

The header includes `gns_flat.h`, `sockets_values.h` and `ideatek_sockets.h`, and `os.h` of mosl for the thread, the mutex and the event.
