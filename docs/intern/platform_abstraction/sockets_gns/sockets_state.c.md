# src/platform_abstraction/sockets_gns/sockets_state.c

This file fills `ideatek:ConnectionState` from GameNetworkingSockets, and attaches it when a byte changed.

## The state

`fill_state` asks GameNetworkingSockets for the info and the real time status of a connection, and fills `ideatek_connection_state_t`:

| Field | Source |
| --- | --- |
| `status` | `status_of` the state of the info: `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md`. |
| `flags` | `flags_of` the flags of the info. |
| `remote_identity`, `remote` | The identity and the address of the info, as texts and bytes. |
| `end_reason`, `end_text` | The end reason and the end debug text of the info, after the end. Else 0 and an empty text. |
| `ping`, `quality_local`, `quality_remote`, `in_bytes`, `out_bytes`, `in_packets`, `out_packets`, `send_rate`, `max_jitter`, `queue_time`, `pending_reliable`, `pending_unreliable`, `sent_unacked_reliable` | The real time status, field by field. |
| `failed_sends` | The count of the slot, which the round empties after the attach. |

For a session, the info and the status come from `GetSessionConnectionInfo` with the identity of the peer, and the handle is not used.
A connection whose handle GameNetworkingSockets forgot, after a close or an identity change, keeps its last state. Only `failed_sends` can change.

`attach_state` compares the state with `last` of the slot, byte by byte. A state that changed goes to the entity, and into `last`.
`attach_states` runs `attach_state` for each live slot, in each round, after the callbacks and the sends.

## The branches

| Case | Result |
| --- | --- |
| GameNetworkingSockets does not know the handle. | The last state, with the count of the failed sends. |
| A state with no changed byte. | No attach. |
| A session. | The info of the session, by the identity of the peer. |

## Functions

| Function | Result |
| --- | --- |
| `fill_info` | Fills the status, the flags, the peer and the end from the info. |
| `fill_status` | Fills the numbers from the real time status. |
| `fill_state` | Fills a state from GameNetworkingSockets, or keeps the last one. |
| `attach_state` | Attaches the state of a slot when a byte changed. |
| `attach_states` | Attaches the state of each live slot. |
