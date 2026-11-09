# src/platform_abstraction/sockets_gns/sockets_close.c

This file accepts and closes the connections, destroys the entities of the closed ones, and ends everything at an identity change.

## The accept and the close

`accept` accepts an inbound connection. GameNetworkingSockets refuses an accept of a connection that is not inbound, or not connecting. The line `the request of <id>: action 2 is outside an inbound connection` tells it.

`close` closes the connection of an entity with `reason`, the text `closed by the game`, and `linger`. Then it marks the slot `closed`, and puts the entity in `gone`: the next round destroys it.
A session closes with `IDEATEK_SESSION_ACTION_CLOSE` instead: `docs/intern/platform_abstraction/sockets_gns/sockets_sessions.c.md`. A close on a session writes the line of the action, and does nothing.
A close of an entity that has no connection writes `the request of <id>: action 3 is outside a connection`.

## The end of everything

`stop_all` runs before an identity change: `docs/intern/platform_abstraction/sockets_gns/sockets_identity.c.md`.
For each slot that is not closed, it attaches `ideatek:ConnectionState` with `IDEATEK_CONNECTION_STATUS_PROBLEM` and the text `identity changed`, and forgets the handle. For each listen socket, it attaches `ideatek:ListenState` with `IDEATEK_LISTEN_STATUS_FAILED`, and frees the slot.
The game then closes each connection with `IDEATEK_CONNECTION_ACTION_CLOSE`, and the close of a forgotten handle only marks the slot and puts the entity in `gone`.

## The round

`drop_closed` runs in each round after `turn_half`: it frees the slot of each closed connection whose entity went.

## The branches

| Case | Result |
| --- | --- |
| An accept of a connection that GameNetworkingSockets refuses. | Its line. |
| A close of a session. | The line of the action. |
| A close of an entity with no connection. | The line of the connection. |
| A close of a forgotten handle. | The slot is marked, and the entity goes. |
| An identity change with open connections. | Each one gets the problem `identity changed`. |

## Functions

| Function | Result |
| --- | --- |
| `accept` | Accepts an inbound connection, or writes its line. |
| `close` | Closes a connection with its reason and `linger`, and marks it for the next round. |
| `stop_all` | Tells the game that each connection and each listen socket ended. |
| `drop_closed` | Frees the slot of each closed connection whose entity went. |
