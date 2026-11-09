# src/platform_abstraction/sockets_gns/sockets_send.c

This file runs the observer of `ideatek:MessagesOut`, and reads the batches out of both kinds.

## The batches

`send_batch` reads a batch of `ideatek:MessagesOut` or of `ideatek:SessionMessagesOut`, record by record: `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md`. It sends each record with `send_one` or `send_session_one`: `docs/intern/platform_abstraction/sockets_gns/sockets_send_session.c.md`.
A record that does not fit ends the batch, with the line of a malformed batch: `docs/intern/platform_abstraction/sockets_gns/sockets_batch.c.md`.
After the batch, `SteamNetworkingSockets_Poll` with a wait of 0 lets GameNetworkingSockets send the packets before the barrier ends: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*.

## The messages

The observer `messages_out` gets each write of `ideatek:MessagesOut`. For each record, `send_one`:

1. `find_entity` finds the slot of `connection`. A record for no connection, or for a session, is a failed send of no slot. A closed or forgotten connection counts as none. Its line ends with `no connection`.
2. A `lane` not below the lanes of the connection is a failed send, whose line says `outside the limits`.
3. `send_on_lane` takes a message of GameNetworkingSockets with `AllocateMessage`, and copies the bytes into it. It sets the connection, the flags and the lane, and sends it with `SendMessages`. GameNetworkingSockets frees the message. A refusal adds 1 to `failed_sends` of the slot, with the line of a refused message.
4. `FlushMessagesOnConnection` flushes the connection, so the message does not wait for the Nagle timer. In the manual poll mode, the timer fires at the poll of the next round.

`send_of` turns the flags of the contract into the ones of GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_flags.c.md`. `IDEATEK_SEND_AUTO_RESTART` has a meaning for a session only, and GameNetworkingSockets ignores it on a connection.
`say_refused` writes the line of a refused message, and counts the failed send when the record has a slot.

## The branches

| Case | Result |
| --- | --- |
| A malformed record. | The line of the batch, and the rest of the batch is left. |
| A record for no connection, or the wrong kind. | The line of the message. |
| A lane outside its range. | The line of the message, and 1 failed send. |
| No message of GameNetworkingSockets. | The line of the message, and 1 failed send. |
| GameNetworkingSockets refuses a message. | The line of the message, and 1 failed send. |

## Functions

| Function | Result |
| --- | --- |
| `say_refused` | Writes the line of a refused message, and counts it on its slot. |
| `send_on_lane` | Sends 1 record on its lane with a message of GameNetworkingSockets. Gives the result. |
| `send_one` | Finds the connection of 1 record, sends it, and flushes the connection. |
| `send_batch` | Reads a batch record by record, sends each one by its kind, and polls once. |
| `messages_out` | Reads the triggers of the observer, and sends each batch. |
