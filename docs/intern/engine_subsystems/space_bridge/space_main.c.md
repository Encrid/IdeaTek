# src/engine_subsystems/space_bridge/space_main.c

This file holds the 3 entries of the module, and the manifest in its own section.

`init` sets `space_module` to zeros, declares the types and registers the observers and the system: `docs/intern/engine_subsystems/space_bridge/space_declare.c.md` and `docs/intern/engine_subsystems/space_bridge/space_observe.c.md`.
Bridge writes the settings after `init`, so `init` reads no setting. `run_started` reads them at the start of the ring: `docs/intern/engine_subsystems/space_bridge/space_moves.c.md`. `shutdown` gives 0. `reload` gives 1, as the manifest states `reloadable` at `false`.

## Functions

| Function | Result |
| --- | --- |
| `init` | 0. |
| `shutdown` | 0. |
| `reload` | 1. |
