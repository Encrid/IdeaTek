# src/engine_subsystems/space_bridge/space_one.c

This file places 1 entity of a walk.

1. An entity with no `ideatek:Transform` stops the walk there.
2. A refused transform detaches `ideatek:WorldTransform`, writes 1 line, and stops the walk there.
3. The place is the composition with the place of the parent of the item, with `absolute` of its `ideatek:Parent`.
4. The place before goes on as `keep_previous` tells.
5. An entity with no place yet gets it by an attach, with `revision` 1. An entity whose origin or basis changed gets the place written in place, with its `revision` plus 1.
   An equal place is not written. The walk goes on to the children in each case.

`keep_previous` sets `ideatek:PreviousWorldTransform` to the new place in 2 cases: for an interpolated entity that has no place before yet, and with a teleport.
The first case attaches the type. The teleport writes it in place, since the entity has it.
A teleport is `ideatek:Teleported` of the entity or the mark of its item, which the walk gives to the descendants. The module detaches the tag.

## Functions

| Function | Result |
| --- | --- |
| `refuse_transform` | Detaches the place and writes the line. Gives `false`. |
| `get_absolute` | `absolute` of the parent of an entity, or 0. |
| `keep_previous` | Sets the place before when it must change. |
| `write_world` | Attaches the first place, or writes a changed place in place with the next revision. |
| `place_one` | Places 1 entity. `true` when the walk goes on to the children. |
