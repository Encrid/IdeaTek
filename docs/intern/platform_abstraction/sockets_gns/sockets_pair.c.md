# src/platform_abstraction/sockets_gns/sockets_pair.c

This file makes the 2 connections of `IDEATEK_CONNECTION_ACTION_PAIR` in the program.

## The pair

`make_pair` takes a second slot, then asks GameNetworkingSockets for a socket pair with the identity of the module at both ends. The first handle goes to the slot of the request. The second goes to the second slot.
`place_second` spawns the entity of the second slot, and gives its handle the index of the slot as user data. Then it attaches `ideatek:Inbound` whose `listen` is 0, and the first `ideatek:ConnectionState`.
The caller sets the user data of the first handle and attaches its state: `docs/intern/platform_abstraction/sockets_gns/sockets_connect.c.md`.

With no free second slot, `make_pair` says the bound of the connections, and gives `too many connections`. A pair that GameNetworkingSockets refuses frees the second slot and gives `refused`. The caller attaches the text as the problem of the request.

## The branches

| Case | Result |
| --- | --- |
| 1 free slot only. | The line of the bound, and `too many connections`. |
| GameNetworkingSockets refuses the pair. | `refused`. |
| A pair. | 2 connections, and `nullptr`. |

## Functions

| Function | Result |
| --- | --- |
| `place_second` | Gives the second end its entity, its user data, `ideatek:Inbound` and its state. |
| `make_pair` | Makes a pair on the slot of the request and a second slot. Gives `nullptr`, or the text of the problem that stopped it. |
