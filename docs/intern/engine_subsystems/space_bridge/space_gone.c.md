# src/engine_subsystems/space_bridge/space_gone.c

This file runs the observer `gone`: it hears `ideatek:Transform` leave.

The function detaches the place and the place before of each entity of the list. Then it visits the children of the entity.
A living parent with no transform counts as no parent, so the module walks each child as a root.
An entity with `ideatek:Children` and no place is the husk of a destroyed parent, since the observer hears its transform leave. Bridge gives no mark of a husk.

For a husk, each child goes by its `orphan`, and the module detaches `ideatek:Children`.
A child whose `ideatek:Parent` names another entity, or no entity, left the husk at that barrier, and stays:

| `orphan` | Result |
| --- | --- |
| `IDEATEK_ORPHAN_DESTROY` | The module destroys the child. |
| `IDEATEK_ORPHAN_KEEP_WORLD` | The child becomes a root at its place: `docs/intern/engine_subsystems/space_bridge/space_keep.c.md`. |

## Functions

| Function | Result |
| --- | --- |
| `is_husk` | `true` when the entity carries `ideatek:Children` and no place. |
| `release_child` | Applies `orphan` of 1 child that still names the husk. |
| `visit_children` | Releases the children of a husk, or walks the children of a living parent. |
| `gone` | The observer. Gives 0. |
