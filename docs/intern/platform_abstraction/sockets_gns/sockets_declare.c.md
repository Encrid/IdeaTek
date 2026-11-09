# src/platform_abstraction/sockets_gns/sockets_declare.c

This file declares the types of the sockets module to Bridge.

## The declarations

The module declares the 12 scalar types of the contract, then its 19 components, then its settings: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The declarations*.
`SOCKETS_TYPE` of the header declares a type of the contract as a table, stripped, removed at the destroy of its entity. The settings are a singleton.

| Function | Types |
| --- | --- |
| `declare_scalars` | `ideatek_entity_t`, `ideatek_address_t`, `ideatek_identity_t`, and the 2 bitmasks `ideatek_send_t` and `ideatek_connection_flag_t`. |
| `declare_enums` | The 7 enums, from `ideatek_listen_kind_t` to `ideatek_address_status_t`. |
| `declare_types` | The 7 components of the identity, the listen sockets and the connections, after the scalars and the enums. Then the batches, then `ideatek_sockets_gns:Settings`. |
| `declare_batches` | The 12 components of the batches, the sessions and the addresses. |

The order of the declarations is the order of the slots of the header: `docs/intern/platform_abstraction/sockets_gns/sockets_inside.h.md`, *The slots*. The system and the observers are in `docs/intern/platform_abstraction/sockets_gns/sockets_runs.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `declare_scalars` | Declares the 3 structs and the 2 bitmasks. |
| `declare_enums` | Declares the 7 enums. |
| `declare_batches` | Declares the 12 components of the batches, the sessions and the addresses. |
| `declare_types` | Declares every type, in the order of the slots. |
