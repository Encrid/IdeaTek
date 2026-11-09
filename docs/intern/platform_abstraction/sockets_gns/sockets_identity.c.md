# src/platform_abstraction/sockets_gns/sockets_identity.c

This file starts GameNetworkingSockets with an identity, and runs the observer of `ideatek:IdentityRequest`.

## The start

`start_gns` puts GameNetworkingSockets in its manual poll mode, starts it with `identity`, takes its 3 interfaces, and gives `true`: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*. A refused start gives `false` with the reason of GameNetworkingSockets in `reason`.
The first round calls it with `ip:::1`: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The request

The observer `identity` gets each write of `ideatek:IdentityRequest` on the sockets entity. `apply_identity` runs each one:

1. `SteamAPI_SteamNetworkingIdentity_ParseString` reads `identity.text`. A text that GameNetworkingSockets refuses writes `the identity <text> is refused`, and nothing changes.
2. `restart` runs `stop_all`, which gives each connection its problem and each listen socket its failure: `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.
3. GameNetworkingSockets stops, and `start_gns` starts it again with the identity. When it refuses, the module writes `no sockets: <reason>` and is broken.
4. `apply_settings` gives the settings again, because the stop forgot them: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
5. `set_certificate` gives `certificate` to GameNetworkingSockets, and `attach_identity` attaches `ideatek:IdentityState`: `docs/intern/platform_abstraction/sockets_gns/sockets_certificate.c.md`.

A request on another entity than the sockets entity is ignored, with no line: the contract puts the request on the sockets entity.

## The branches

| Case | Result |
| --- | --- |
| A text that is no identity. | The line of the identity; the state does not change. |
| A request on another entity. | Nothing. |
| GameNetworkingSockets does not start again. | `no sockets: <reason>`, and a broken module. |

## Functions

| Function | Result |
| --- | --- |
| `start_gns` | Starts GameNetworkingSockets with an identity in the manual poll mode, and takes its interfaces. Gives `false` with the reason when it refuses. |
| `restart` | Stops GameNetworkingSockets and starts it with a new identity, then applies the settings and the certificate again. |
| `apply_identity` | Reads the identity of a request, and restarts with it, or writes its line. |
| `identity` | Runs `apply_identity` for each trigger on the sockets entity. |
