# src/platform_abstraction/sockets_gns/sockets_receive.c

This file takes the messages that came, writes them into the half, and attaches `ideatek:MessagesIn` and `ideatek:SessionMessagesIn`.

## The messages

`receive_connections` runs once a round. For each connection:

1. While the half has room for `recv_max_message` and a head, it takes up to `SOCKETS_BATCH` messages from GameNetworkingSockets. A message is never larger than `recv_max_message`, so each one fits.
2. `write_message` writes each message as a record of `ideatek:MessagesIn`. The record holds the entity of the slot, and the number, the lane and the flags of the message. Then the message goes back to GameNetworkingSockets.
3. With no room, the messages stay with GameNetworkingSockets until the round after. The contract says so: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The bytes of the batches in*.

After the connections, `attach_batch` attaches `ideatek:MessagesIn` to the sockets entity if a record came and `changes` has room. With no room in `changes`, the batch is lost, with the line of the room. This has no path in the tests: the manifest gives the room.

## The sessions

`receive_sessions` does the same for the sessions, for each channel from 0 to `session_channels` less 1. It takes the messages of the channel, and finds the slot of the peer of each one with `find_peer`. Then it writes the record of `ideatek:SessionMessagesIn`.
A message from a peer with no session slot is a message of a session that GameNetworkingSockets opened and the game did not accept yet. It goes back to GameNetworkingSockets, and the line of an early message tells it.

## The branches

| Case | Result |
| --- | --- |
| The half has room. | The messages come. |
| The half has no room for the largest message. | The messages wait in GameNetworkingSockets. |
| No message came. | No attach. |
| No room in `changes`. | The batch is lost, with its line. |
| A message of a peer with no session. | Its line, and the message goes back. |

## Functions

| Function | Result |
| --- | --- |
| `take_messages` | Takes up to `SOCKETS_BATCH` messages of a connection or a channel, writes them, and releases them. Gives the count written. |
| `receive_connections` | Takes the messages of each connection into the half. |
| `receive_sessions` | Takes the messages of each channel into the half. |
| `attach_batch` | Attaches a batch of the half to the sockets entity, when it holds a record. |
