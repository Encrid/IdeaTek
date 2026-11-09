# src/platform_abstraction/sockets_gns/sockets_connect.c

This file runs the observer of `ideatek:ConnectionRequest`, and opens the connections that the game asks for.

## The request

The observer `connection` gets each write of `ideatek:ConnectionRequest`:

1. `judge` checks the request: `docs/intern/platform_abstraction/sockets_gns/sockets_judge.c.md`. A wrong field writes the line of the field, and the request does nothing.
2. `IDEATEK_CONNECTION_ACTION_ACCEPT` and `IDEATEK_CONNECTION_ACTION_CLOSE` go to `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.
3. The 3 other actions open a connection on an entity that has none. An entity that has a connection writes `the request of <id>: action <value> is outside a new entity`, and the request does nothing.
4. With no free slot, the request writes `more than <max> connections: the request of <id> fails`, and attaches `ideatek:ConnectionState` with `IDEATEK_CONNECTION_STATUS_PROBLEM` and the text `too many connections`.
5. `IDEATEK_CONNECTION_ACTION_CONNECT` connects to `address`. `IDEATEK_CONNECTION_ACTION_CONNECT_P2P` connects to `remote_identity` on `virtual_port`, with a signaling of the module: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md`. Both go through `open_one`.
6. `IDEATEK_CONNECTION_ACTION_PAIR` makes 2 connections in the program: `docs/intern/platform_abstraction/sockets_gns/sockets_pair.c.md`.
7. `finish` gives the new connection its slot index as user data. Then `attach_state` attaches its first `ideatek:ConnectionState`: `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md`.

`options_of` fills the options of a new connection: the 2 send rates, `symmetric`, and the user data. So the first callback of GameNetworkingSockets finds the slot already.
A connection that GameNetworkingSockets refuses at once, or a `remote_identity` that it does not parse, frees its slot. The state of the entity gets `IDEATEK_CONNECTION_STATUS_PROBLEM` and the text `refused`.

## The branches

| Case | Result |
| --- | --- |
| A field outside its range. | Its line, and nothing. |
| An open action on an entity with a connection. | Its line, and nothing. |
| No free slot for an open. | The line of the bound, and a state with the problem. |
| GameNetworkingSockets refuses the connection or the identity. | A state with the problem `refused`. |
| A pair with 1 free slot. | The line of the bound, and a state with the problem. |

## Functions

| Function | Result |
| --- | --- |
| `options_of` | Fills the options of a new connection: the send rates, `symmetric` and the user data. |
| `open_one` | Opens a connection over IP or P2P. Gives the text of a problem, or `nullptr`. |
| `finish` | Frees the slot and attaches the problem of a refused open, or sets the user data and attaches the first state. |
| `open_connection` | Takes a slot for an open action, and runs it. |
| `connection` | Judges each trigger of the observer, and runs its action. |
