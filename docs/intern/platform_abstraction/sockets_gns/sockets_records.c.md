# src/platform_abstraction/sockets_gns/sockets_records.c

This file reads and writes the records of the batches of the contract, whose fields have no alignment.

## The records

Each read takes the bytes of a batch and the bytes left, and fills a `sockets_record_t`. It gives the bytes of the record, or 0 when the record does not fit in the bytes left.
Each write takes the room left, writes a record, and gives its bytes, or 0 when the record does not fit in the room.
The fields go through `memcpy`, because a record can start at any byte: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The messages*.

| Function | Record |
| --- | --- |
| `read_message` | `ideatek:MessagesOut`: `connection`, `length`, `lane`, `flags`, the bytes. |
| `read_session_message` | `ideatek:SessionMessagesOut`: `session`, `channel`, `length`, `flags`, the bytes. |
| `read_signal` | `ideatek:SignalsIn`: `length`, the bytes. |
| `write_message` | `ideatek:MessagesIn`: `connection`, `number`, `length`, `lane`, `flags`, the bytes. |
| `write_session_message` | `ideatek:SessionMessagesIn`: `session`, `channel`, `length`, `flags`, the bytes. |

A read of a batch stops at the first record that does not fit. The rest of the batch is malformed, and the caller writes its line.

## The branches

| Case | Result |
| --- | --- |
| The head of a record does not fit in the bytes left. | 0. |
| The bytes of a record do not fit in the bytes left. | 0. |
| A record does not fit in the room of a write. | 0, and nothing is written. |

## Functions

| Function | Result |
| --- | --- |
| `read_message` | Reads a record of `ideatek:MessagesOut`. |
| `read_session_message` | Reads a record of `ideatek:SessionMessagesOut`. |
| `read_signal` | Reads a record of `ideatek:SignalsIn`. |
| `write_message` | Writes a record of `ideatek:MessagesIn`. |
| `write_session_message` | Writes a record of `ideatek:SessionMessagesIn`. |
