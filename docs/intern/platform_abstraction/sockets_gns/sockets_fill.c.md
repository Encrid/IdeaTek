# src/platform_abstraction/sockets_gns/sockets_fill.c

This file fills the fields of `ideatek:ConnectionState` from what GameNetworkingSockets tells of a connection: its info and its real time status.

## The fields

| Field | Source |
| --- | --- |
| `status` | `status_of` the state of the info: `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md`. |
| `flags` | `flags_of` the flags of the info: `docs/intern/platform_abstraction/sockets_gns/sockets_flags.c.md`. |
| `remote_identity`, `remote` | The identity and the address of the info, as a text and as bytes. |
| `end_reason`, `end_text` | The end reason and the end debug text of the info, after the end. Else 0 and an empty text. |
| `ping`, `quality_local`, `quality_remote`, `in_bytes`, `out_bytes`, `in_packets`, `out_packets`, `send_rate`, `max_jitter`, `queue_time`, `pending_reliable`, `pending_unreliable`, `sent_unacked_reliable` | The real time status, field by field. |

`fill_info` fills the first 4 rows. `fill_status` fills the numbers. `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md` calls both, and adds `failed_sends`.

## The branches

| Case | Result |
| --- | --- |
| A connection that ended. | `end_reason` and `end_text` of the info. |
| A live connection. | 0 and an empty text. |

## Functions

| Function | Result |
| --- | --- |
| `fill_info` | Fills the status, the flags, the peer and the end from the info. |
| `fill_status` | Fills the numbers from the real time status. |
