# src/engine_subsystems/space_bridge/space_ancestor.c

This file walks up the parents of an entity, up to a root or into a cycle. It also gives the next child of a list.

`get_next_child` reads `next` of `ideatek:Sibling`. The loops over a list stop at `count` of `ideatek:Children`, so a list that a bug broke cannot run them forever.

The walk finds a cycle by the method of Brent. A mark stays at an entity, and jumps to the walker after 1, 2, 4 and more steps.
The walker meets the mark only in a cycle, so the walk needs no bound and no memory beyond 2 ids.

| Case | Result |
| --- | --- |
| The walker meets the mark. | `ANCESTOR_CYCLE` |
| An ancestor is in the set of the call. | `ANCESTOR_CHANGED`: the walk of that ancestor places the entity. |
| Else | `ANCESTOR_ROOT`: the module walks from the entity. |

## Functions

| Function | Result |
| --- | --- |
| `get_next_child` | The child after a child in its list, or 0. |
| `get_ancestor_state` | The result of the walk. |
