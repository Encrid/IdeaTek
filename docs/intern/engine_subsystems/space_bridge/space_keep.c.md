# src/engine_subsystems/space_bridge/space_keep.c

This file keeps the place in the world of a child whose parent changes or goes.

`keep_world` attaches the transform that gives the child its place under its new parent: `docs/intern/engine_subsystems/space_bridge/space_local.c.md`.
A new parent with no place makes the child a root at its place. A new parent with no inverse leaves the transform, and the log gets 1 line.
`keep_orphan` detaches `ideatek:Parent` and `ideatek:Sibling`, and attaches the place of the child as the transform of a root.

## Functions

| Function | Result |
| --- | --- |
| `get_root_transform` | The transform of a root at a place. |
| `keep_world` | Attaches the transform of the child under its new parent. |
| `keep_orphan` | Makes the child a root at its place. |
