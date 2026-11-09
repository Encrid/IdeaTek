# src/platform_abstraction/sockets_gns/sockets_state.c

This file fills `ideatek:ConnectionState` from GameNetworkingSockets, and attaches it when a byte changed.

## The state

`fill_state` asks GameNetworkingSockets for the info and the real time status of a connection, and fills `ideatek_connection_state_t` with `fill_info` and `fill_status`: `docs/intern/platform_abstraction/sockets_gns/sockets_fill.c.md`.
For a session, the info and the status come from `GetSessionConnectionInfo` with the identity of the peer. A session whose connection is not there yet keeps its last state.
For a connection, `live_state` asks both calls, and gives `false` when one refuses. A refused call writes `no state of the connection of <id>`. GameNetworkingSockets knows each handle of the module, so the line comes only with a failed call of the tests.

A connection whose handle GameNetworkingSockets forgot, after an identity change, keeps its last state. Only `failed_sends` can change.

`attach_state` compares the state with `last` of the slot, byte by byte. A state that changed goes to the entity, and into `last`.
`attach_states` runs `attach_state` for each live slot, in each round, after the callbacks and the sends.

## The branches

| Case | Result |
| --- | --- |
| GameNetworkingSockets refuses a call of the state. | The line of the state, and the last state with the count of the failed sends. |
| A session with no connection yet. | The last state. |
| A state with no changed byte. | No attach. |
| A session. | The info of the session, by the identity of the peer. |

## Functions

| Function | Result |
| --- | --- |
| `live_state` | Asks the info and the status of a connection. Gives `false` with its line when a call refuses. |
| `fill_state` | Fills a state from GameNetworkingSockets, or keeps the last one. |
| `attach_state` | Attaches the state of a slot when a byte changed. |
| `attach_states` | Attaches the state of each live slot. |
