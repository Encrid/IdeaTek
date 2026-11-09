# src/platform_abstraction/sockets_gns/sockets_receive.c

This file takes the messages that came, writes them into the half, and attaches `ideatek:MessagesIn` and `ideatek:SessionMessagesIn`.

## The messages

`receive_connections` runs once a round. For each live connection:

1. `take_messages` asks GameNetworkingSockets for as many messages as the half holds, up to `SOCKETS_BATCH`. The room of 1 message is `recv_max_message` and a head, so each message that comes fits. With a full batch, it asks again.
2. `place_one` writes each message as a record of `ideatek:MessagesIn`, with `write_message`: `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md`. The record holds the entity of the slot, and the number, the lane and the flags of the message. Then the message goes back to GameNetworkingSockets.
3. With no room, the messages stay with GameNetworkingSockets until the round after. The contract says so: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The bytes of the batches in*.

After the connections, `attach_batch` attaches `ideatek:MessagesIn` to the sockets entity if a record came: `docs/intern/platform_abstraction/sockets_gns/sockets_batch.c.md`.

## The sessions

`receive_sessions` does the same for the sessions, for each channel from 0 to `session_channels` less 1. It takes the messages of the channel, and `place_one` finds the slot of the peer of each one with `find_peer`. Then it writes the record of `ideatek:SessionMessagesIn`.
A message from a peer with no session slot goes back to GameNetworkingSockets, and the line of an early message tells it. GameNetworkingSockets gives the messages of a session after the accept, and the slot exists from the request to the close of the game. No case reaches this guard, and `.toolchain/project.toml` excepts it.

## The branches

| Case | Result |
| --- | --- |
| The half has room for 1 message or more. | As many messages come as fit. |
| The half has no room for the largest message. | The messages wait in GameNetworkingSockets. |
| A batch of `SOCKETS_BATCH` messages. | `take_messages` runs again. |
| No message came. | No attach. |
| A message of a peer with no session. | Its line, and the message goes back. |

## Functions

| Function | Result |
| --- | --- |
| `write_one` | Writes 1 message as a record of the half. |
| `place_one` | Finds the slot of a message, writes it, and releases it. |
| `take_messages` | Takes the messages of a connection or a channel that fit in the half, and places them. Tells when the batch was full. |
| `receive_connections` | Takes the messages of each connection into the half, and attaches `ideatek:MessagesIn`. |
| `receive_sessions` | Takes the messages of each channel into the half, and attaches `ideatek:SessionMessagesIn`. |
