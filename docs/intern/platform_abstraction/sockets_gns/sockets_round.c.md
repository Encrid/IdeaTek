# src/platform_abstraction/sockets_gns/sockets_round.c

This file holds the system `round` of the first phase.

## The round

`round` does these steps:

1. The first round reads the settings and starts GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`. A broken module makes each round give 1.
2. `turn_half` destroys the entities of `gone`, and turns the half: `docs/intern/platform_abstraction/sockets_gns/sockets_half.c.md`. `drop_closed` frees their slots: `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.
3. `say_lines` writes the lines of GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
4. `gns_run_callbacks` runs the callbacks of GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_status.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_sessions.c.md`.
5. `receive_connections` and `receive_sessions` take the messages into the half: `docs/intern/platform_abstraction/sockets_gns/sockets_receive.c.md`.
6. `attach_signals` moves the signals of the ring into the half: `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md`.
7. `attach_lanes` writes the lane states: `docs/intern/platform_abstraction/sockets_gns/sockets_lanes.c.md`.
8. `publish_names` attaches the addresses that the thread found: `docs/intern/platform_abstraction/sockets_gns/sockets_names.c.md`.
9. `attach_states` attaches each state that changed, and empties the counts of the failed sends: `docs/intern/platform_abstraction/sockets_gns/sockets_state.c.md`.

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
