# src/engine_subsystems/space_bridge/space_main.c

This file holds the 3 entries of the module: `init`, `shutdown` and `reload`.

## The entries

`init` declares the types: `docs/intern/engine_subsystems/space_bridge/space_declare.c.md`.
Then it registers the 5 observers and the system: `docs/intern/engine_subsystems/space_bridge/space_observe.c.md`.
The module has no setting, so `init` reads no configuration.

`shutdown` gives 0: the module holds no resource outside the world.
`reload` gives 1, as the manifest states `reloadable` at `false`.

## Functions

| Function | Result |
| --- | --- |
| `init` | 0 when the declarations and the registrations pass. |
| `shutdown` | 0. |
| `reload` | 1. |
