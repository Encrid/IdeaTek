# src/platform_abstraction/sockets_gns/sockets_certificate.c

This file gives a certificate to GameNetworkingSockets, and attaches `ideatek:IdentityState`.

## The certificate

`set_certificate` takes the `certificate` of an identity request. A text of 0 bytes gives the status `IDEATEK_AUTH_STATUS_NONE`, and GameNetworkingSockets gets nothing.
Else GameNetworkingSockets gets the bytes, and 1 line tells the outcome: `the certificate is taken`, or `the certificate is refused: <reason>` as a warning. The status is `IDEATEK_AUTH_STATUS_CURRENT` or `IDEATEK_AUTH_STATUS_FAILED`.
The line and the status come from tables by the outcome, with no branch. A certificate that GameNetworkingSockets takes is signed by the certificate authority of Valve, so no case can make one.

## The state

`attach_identity` asks GameNetworkingSockets for the request of a certificate, into the `SOCKETS_CERTIFICATE` bytes of the arena at `certificate_at`. A request that does not fit, or that GameNetworkingSockets does not give, leaves a text of 0 bytes.
Then it attaches `ideatek:IdentityState` to the sockets entity: the text of the identity, the request, and the status.
The first round calls it after the start: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`. An identity request calls it after the restart: `docs/intern/platform_abstraction/sockets_gns/sockets_identity.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A certificate of 0 bytes. | `IDEATEK_AUTH_STATUS_NONE`, and no line. |
| A certificate of 1 byte or more. | The line of the outcome, and the status of the outcome. |
| No request of a certificate from GameNetworkingSockets. | A request of 0 bytes. |

## Functions

| Function | Result |
| --- | --- |
| `set_certificate` | Gives a certificate to GameNetworkingSockets, writes the line of the outcome, and sets the status. |
| `attach_identity` | Attaches `ideatek:IdentityState` to the sockets entity. |
