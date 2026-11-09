# src/platform_abstraction/sockets_gns/sockets_signals_in.c

This file gives GameNetworkingSockets the signals that came in, and attaches the signals out in each round.

## The signals in

The observer `signals_in` gets each write of `ideatek:SignalsIn`. For each record, `read_signal` reads the bytes: `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md`. Then `ReceivedP2PCustomSignal2` gives them to GameNetworkingSockets, with `on_connect_request` and no rejection callback.
`on_connect_request` gives a signaling for a connection that a peer opens: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md`. The connection then comes as an inbound one, on the P2P listen socket of its virtual port.

GameNetworkingSockets calls the rejection callback only for a connection that the application closes inside the call. The callbacks of the module run later, from `RunCallbacks`, so the module gives none. GameNetworkingSockets refuses a signal for a virtual port where the module does not listen, with the line of a refused signal.
A signal that GameNetworkingSockets refuses writes `a signal of <count> bytes is refused`. A record that does not fit writes the line of a malformed batch, and the records after it are left.
After the triggers, the observer polls GameNetworkingSockets once, so a signal that answers a signal goes out in the same barrier.

## The round

`attach_signals` runs once a round. It moves the records of the ring `signals` into the half, and attaches `ideatek:SignalsOut` to the sockets entity if a record moved.
A record that does not fit in the half stays in the ring for the next round. The tail of the ring goes back to its place before the record, by arithmetic.

## The branches

| Case | Result |
| --- | --- |
| A signal in for a listen socket. | A connection comes, as an inbound one. |
| A signal in for no listen socket. | The line of a refused signal. |
| A signal that GameNetworkingSockets refuses. | Its line. |
| A malformed record. | The line of the batch, and nothing more of the batch. |
| No signal in the ring. | No attach. |
| No room in the half for a signal. | It waits for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `on_connect_request` | Gives a signaling for a connection that a peer opens. |
| `give_signals` | Reads a batch of signals, and gives each one to GameNetworkingSockets. |
| `signals_in` | Reads the triggers of the observer, gives each batch, and polls once. |
| `attach_signals` | Moves the ring into the half, and attaches `ideatek:SignalsOut`. |
