# src/platform_abstraction/sockets_gns/sockets_declare.c

This file declares the types, the system and the observers of the sockets module to Bridge.

## The declarations

The module declares the 11 scalar types of the contract, then its 19 components, then its settings: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The declarations*.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `round` |
| Observer | write | `identity` on `ideatek:IdentityRequest` |
| Observer | write and detach | `listen` on `ideatek:ListenRequest` |
| Observer | write | `connection` on `ideatek:ConnectionRequest` |
| Observer | write | `lanes` on `ideatek:LanesRequest` |
| Observer | write | `session` on `ideatek:SessionRequest` |
| Observer | write | `address` on `ideatek:AddressRequest` |
| Observer | write | `messages_out` on `ideatek:MessagesOut` |
| Observer | write | `session_messages_out` on `ideatek:SessionMessagesOut` |
| Observer | write | `signals_in` on `ideatek:SignalsIn` |

`round` states as its changes the 10 components that the sockets module writes: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The grants*.
The observers `identity`, `listen`, `connection` and `session` state `ideatek:ListenState`, `ideatek:Inbound`, `ideatek:ConnectionState` and `ideatek:Session` as their changes. A request gets its state in the barrier that applies it.
The 4 other observers state no change.

`lists` keeps the list of each observer, in the order of the table.

## Functions

| Function | Result |
| --- | --- |
| `declare_scalars` | Declares the 11 scalar types. |
| `declare_components` | Declares the 19 components and the settings. |
| `add_observer` | Declares 1 observer on 1 component, with its events and its changes, and keeps its list. |
| `add_runs` | Declares the system and the 9 observers. |
