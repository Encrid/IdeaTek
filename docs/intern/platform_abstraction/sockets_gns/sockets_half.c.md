# src/platform_abstraction/sockets_gns/sockets_half.c

This file holds the half of the round and the entities that go.

## The half

`sockets_half_t` holds 2 sides of `message_bytes` divided by 2, in the arena. A round writes its batches in into 1 side, and the game reads them in the next round while the module writes the other side.
`turn_half` switches the side, and empties the new side. `put_bytes` copies bytes into the current side, and gives their text. `room_left` gives the bytes left in the current side.

## The entities that go

A closed connection, and a name with its result, keep their entity until the game read its last state. `drop_later` puts an entity in `later`.
`turn_half` destroys the entities of `gone`, then moves `later` into `gone`. So an entity marked in a round goes 2 rounds later. An observer marks an entity at a barrier, and the first phase of the next round cannot destroy it yet.
A destroy always fits in `changes`, because it costs less than the attach that marked the entity.

## The branches

| Case | Result |
| --- | --- |
| An entity in `later`. | It goes in the round after the next one. |
| An entity in `gone`. | It goes in this round. |

## Functions

| Function | Result |
| --- | --- |
| `turn_half` | Destroys the entities of `gone`, moves `later` into `gone`, and switches the side of the half. |
| `drop_later` | Marks an entity that goes in a later round. |
| `put_bytes` | Copies bytes into the current side, and gives their text. |
| `room_left` | Gives the bytes left in the current side. |
