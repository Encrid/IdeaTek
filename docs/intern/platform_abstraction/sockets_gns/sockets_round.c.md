# src/platform_abstraction/sockets_gns/sockets_round.c

This file holds the system `round` of the first phase.

## The round

`round` does these steps:

1. The first round reads the settings and starts GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`. A broken module makes each round give 1.
2. `turn_half` destroys the entities of `gone`, and turns the half: `docs/intern/platform_abstraction/sockets_gns/sockets_half.c.md`. `drop_closed` frees their slots: `docs/intern/platform_abstraction/sockets_gns/sockets_table.c.md`.
3. `say_lines` writes the lines of GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
4. `SteamNetworkingSockets_Poll` with a wait of 0 does the network of GameNetworkingSockets: the packets in, the packets out, the timers.
5. `RunCallbacks` runs the callbacks of GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_status.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_session_peer.c.md`.
6. `receive_connections` and `receive_sessions` take the messages into the half: `docs/intern/platform_abstraction/sockets_gns/sockets_receive.c.md`.
7. `attach_signals` moves the signals of the ring into the half: `docs/intern/platform_abstraction/sockets_gns/sockets_signals_in.c.md`.
8. `attach_lanes` writes the lane states: `docs/intern/platform_abstraction/sockets_gns/sockets_lane_states.c.md`.
9. `publish_names` attaches the addresses that the thread found: `docs/intern/platform_abstraction/sockets_gns/sockets_names.c.md`.
10. `attach_states` attaches each state that changed, and empties the counts of the failed sends: `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md`.

## The manual poll mode

GameNetworkingSockets runs in its manual poll mode: `start_gns` sets it before the start: `docs/intern/platform_abstraction/sockets_gns/sockets_identity.c.md`. GameNetworkingSockets has no thread of its own then, and does the network inside `SteamNetworkingSockets_Poll`.
The round polls once. The observers of the batches out poll once after their triggers, as does the observer of the signals in. So a batch reaches the network before the next phase, in the barrier that took it: `docs/intern/platform_abstraction/sockets_gns/sockets_send.c.md`.

In bridge-host, the thread of GameNetworkingSockets sent and received only during a call of the module, or at its own timeout of 1 second. The manual poll mode gives the module the time of the network.
So each callback of GameNetworkingSockets runs on the thread of the phases, inside a call of the module.

An observer takes each request and each batch out at its barrier, so they wait for no round.
A message that came in a round reaches `ideatek:MessagesIn` in the round after.

## The branches

| Case | Result |
| --- | --- |
| The first round. | The settings, and the start. |
| A broken module. | 1, and nothing else. |

## Functions

| Function | Result |
| --- | --- |
| `round` | Runs the steps of the round. Gives 1 when the module is broken. |
