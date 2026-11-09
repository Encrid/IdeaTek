# src/platform_abstraction/sockets_gns/sockets_send_session.c

This file runs the observer of `ideatek:SessionMessagesOut`: it sends each record to the peer of its session.

## The messages

The observer `session_messages_out` gets each write of `ideatek:SessionMessagesOut`. `send_batch` reads the batch: `docs/intern/platform_abstraction/sockets_gns/sockets_send.c.md`. For each record, `send_session_one`:

1. `find_entity` finds the slot of `session`. A record for no session, for a closed one, or for a connection, is a failed send of no slot. Its line is `the message of <id> is refused: no session`.
2. A `channel` below 0 or not below `session_channels` is a failed send, with the limits in its line. The module reads no message of such a channel.
3. GameNetworkingSockets sends the message to the identity of the peer, on `channel`, with the flags of the record. A refused message writes its line with the reason of GameNetworkingSockets, and `failed_sends` of the slot grows by 1.

GameNetworkingSockets opens the connection of a session at its first message, and `IDEATEK_SEND_AUTO_RESTART` opens it again after a break.

## The branches

| Case | Result |
| --- | --- |
| A record for no session, or the wrong kind. | The line of the message. |
| A channel outside its range. | Its line, and `failed_sends` grows by 1. |
| GameNetworkingSockets refuses a message. | Its line, and `failed_sends` grows by 1. |

## Functions

| Function | Result |
| --- | --- |
| `send_session_one` | Sends 1 record to its session, and counts a refusal. |
| `session_messages_out` | Runs `send_batch` for each trigger of the observer. |
