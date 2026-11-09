# src/platform_abstraction/sockets_gns/sockets_connect.c

This file runs the observer of `ideatek:ConnectionRequest`, and opens the connections that the game asks for.

## The request

The observer `connection` gets each write of `ideatek:ConnectionRequest`:

1. `judge` checks the request. `action` is in its enum. Each send rate is from 1024 to 268435456, and `send_rate_min` is not above `send_rate_max`. With a close, `reason` is from 1000 to 2999, and no `linger` comes with `symmetric`. A wrong field writes the line of the field, and the request does nothing.
2. `IDEATEK_CONNECTION_ACTION_ACCEPT` and `IDEATEK_CONNECTION_ACTION_CLOSE` go to `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.
3. The 3 other actions open a connection on an entity that has none. An entity that has a connection writes `the request of <id>: action <value> is outside a new entity`, and the request does nothing.
4. With no free slot, the request writes `more than <max> connections: the request of <id> fails`, and attaches `ideatek:ConnectionState` with `IDEATEK_CONNECTION_STATUS_PROBLEM` and the text `too many connections`.
5. `IDEATEK_CONNECTION_ACTION_CONNECT` connects to `address`. `IDEATEK_CONNECTION_ACTION_CONNECT_P2P` connects to `remote_identity` on `virtual_port`, with a signaling of the module: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md`.
6. `IDEATEK_CONNECTION_ACTION_PAIR` makes 2 connections in the program. The first goes to the entity of the request. The second gets a new entity with `ideatek:Inbound` whose `listen` is 0, and its own slot.
7. Each new connection gets the index of its slot as its user data. Its options are the 2 send rates and `symmetric`. Then `attach_state` attaches its first `ideatek:ConnectionState`: `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md`.

A connection that GameNetworkingSockets refuses at once frees its slot, and attaches `ideatek:ConnectionState` with `IDEATEK_CONNECTION_STATUS_PROBLEM` and the text `refused`.

## The branches

| Case | Result |
| --- | --- |
| A field outside its range. | Its line, and nothing. |
| An open action on an entity with a connection. | Its line, and nothing. |
| No free slot for a connect. | The line of the bound, and a state with the problem. |
| GameNetworkingSockets refuses the connection. | A state with the problem `refused`. |
| A pair with 1 free slot. | The line of the slots, and no connection. |

## Functions

| Function | Result |
| --- | --- |
| `judge` | Checks the fields of a request. Gives `false` with its line for a wrong field. |
| `options_of` | Fills the options of a new connection: the send rates, `symmetric` and the user data. |
| `open_connection` | Opens a connection over IP or P2P, and gives its handle or 0. |
| `make_pair` | Makes a pair, takes the second slot and spawns its entity. Gives `false` when a slot is missing. |
| `connection` | Judges each trigger of the observer, and runs its action. |
