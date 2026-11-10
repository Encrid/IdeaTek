# src/engine_subsystems/space_bridge/space_walk.c

This file places an entity and its descendants, from a stack in the scratch arena: `docs/intern/engine_subsystems/space_bridge/space_one.c.md`.

The first item takes the place of its parent, when the parent is alive and has a transform and a place. Else the entity counts as a root.
Each child takes the place that the walk just computed for its parent. The walk goes through the whole tree under the first entity.
When the stack is full, the walk stops, and the log gets 1 line. The rest of the tree waits for its next change.

## Functions

| Function | Result |
| --- | --- |
| `get_parent_place` | The place of the parent. `false` when the entity counts as a root. |
| `push_children` | Pushes the children of an entity. |
| `say_full` | Writes the line of a full stack. |
| `walk` | Places an entity and its tree. |
