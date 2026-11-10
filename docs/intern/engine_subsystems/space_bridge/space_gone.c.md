# src/engine_subsystems/space_bridge/space_gone.c

This file runs the observer `gone`: it hears `ideatek:Transform` leave.

## The call

For a living entity, the function detaches `ideatek:WorldTransform` and `ideatek:PreviousWorldTransform`. Its children count as children of no parent, and `place` gives them their places again.
An entity with `ideatek:Children` and no `ideatek:WorldTransform` is the husk of a destroyed parent: Bridge removed each type but `ideatek:Children`.
For a husk, the function walks the children by `ideatek:Sibling`, and applies `orphan` of each one:

| `orphan` | Result |
| --- | --- |
| `IDEATEK_ORPHAN_DESTROY` | The module destroys the child. |
| `IDEATEK_ORPHAN_KEEP_WORLD` | The child becomes a root at its place: `docs/intern/engine_subsystems/space_bridge/space_keep.c.md`. |

Then it detaches `ideatek:Children`, and the husk goes.
A living parent whose transform was refused has no `ideatek:WorldTransform`. If it loses its transform, the module takes it for a husk: Bridge gives no mark of a husk.

## Functions

| Function | Result |
| --- | --- |
| `is_husk` | `true` when the entity carries `ideatek:Children` and no place. |
| `orphan_child` | Applies `orphan` of 1 child. |
| `release` | Applies `orphan` to each child of a husk, and detaches `ideatek:Children`. |
| `gone` | The observer of the leave of `ideatek:Transform`. Gives 0. |
