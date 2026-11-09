# src/platform_abstraction/sockets_gns/sockets_start.c

This file checks the settings, takes the arena and starts GameNetworkingSockets in the first round.

## The first round

`start_sockets` does these steps:

1. It checks each number setting against its range: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`, *The table*. A wrong value writes `<setting>: <value> is outside <range>`, and the module is broken: `readme.md`, *The contracts*.
2. It copies each text setting from the arena into its text of `SOCKETS_TEXT` bytes. A text outside the arena, or too long, writes the line of the text, and the module is broken.
3. It checks each word setting against its words: `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md`. A wrong word writes its line, and the module is broken.
4. It takes the arena: `message_bytes` for the half, 512 for `certificate_request`, the 3 tables and the 2 rings. The tables have the sizes of `max_connections`, `max_listen_sockets` and `max_addresses`.
5. It sets the debug output of GameNetworkingSockets to the level of `debug_output`: `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.
6. It gives GameNetworkingSockets the settings and the 3 callbacks: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
7. It starts GameNetworkingSockets with the identity `ip:::1`: `docs/intern/platform_abstraction/sockets_gns/sockets_identity.c.md`, *The start*. A refused start breaks the module, with `no sockets: <reason>`.
8. It spawns the sockets entity, and attaches `ideatek:IdentityState` with `IDEATEK_AUTH_STATUS_NONE` to it.

The settings are set before the start, so the first connection sees them. GameNetworkingSockets takes a global value at any time, but a connection that exists keeps its own.

## The branches

| Case | Result |
| --- | --- |
| A number setting outside its range. | Its line, and `round` gives 1 in each round. |
| A text setting outside its range or the arena. | Its line, and `round` gives 1 in each round. |
| A word setting with a wrong word. | Its line, and `round` gives 1 in each round. |
| GameNetworkingSockets does not start. | `no sockets: <reason>`, and `round` gives 1 in each round. |

## Functions

| Function | Result |
| --- | --- |
| `check_numbers` | Checks each number setting against its range, and writes the line of each wrong one. |
| `copy_text` | Copies 1 text setting from the arena. Gives `false` with its line when the text does not fit. |
| `take_arena` | Takes the half, the request of the certificate, the 3 tables and the 2 rings. |
| `start_sockets` | Runs the steps of the first round. Gives `false` when the module is broken. |
