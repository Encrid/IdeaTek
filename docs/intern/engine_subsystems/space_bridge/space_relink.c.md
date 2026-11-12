# src/engine_subsystems/space_bridge/space_relink.c

This file runs the observer `relink`. It places the entities of its list as `place` does: `docs/intern/engine_subsystems/space_bridge/space_place.c.md`. A child with a new sibling link is placed under its new parent.

## Functions

| Function | Result |
| --- | --- |
| `relink_all` | Places the list of `relink`, and publishes its moves. |
| `relink` | The observer of `ideatek:Sibling`. Runs `relink_all` through `run_started`. |
