# src/engine_subsystems/space_bridge/space_link.c

This file runs the observer `link`: it moves each child whose `ideatek:Parent` changed to the list of its new parent.

## The call

1. For each child, the function reads its old parent in `ideatek:Sibling`, and its new parent in `ideatek:Parent`.
2. The function marks each old parent and each new parent in a set of the scratch arena.
3. For each marked parent, it writes the list again: `docs/intern/engine_subsystems/space_bridge/space_lists.c.md`.
4. A child with no new parent loses `ideatek:Sibling`, and becomes a root.

With `IDEATEK_KEEP_WORLD`, the function attaches the transform that keeps the place in the world under the new parent: `docs/intern/engine_subsystems/space_bridge/space_keep.c.md`.
The new place comes at the next barrier, when `relink` hears `ideatek:Sibling`.

## Functions

| Function | Result |
| --- | --- |
| `old_parent` | `parent` of the `ideatek:Sibling` of a child, or 0. |
| `mark_parents` | Puts the old and the new parents of the list in the set. |
| `rewrite` | Writes the list of 1 parent again. |
| `link` | The observer of `ideatek:Parent`. Gives 0. |
