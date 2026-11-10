# src/engine_subsystems/space_bridge/space_walk.c

This file places an entity and its descendants: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*.

## The walk

The walk keeps its stack in the scratch arena: each item is an entity and the place of its parent. So a deep tree uses no stack of the thread.
For each entity:

1. The place of the parent is the place that the walk just computed, or the `ideatek:WorldTransform` of the parent. A dead parent, or a parent with no place, counts as no parent.
2. A refused transform gives no place: the module detaches `ideatek:WorldTransform`, writes 1 line, and skips the children.
3. A new place with the same bytes stops the walk there: the children keep their places.
4. Else the module attaches the new place, and pushes the children of the entity.

With `ideatek:Teleported`, the walk applies `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:Teleported*, to the entity and its descendants. Then it detaches the tag.
When the scratch arena cannot hold the stack, the walk stops, and the log gets 1 line. The rest of the tree waits for its next change.

## Functions

| Function | Result |
| --- | --- |
| `parent_place` | The place of the parent, or none. |
| `place_one` | Places 1 entity. `true` when its children need a new place. |
| `push_children` | Pushes the children of an entity on the stack. |
| `walk` | Places an entity and its descendants. |
