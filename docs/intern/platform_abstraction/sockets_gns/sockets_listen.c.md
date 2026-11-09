# src/platform_abstraction/sockets_gns/sockets_listen.c

This file runs the observer of `ideatek:ListenRequest`: it opens and closes the listen sockets.

## The request

The observer `listen` gets each write and each detach of `ideatek:ListenRequest`:

1. A detach closes the listen socket of the entity, and frees its slot. A detach of an entity with no listen socket does nothing.
2. A write on an entity that has a listen socket closes it first: a new request replaces the listen socket.
3. With no free slot, the write writes the line of the bound, and attaches `ideatek:ListenState` with `IDEATEK_LISTEN_STATUS_FAILED`.
4. With `IDEATEK_LISTEN_KIND_IP`, `open_ip` opens a listen socket on every address of the machine, on `port`. With `IDEATEK_LISTEN_KIND_P2P`, `open_p2p` opens one on `virtual_port`.
5. A kind outside its enum writes `the request of <id>: kind <value> is outside 0 to 1`, and the status is `IDEATEK_LISTEN_STATUS_FAILED`.
6. `attach_listen` attaches `ideatek:ListenState` with the fields of the request and the status: `IDEATEK_LISTEN_STATUS_LISTENING` for a handle, else `IDEATEK_LISTEN_STATUS_FAILED`.

A listen socket on `port` 0 takes a free port of the system. The state gives the port of the request, so a game that wants the chosen port asks GameNetworkingSockets through a connection.

## The branches

| Case | Result |
| --- | --- |
| A detach. | The listen socket closes, and its slot is free. |
| A detach of an entity with no listen socket. | Nothing. |
| A write on an entity with a listen socket. | The old listen socket closes first. |
| No free slot. | The line of the slots, and `IDEATEK_LISTEN_STATUS_FAILED`. |
| A kind outside its enum. | The line of the kind, and `IDEATEK_LISTEN_STATUS_FAILED`. |
| GameNetworkingSockets refuses the listen socket. | `IDEATEK_LISTEN_STATUS_FAILED`. |

## Functions

| Function | Result |
| --- | --- |
| `find_listen` | Gives the slot of the listen socket of an entity, or `max_listen_sockets`. |
| `close_listen` | Closes the listen socket of a slot, and frees the slot. |
| `open_listen` | Opens the listen socket of a request, by its kind, and gives its handle or 0. |
| `attach_listen` | Attaches `ideatek:ListenState` to the entity of a request. |
| `listen` | Opens or closes the listen socket of each trigger of the observer. |
