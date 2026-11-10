# src/engine_subsystems/space_bridge/space_declare.c

This file declares the types of the space part, and `ideatek:Tick` of the frame part.

## The declarations

`declare_fields` declares the 7 types of the fields, each one before the types that hold it.
`declare_space` declares the 8 components and tags of the space part, then `ideatek:Tick` as a singleton.
`ideatek:Children` takes `BRIDGE_ON_DESTROY_KEEP`, and each other type takes `BRIDGE_ON_DESTROY_REMOVE`, as `docs/extern/engine_subsystems/ideatek_space.h.md`, *The declarations*, tells.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the types of the fields. |
| `declare_space` | Declares the other types, and keeps their slots in `space_module`. |
