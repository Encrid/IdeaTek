# src/platform_abstraction/sockets_gns/sockets_ring.c

This file holds the rings of records: the signals that GameNetworkingSockets asks to send, and its lines.

## The rings

A ring of `sockets_ring_t` holds records in a span of bytes: a length of 4 bytes, then the bytes. The ring knows its place in the span, its size, its head and its tail. The caller gives the span, so the file knows nothing of Bridge, and the program `sockets_ring` of the tests runs it alone on a buffer.
A record does not wrap. A writer that reaches the end marks the rest with the length `0xffffffff`. Then it starts at the place of the ring, when the tail left room there.

`ring_fits` tells whether a record of a size fits, and where it starts. `ring_put` writes a record, and gives `false` when the record does not fit.
`ring_take` gives the next record and moves the tail, or gives a length of 0 when the ring is empty. It skips the mark at the end, and finds a record there: a writer wraps only to write one.

GameNetworkingSockets runs in the manual poll mode: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*. Its callbacks run on the thread of the phases, so the rings need no lock.
A ring stays between the callbacks and the round, because a callback cannot attach a batch. The round moves the records into the half, or into the log.

## The branches

| Case | Result |
| --- | --- |
| A record does not fit at the end of the ring. | It goes to the place of the ring, if the tail left room there. |
| A record larger than the ring. | `ring_put` gives `false`. |
| The ring is full, with the head before the tail or after it. | `ring_put` gives `false`. |
| The ring is empty. | `ring_take` gives 0. |
| The tail reaches the mark, or the end of the ring. | `ring_take` goes to the place of the ring. |

## Functions

| Function | Result |
| --- | --- |
| `ring_fits` | Tells whether a record fits, and gives its start. |
| `ring_put` | Writes a record into a ring, or gives `false`. |
| `ring_take` | Takes the next record of a ring, or gives 0. |
