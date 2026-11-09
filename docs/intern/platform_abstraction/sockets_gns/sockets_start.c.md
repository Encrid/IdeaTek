# src/platform_abstraction/sockets_gns/sockets_start.c

This file takes the arena and starts GameNetworkingSockets in the first round.

## The first round

`start_sockets` does these steps:

1. `take_texts` takes the texts of the 9 text settings and of `debug_output` from the arena, `SOCKETS_TEXT` bytes each.
2. `check_numbers`, `copy_texts` and `check_words` check the settings: `docs/intern/platform_abstraction/sockets_gns/sockets_check.c.md`. A wrong setting breaks the module, and the steps after do not run.
3. `take_arena` takes the half of `message_bytes`, and the `SOCKETS_CERTIFICATE` bytes of the request of a certificate. It also takes the 2 rings, and the 2 lists of the entities that go. Then `take_tables` takes the 3 tables, with the sizes of `max_connections`, `max_listen_sockets` and `max_addresses`, and keeps `session_channels`.
4. `start_network` starts GameNetworkingSockets with the identity `ip:::1`: `docs/intern/platform_abstraction/sockets_gns/sockets_identity.c.md`, *The start*. A refused start breaks the module, with `no sockets: <reason>`.
5. It sets the debug output of GameNetworkingSockets to the level of `debug_output`: `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
6. It gives GameNetworkingSockets the settings and the 4 callbacks: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
7. It spawns the sockets entity, and attaches `ideatek:IdentityState` with `IDEATEK_AUTH_STATUS_NONE` to it: `docs/intern/platform_abstraction/sockets_gns/sockets_certificate.c.md`.

The settings are set before the first connection, so it sees them. GameNetworkingSockets takes a global value at any time, but a connection that exists keeps its own.

## The branches

| Case | Result |
| --- | --- |
| A wrong setting. | Its line, and `round` gives 1 in each round. |
| GameNetworkingSockets does not start. | `no sockets: <reason>`, and `round` gives 1 in each round. |

## Functions

| Function | Result |
| --- | --- |
| `take_texts` | Takes the texts of the settings from the arena. |
| `take_tables` | Takes the 3 tables from the arena, and keeps their sizes and the channels. |
| `take_arena` | Takes the half, the request of the certificate, the 2 rings, the 2 lists, and the tables. |
| `start_network` | Starts GameNetworkingSockets, gives it the settings, and attaches the first identity state. |
| `start_sockets` | Runs the steps of the first round. Gives `false` when the module is broken. |
