# src/platform_abstraction/sockets_gns/sockets_signals.c

This file gives GameNetworkingSockets the signaling of the module, carries its signals out, and gives it the signals that came in.

## The signaling

GameNetworkingSockets asks the module for a signaling object in 3 ways. `make_signaling` answers each one with `CreateCustomSignaling`, with `send_signal` as the send callback and `release_signaling` as the release callback:

- A P2P connect of the game: `docs/intern/platform_abstraction/sockets_gns/sockets_connect.c.md`.
- A session that the game opens, through the callback `Callback_CreateConnectionSignaling`: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
- A connection that came through a signal, in `on_connect_request` of `ReceivedP2PCustomSignal2`.

`send_signal` runs on the thread of the network, whenever GameNetworkingSockets wants. Under `lock`, it writes a record of `ideatek:SignalsOut` into the ring `signals`. The record holds the entity of the connection, from its user data, the text of the peer, and the bytes of the signal.
A signal that does not fit in the ring gives `false`, and GameNetworkingSockets sends it again later. `release_signaling` does nothing: the module keeps no object for a signaling.

## The signals in

The observer `signals_in` gets each write of `ideatek:SignalsIn`. For each record, `read_signal` reads the bytes, and `ReceivedP2PCustomSignal2` gives them to GameNetworkingSockets, with `on_connect_request` and `send_rejection`.
`on_connect_request` gives a signaling for a connection that a peer opens. `send_rejection` writes a rejection record into the ring, whose `connection` is 0, for a signal that no listen socket takes.
A signal that GameNetworkingSockets refuses writes `a signal of <count> bytes is refused`. A malformed batch writes its line.

## The round

`attach_signals` runs once a round. It moves the records of the ring into the half, under `lock`. Then it attaches `ideatek:SignalsOut` to the sockets entity if a record moved.
A record that does not fit in the half stays in the ring for the next round.

## The branches

| Case | Result |
| --- | --- |
| A signal to send, with room in the ring. | A record in the ring, and `true`. |
| A signal to send, with no room. | `false`, and GameNetworkingSockets tries again. |
| A signal in for a listen socket. | A connection comes, as an inbound one. |
| A signal in for no listen socket. | A rejection in the ring. |
| A signal that GameNetworkingSockets refuses. | Its line. |
| No signal in the ring. | No attach. |

## Functions

| Function | Result |
| --- | --- |
| `send_signal` | Writes a signal into the ring under the lock. Gives `false` when it does not fit. |
| `make_signaling` | Gives a signaling object of GameNetworkingSockets with the callbacks of the module. |
| `send_rejection` | Writes a rejection into the ring under the lock. |
| `signals_in` | Reads the triggers of the observer, and gives each signal to GameNetworkingSockets. |
| `attach_signals` | Moves the ring into the half, and attaches `ideatek:SignalsOut`. |
