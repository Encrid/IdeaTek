# src/engine_subsystems/space_bridge/space_link.c

This file runs the observer `link`: each child whose `ideatek:Parent` changed moves to the list of its new parent.

1. The old parent is `parent` of `ideatek:Sibling`, and the new parent is `entity` of `ideatek:Parent`.
2. A child with no new parent loses `ideatek:Sibling`. With `IDEATEK_KEEP_WORLD`, a new parent keeps the place: `docs/intern/engine_subsystems/space_bridge/space_keep.c.md`.
3. The function puts each old and new parent in a set, then writes the list of each one again: `docs/intern/engine_subsystems/space_bridge/space_lists.c.md`.

When the set cannot hold the parents, the function writes the 2 lists of each child at once. 2 writes of 1 list in 1 call give the same list.
When the buffer of a list does not fit, the list keeps its old links, and the log gets 1 line.

## Functions

| Function | Result |
| --- | --- |
| `get_old_parent` | The parent of the old list of a child, or 0. |
| `rewrite` | Writes the list of 1 parent again. |
| `move_child` | Notes the 2 parents of a child, or writes their lists. |
| `link` | The observer of `ideatek:Parent`. Gives 0. |
