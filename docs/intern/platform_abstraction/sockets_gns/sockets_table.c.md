# src/platform_abstraction/sockets_gns/sockets_table.c

This file finds, takes and frees the slots of the table of the connections.

## The table

The table holds `max_connections` slots of `sockets_connection_t`. A slot is free when its `entity` is 0.
`find_entity` finds the slot of an entity. `find_peer` finds the slot of the session of a peer, by its identity.
Each find walks the table with no branch, as `find_device` of the audio module, and gives `max_connections` when nothing matches.

GameNetworkingSockets keeps the index of the slot as the user data of each connection, so a callback finds its slot in 1 step: `docs/intern/platform_abstraction/sockets_gns/sockets_status.c.md`.

`take_slot` takes the first free slot for an entity, with 1 lane, and gives it. With no free slot, it gives `nullptr`, and the caller writes its line.
`free_slot` makes a slot free. It keeps no handle, because GameNetworkingSockets freed the connection before.
`drop_closed` runs in each round after `turn_half`: it frees the slot of each closed connection, whose entity went in the round before: `docs/intern/platform_abstraction/sockets_gns/sockets_half.c.md`.

## The branches

| Case | Result |
| --- | --- |
| No slot matches. | The index `max_connections`. |
| No free slot. | `nullptr`. |

## Functions

| Function | Result |
| --- | --- |
| `find_entity` | Gives the index of the slot of an entity, or `max_connections`. |
| `find_peer` | Gives the index of the slot of the session of a peer, or `max_connections`. |
| `take_slot` | Takes a free slot for an entity, or gives `nullptr`. |
| `free_slot` | Makes a slot free. |
| `drop_closed` | Frees the slot of each closed connection. |
