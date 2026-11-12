# src/engine_subsystems/space_bridge/space_declare.c

This file declares the types of the space part, `ideatek:Tick` of the frame part as a singleton, and the settings of the module.

`declare_fields` declares the 7 types of the fields first. `declare_types` declares the other types after them.
`ideatek:Children` and `ideatek:Sibling` take `BRIDGE_ON_DESTROY_KEEP`, and each other type takes `BRIDGE_ON_DESTROY_REMOVE`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *The declarations*.
`ideatek:WorldMoves` and `ideatek_space_bridge:Settings` are singletons.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the types of the fields. |
| `declare_types` | Declares each type, in the order of the slots. |
