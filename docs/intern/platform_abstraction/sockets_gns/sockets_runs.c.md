# src/platform_abstraction/sockets_gns/sockets_runs.c

This file declares the system and the 9 observers of the sockets module to Bridge.

## The calls

| Call | Kind | Name | Changes |
| --- | --- | --- | --- |
| System | `first` | `round` | The 10 components that the module writes. |
| Observer | write | `identity` on `ideatek:IdentityRequest` | `ideatek:IdentityState`, `ideatek:ConnectionState`, `ideatek:ListenState` |
| Observer | write and detach | `listen` on `ideatek:ListenRequest` | `ideatek:ListenState` |
| Observer | write | `connection` on `ideatek:ConnectionRequest` | `ideatek:ConnectionState`, `ideatek:Inbound` |
| Observer | write | `lanes` on `ideatek:LanesRequest` | None |
| Observer | write | `session` on `ideatek:SessionRequest` | `ideatek:Session`, `ideatek:ConnectionState` |
| Observer | write | `address` on `ideatek:AddressRequest` | `ideatek:AddressResult` |
| Observer | write | `messages_out` on `ideatek:MessagesOut` | None |
| Observer | write | `session_messages_out` on `ideatek:SessionMessagesOut` | None |
| Observer | write | `signals_in` on `ideatek:SignalsIn` | None |

A write is `ON_REQUEST`: `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH`, so a request on an entity that had one before triggers too.
`round` states as its changes the 10 components of `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The grants*. An observer states the components that its requests write, so a request gets its state in the barrier that applies it.

## The order

2 observers that change the same component run in a stated order. `listen` and `connection` run after `identity`, and `session` runs after `identity` and `connection`. So an identity change of the same barrier gives the connections their problem before a new request opens one.
`add_requests` declares the first 3 observers. `add_sessions` declares the next 3 with the slots of the first ones as their order. `add_runs` declares the system, calls both, and declares the 3 observers of the batches.

`lists` keeps the list of each observer, in the order of the table: `LIST_*` of the header gives each index.

## Functions

| Function | Result |
| --- | --- |
| `add_requests` | Declares `identity`, `listen` and `connection`. |
| `add_sessions` | Declares `lanes`, `session` and `address`. |
| `add_runs` | Declares the system and the 9 observers. |
