# src/platform_abstraction/sockets_gns/sockets_half.c

This file holds the half of the round, the 2 rings, and the room of `changes`.

## The round

`turn_half` destroys the entities of `gone`. Then it switches the side of the half, and empties the new side.
A destroy always fits in `changes`, because it costs less than the attach that put the entity in `gone`.
`put_bytes` copies bytes into the current side, and gives their text. `room_left` gives the bytes left in the current side.
`has_room` tells whether `changes` can take a change of the given bytes.

## The rings

A ring of `sockets_ring_t` holds records: a length of 4 bytes, then the bytes. A record does not wrap: a writer that reaches the end starts at the place of the ring when the head is far enough.
`ring_put` writes a record under no lock of its own: the caller holds `lock`. It gives `false` when the record does not fit.
`ring_take` gives the next record and moves the tail, or gives a length of 0 when the ring is empty.

## The branches

| Case | Result |
| --- | --- |
| A record does not fit at the end of the ring. | It goes to the place of the ring, if the tail left room there. |
| The ring is full. | `ring_put` gives `false`. |
| The ring is empty. | `ring_take` gives 0. |

## Functions

| Function | Result |
| --- | --- |
| `has_room` | Tells whether `changes` can take the bytes. |
| `turn_half` | Destroys the entities of `gone`, and switches the side of the half. |
| `put_bytes` | Copies bytes into the current side, and gives their text. |
| `ring_put` | Writes a record into a ring, or gives `false`. |
| `ring_take` | Takes the next record of a ring, or gives 0. |
