# src/platform_abstraction/sockets_gns/sockets_signals.c

This file gives GameNetworkingSockets the signaling of the module, and carries its signals out into the ring.

## The signaling

GameNetworkingSockets asks the module for a signaling object in 3 ways. `make_signaling` answers each one with `CreateCustomSignaling`, with `send_signal` as the send callback and `release_signaling` as the release callback:

- A P2P connect of the game: `docs/intern/platform_abstraction/sockets_gns/sockets_connect.c.md`.
- A session that the game opens, through `create_signaling`, the callback `Callback_CreateConnectionSignaling`: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
- A connection that came through a signal, in `on_connect_request` of `ReceivedP2PCustomSignal2`: `docs/intern/platform_abstraction/sockets_gns/sockets_signals_in.c.md`.

`send_signal` runs inside a call of the module, when GameNetworkingSockets wants to send a signal: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*. A callback cannot attach a batch, so `put_signal` writes a record of `ideatek:SignalsOut` into the ring `signals`. The record holds the entity of the connection, from its user data, and the text of the peer. Then come the bytes of the signal, up to `SOCKETS_SIGNAL`.
A signal that does not fit in the ring gives `false`, and GameNetworkingSockets sends it again later. The release callback, `release_signaling`, does nothing: the module keeps no object for a signaling.

The round moves the ring into the half: `docs/intern/platform_abstraction/sockets_gns/sockets_signals_in.c.md`, *The round*.

## The branches

| Case | Result |
| --- | --- |
| A signal to send, with room in the ring. | A record in the ring, and `true`. |
| A signal to send, with no room, or above `SOCKETS_SIGNAL`. | `false`, and GameNetworkingSockets tries again. |
| A signal of a connection with no slot. | A record whose `connection` is 0. |

## Functions

| Function | Result |
| --- | --- |
| `put_signal` | Writes a record of a signal into the ring. Gives `false` when it does not fit. |
| `send_signal` | Finds the entity of the connection of a signal, and puts the signal in the ring. |
| `release_signaling` | Does nothing. |
| `make_signaling` | Gives a signaling object of GameNetworkingSockets with the callbacks of the module. |
| `create_signaling` | Answers the callback of GameNetworkingSockets for a session with `make_signaling`. |
