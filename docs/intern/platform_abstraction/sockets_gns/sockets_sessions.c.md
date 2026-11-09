# src/platform_abstraction/sockets_gns/sockets_sessions.c

This file runs the observer of `ideatek:SessionRequest`, and the 2 callbacks of GameNetworkingSockets for the sessions.

## The request

The observer `session` gets each write of `ideatek:SessionRequest`:

1. `IDEATEK_SESSION_ACTION_OPEN` on an entity with no slot takes a slot with `session` set, and the identity of `remote`. An identity that GameNetworkingSockets refuses writes `the request of <id>: remote <text> is outside an identity`, and frees the slot. Then it attaches `ideatek:Session` and the first `ideatek:ConnectionState`. GameNetworkingSockets opens the connection at the first message.
2. `IDEATEK_SESSION_ACTION_ACCEPT` accepts the session of the peer of the slot. A refusal writes `the request of <id>: action 1 is outside a session of a peer`.
3. `IDEATEK_SESSION_ACTION_CLOSE` closes the session of the peer. The slot becomes `closed`, and the entity goes in `gone`.
4. `IDEATEK_SESSION_ACTION_CLOSE_CHANNEL` closes `channel` of the session. GameNetworkingSockets closes the session with its last channel, and the next callback tells it.
5. A request on a connection, or an open on an entity with a slot, is wrong. It writes the line of the action, and nothing changes.
6. With no free slot, an open writes the line of the slots, and attaches `ideatek:ConnectionState` with the problem `too many connections`.

## The callbacks

`on_session_request` runs when a peer opens a session. `take_session` takes a slot, spawns an entity, and attaches `ideatek:Session` with the identity of the peer, then `ideatek:ConnectionState`. The game answers with an accept or a close.
With no free slot, it closes the session of the peer, and writes the line of the slots.
`on_session_failed` runs when a session breaks or times out: the slot gets `attach_state`, which gives `IDEATEK_CONNECTION_STATUS_PROBLEM` or `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` from the info. A session that GameNetworkingSockets closed after 3 minutes without a message is destroyed in the next round, so its entity goes in `gone`.

## The branches

| Case | Result |
| --- | --- |
| An open with a wrong identity. | Its line, and nothing. |
| An action on the wrong kind of entity. | Its line, and nothing. |
| No free slot for an open. | The line of the bound, and the problem `too many connections` in the state. |
| A peer opens a session. | A slot, an entity, `ideatek:Session` and a state. |
| A session fails. | A state with the problem or the close. |
| A session ends by itself. | Its entity goes in the next round. |

## Functions

| Function | Result |
| --- | --- |
| `take_session` | Takes a slot for a session with a peer, on a given or a new entity. Gives `nullptr` with its line when no slot is free. |
| `open_session` | Runs an open: the identity, the slot and the 2 attaches. |
| `session` | Runs the action of each trigger of the observer. |
| `on_session_request` | Gives a session that a peer opened its slot and its entity. |
| `on_session_failed` | Attaches the state of a session that failed, and marks it when it ended. |
