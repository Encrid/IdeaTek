# src/platform_abstraction/sockets_gns/sockets_status.c

This file takes the callback of GameNetworkingSockets for the state of a connection: the connections that come, and the changes of state.

## The callback

GameNetworkingSockets calls `on_changed` from `RunCallbacks`, on the thread of the phases, with the handle, the info and the old state.
The user data of the connection is the index of its slot, or -1 for a connection that the module does not know.

1. An unknown connection in the state connecting, with a listen socket, is an inbound connection: `take_inbound`.
2. An unknown connection in another state belongs to a pair or a session, which GameNetworkingSockets reports early. The callback ignores it: the slot comes from its maker.
3. A known connection that reaches the problem state writes the line of the problem. The detailed status in it has up to `SOCKETS_TEXT` bytes.
4. A known connection in any new state gets `attach_state`: `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md`. So the game sees a change in the round of the callback.

## The inbound connections

`take_inbound` takes a slot for the connection, spawns its entity, and sets its user data. It finds the entity of the listen socket, and attaches `ideatek:Inbound` and `ideatek:ConnectionState`.
With no free slot, it closes the connection with the reason 5001 and the text `too many connections`, and writes the line of the bound.
A P2P connection that comes through the signals is an inbound connection too: its listen socket is the P2P one.

## The branches

| Case | Result |
| --- | --- |
| An unknown connecting connection with a listen socket. | A slot, an entity, `ideatek:Inbound` and a state. |
| An unknown connection in another state. | Nothing. |
| No free slot for an inbound connection. | The connection is closed with 5001, and its line. |
| A known connection reaches the problem state. | Its line with the detailed status, and a state. |
| A known connection changes its state. | A state. |

## Functions

| Function | Result |
| --- | --- |
| `listen_entity` | Gives the entity of a listen socket by its handle, or 0. |
| `take_inbound` | Gives a connection that came its slot and its entity, or closes it. |
| `say_problem` | Writes the line of a problem with the detailed status. |
| `on_changed` | Runs the steps of the callback. |
