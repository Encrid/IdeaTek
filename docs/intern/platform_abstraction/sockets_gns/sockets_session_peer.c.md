# src/platform_abstraction/sockets_gns/sockets_session_peer.c

This file takes the 2 callbacks of GameNetworkingSockets for the sessions: a session that a peer opens, and a session that fails.

## The callbacks

GameNetworkingSockets calls both from `RunCallbacks` of the round: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`.

`on_session_request` runs when a peer opens a session. A peer that has a slot is a peer the game opened a session to, and sent nothing to yet. The module accepts the connection of the peer at once, because the game wants that peer. Else `take_session` takes a slot, spawns an entity, and attaches `ideatek:Session` with the identity of the peer, then `ideatek:ConnectionState`: `docs/intern/platform_abstraction/sockets_gns/sockets_sessions.c.md`. The game answers with an accept or a close.
With no free slot, it destroys the entity, closes the session of the peer, and `take_session` writes the line of the bound.

`on_session_failed` runs when the connection of a session breaks or times out. In `last`, the slot of the peer gets the status of the state, the end reason and the text of the peer. Then the state goes to the entity. The game closes the session with `IDEATEK_SESSION_ACTION_CLOSE`, as after any problem.

A failure of a peer with no slot, or of a closed slot, does nothing. The failure comes from the connection of a live session, and a close of the game deletes the session at once. No case reaches the guard, and the exception is in `.toolchain/project.toml`.

## The branches

| Case | Result |
| --- | --- |
| A peer opens a session, with a free slot. | A slot, an entity, `ideatek:Session` and a state. |
| A peer opens a session that the game opened to it. | The module accepts the connection. |
| No free slot. | The line of the bound, and the session closes. |
| A session fails. | A state with the problem or the close. |
| A failure of a peer with no slot, or a closed slot. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `on_session_request` | Gives a session that a peer opened its slot and its entity, accepts a known peer, or closes the session. |
| `on_session_failed` | Attaches the state of a session that failed. |
