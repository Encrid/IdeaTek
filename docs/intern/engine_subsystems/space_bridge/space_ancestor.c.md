# src/engine_subsystems/space_bridge/space_ancestor.c

This file walks up the parents of an entity: it finds an ancestor in the set of the call, a root, or a cycle.

## The walk

The walk reads `ideatek:Parent` of each entity, up to an entity with no parent or with a dead parent.
It finds a cycle by the method of Brent. A mark stays at an entity, and jumps to the walker after 1, 2, 4 and more steps.
The walker meets the mark only in a cycle.
So the walk needs no bound and no memory beyond 2 ids.

## The branches

| Case | Result |
| --- | --- |
| An ancestor is in the set. | `ANCESTOR_CHANGED`: the walk of that ancestor places the entity. |
| The walk reaches a root. | `ANCESTOR_ROOT`: the module places the entity. |
| The walker meets the mark. | `ANCESTOR_CYCLE`: the entity gets no place, and the log gets 1 line. |

## Functions

| Function | Result |
| --- | --- |
| `parent_of` | The parent of an entity, or 0. |
| `ancestor_state` | The result of the walk. |
