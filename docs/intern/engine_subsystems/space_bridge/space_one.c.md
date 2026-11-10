# src/engine_subsystems/space_bridge/space_one.c

This file places 1 entity of a walk.

1. An entity with no `ideatek:Transform` stops the walk there.
2. A refused transform detaches `ideatek:WorldTransform`, writes 1 line, and stops the walk there.
3. The place is the composition with the place of the parent of the item, with `absolute` of its `ideatek:Parent`.
4. The place before goes on as `keep_previous` tells.
5. The module attaches the place only when its bytes differ. The walk goes on to the children in each case.

`keep_previous` attaches `ideatek:PreviousWorldTransform` at the new place in 2 cases: for an interpolated entity that has no place before yet, and with a teleport.
A teleport is `ideatek:Teleported` of the entity or the mark of its item, which the walk gives to the descendants. The module detaches the tag.

## Functions

| Function | Result |
| --- | --- |
| `refuse_transform` | Detaches the place and writes the line. Gives `false`. |
| `get_absolute` | `absolute` of the parent of an entity, or 0. |
| `keep_previous` | Attaches the place before when it must change. |
| `place_one` | Places 1 entity. `true` when the walk goes on to the children. |
