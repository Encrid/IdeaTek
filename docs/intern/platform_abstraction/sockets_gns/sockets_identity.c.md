# src/platform_abstraction/sockets_gns/sockets_identity.c

This file starts GameNetworkingSockets with an identity, and runs the observer of `ideatek:IdentityRequest`.

## The start

`start_gns` starts GameNetworkingSockets with `identity`, takes its 3 interfaces, and gives `true`. A refused start gives `false` with the reason of GameNetworkingSockets in `reason`.
The first round calls it with `ip:::1`: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The request

The observer `identity` gets each write of `ideatek:IdentityRequest` on the sockets entity:

1. `gns_identity_parse` reads `identity.text`. A text that GameNetworkingSockets refuses writes `the identity <text> is refused`, and nothing changes.
2. `stop_all` gives each connection its problem and each listen socket its failure: `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.
3. GameNetworkingSockets stops, and `start_gns` starts it again with the identity. When it refuses, the module writes `no sockets: <reason>` and is broken.
4. `apply_settings` gives the settings again, because the stop forgot them: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`.
5. With a `certificate` of 1 byte or more, `set_certificate` gives it to GameNetworkingSockets. A refused certificate writes `the certificate is refused: <reason>`.
6. `attach_identity` attaches `ideatek:IdentityState`: the identity, the request of a certificate, and the status.

A request on another entity than the sockets entity is ignored, with no line: the contract puts the request on the sockets entity.

## The state

`attach_identity` asks GameNetworkingSockets for the request of a certificate, into the 512 bytes of `certificate_request`. A request that does not fit, or that GameNetworkingSockets does not give, leaves a text of 0 bytes.
The status is `IDEATEK_AUTH_STATUS_CURRENT` after a certificate that GameNetworkingSockets took, `IDEATEK_AUTH_STATUS_FAILED` after one that it refused, else `IDEATEK_AUTH_STATUS_NONE`.

## The branches

| Case | Result |
| --- | --- |
| A text that is no identity. | The line of the identity; the state does not change. |
| A request on another entity. | Nothing. |
| GameNetworkingSockets does not start again. | `no sockets: <reason>`, and a broken module. |
| A certificate of 0 bytes. | `IDEATEK_AUTH_STATUS_NONE`. |
| A refused certificate. | Its line, and `IDEATEK_AUTH_STATUS_FAILED`. |
| No request of a certificate from GameNetworkingSockets. | `certificate_request` of 0 bytes. |

## Functions

| Function | Result |
| --- | --- |
| `start_gns` | Starts GameNetworkingSockets with an identity, and takes its interfaces. Gives `false` with the reason when it refuses. |
| `set_certificate` | Gives a certificate to GameNetworkingSockets, and gives the status that follows. |
| `attach_identity` | Attaches `ideatek:IdentityState` to the sockets entity. |
| `restart` | Stops GameNetworkingSockets and starts it with a new identity, then applies the settings again. |
| `identity` | Runs the steps of each request of the observer. |
