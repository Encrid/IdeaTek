# src/platform_abstraction/sockets_gns/sockets_sessions.c

This file runs the observer of `ideatek:SessionRequest`, and takes the slot of a session.

## The slot

`take_session` takes a slot with `session` set and the identity of the peer, attaches `ideatek:Session` with the text of the peer, then the first `ideatek:ConnectionState`. The observer calls it for an open, and the callback of a peer calls it for a session that came: `docs/intern/platform_abstraction/sockets_gns/sockets_session_peer.c.md`.
With no free slot, it writes the line of the bound, and gives `nullptr`.

## The request

The observer `session` gets each write of `ideatek:SessionRequest`:

1. `IDEATEK_SESSION_ACTION_OPEN` on an entity with no slot reads the identity of `remote`. An identity that GameNetworkingSockets refuses, or an IP identity, writes `the request of <id>: remote <text> is outside an identity`, and nothing changes. Else `take_session` takes the slot. GameNetworkingSockets opens the connection at the first message.
2. `IDEATEK_SESSION_ACTION_ACCEPT` accepts the session of the peer of the slot. A refusal writes `the request of <id>: action 1 is outside a session of a peer`.
3. `IDEATEK_SESSION_ACTION_CLOSE` closes the session of the peer. `IDEATEK_SESSION_ACTION_CLOSE_CHANNEL` closes `channel` of the session, and drops its messages. GameNetworkingSockets keeps the session after the close of its last channel.
4. After a close, `close_session` asks GameNetworkingSockets for the state of the session. A session in the state none is over: the slot becomes `closed`, and the entity goes in a later round. A session that failed closes the same way: `docs/intern/platform_abstraction/sockets_gns/sockets_session_peer.c.md`.
5. A request on a connection, on a closed session, or an open on an entity with a slot, is wrong. It writes the line of the action, and nothing changes. An action outside its enum writes the same line.
6. With no free slot, an open attaches `ideatek:ConnectionState` with the problem `too many connections`.

## The branches

| Case | Result |
| --- | --- |
| An open with a wrong identity. | Its line, and nothing. |
| An action on the wrong kind of entity, or outside its enum. | Its line, and nothing. |
| No free slot for an open. | The line of the bound, and the problem `too many connections` in the state. |
| A refused accept. | Its line. |
| A close that ends the session. | The slot is closed, and the entity goes. |
| A close of a channel. | The session stays. |

## Functions

| Function | Result |
| --- | --- |
| `take_session` | Takes a slot for a session with a peer, and attaches `ideatek:Session` and the first state. Gives `nullptr` with its line when no slot is free. |
| `open_session` | Runs an open: the identity, the slot and the 2 attaches. |
| `close_session` | Closes a session or 1 channel, and marks the slot when the session is over. |
| `session_action` | Checks the slot and the action of a request, then accepts or closes. |
| `session` | Runs the action of each trigger of the observer. |
