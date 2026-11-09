# src/platform_abstraction/sockets_gns/sockets_send.c

This file runs the observers of `ideatek:MessagesOut` and `ideatek:SessionMessagesOut`: it sends each record once.

## The messages

The observer `messages_out` gets each write of `ideatek:MessagesOut`. For each record of the batch, in order:

1. `read_message` reads the record: `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md`. A record that does not fit ends the batch, with the line of a malformed batch.
2. `find_entity` finds the slot of `connection`. A record for no connection, or for a session, is a failed send of no slot. Its line is `the message of <id> is refused: no connection`.
3. A `length` above `GNS_MAX_MESSAGE` is a failed send with its line. A `lane` not below the lanes of the connection is one too.
4. GameNetworkingSockets sends the message with the flags of the record, on its lane. A refusal adds 1 to `failed_sends` of the slot, with the line `the message of <id> is refused: <reason>`.

The flags of the contract have the values of GameNetworkingSockets, so they go as they are. `IDEATEK_SEND_AUTO_RESTART` has a meaning for a session only, and GameNetworkingSockets ignores it on a connection.

## The sessions

The observer `session_messages_out` does the same for `ideatek:SessionMessagesOut`, with `read_session_message`, `find_entity` on `session`, and the send to the identity of the peer on `channel`.
A `channel` below 0 or not below `session_channels` is a failed send with its line: the module reads no message of such a channel.
A record for an entity that is a connection, not a session, is a failed send with its line.

## The branches

| Case | Result |
| --- | --- |
| A malformed record. | The line of the batch, and the rest of the batch is left. |
| A record for no connection, or the wrong kind. | The line of the message. |
| A length or a lane outside its range. | The line of the message, and 1 failed send. |
| A channel outside its range. | The line of the message, and 1 failed send. |
| GameNetworkingSockets refuses a message. | The line of the message, and 1 failed send. |

## Functions

| Function | Result |
| --- | --- |
| `send_one` | Sends 1 record to its connection, and counts a refusal. |
| `send_session_one` | Sends 1 record to its session, and counts a refusal. |
| `send_batch` | Reads a batch record by record, and sends each one with the given function. |
| `messages_out` | Reads the triggers of the observer, and sends each batch. |
| `session_messages_out` | Reads the triggers of the observer, and sends each batch. |
