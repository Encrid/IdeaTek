# include/engine_subsystems/ideatek_space.h

`include/engine_subsystems/ideatek_space.h` gives the types of the place of an entity in the world: their C types and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields name entities with `ideatek_entity_t` of `include/platform_abstraction/ideatek_world.h`, so the header includes it.

## The space

- The unit of length is 1 meter.
- The axes are right-handed. +Y is up. The forward direction of a camera is -Z.
- The front of a model faces +Z.
- A position in the world has 3 `double` values. A direction, a velocity and a scale have 3 `float` values.
- A 2D game uses the same types. It keeps its entities in the XY plane and turns them only about +Z. Z is the depth of the draw order.

## The roles

- A mover writes `ideatek:Transform`, `ideatek:Parent`, `ideatek:Interpolated` and `ideatek:Teleported`. Physics, animation and a character controller are movers.
- The space module reads `ideatek:Transform`, `ideatek:Parent` and `ideatek:Tick`. It writes `ideatek:WorldTransform`, `ideatek:Children`, `ideatek:Sibling` and `ideatek:PreviousWorldTransform`.
- A reader reads `ideatek:WorldTransform`. Render, audio and the net modules are readers.

`ideatek:WorldTransform` has one writer: the space module.
The space module also writes `ideatek:Transform` and `ideatek:Parent` in the cases that `keep` and `orphan` tell, and detaches `ideatek:Teleported`.
At the same barrier, the attach of the space module comes after the attach of a mover.

## ideatek:ideatek_vec3_t

`ideatek_vec3_t` is a struct of 12 bytes, with an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The value on the X axis. |
| `y` | `float` | 4 | The value on the Y axis. |
| `z` | `float` | 8 | The value on the Z axis. |

## ideatek:ideatek_quat_t

`ideatek_quat_t` is a rotation: a quaternion of length 1. It is a struct of 16 bytes, with an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The X part of the axis. |
| `y` | `float` | 4 | The Y part of the axis. |
| `z` | `float` | 8 | The Z part of the axis. |
| `w` | `float` | 12 | The real part. |

The rotation that does not turn is `{ 0, 0, 0, 1 }`.

## ideatek:ideatek_position_t

`ideatek_position_t` is a position in the world. It is a struct of 24 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `double` | 0 | The position on the X axis, in meters. |
| `y` | `double` | 8 | The position on the Y axis, in meters. |
| `z` | `double` | 16 | The position on the Z axis, in meters. |

## ideatek:ideatek_keep_t

`ideatek_keep_t` is an enum of 1 byte. It tells what stays when an entity gets a new parent.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_KEEP_LOCAL` | `ideatek:Transform` stays. The place in the world changes. |
| 1 | `IDEATEK_KEEP_WORLD` | The place in the world stays. The space module attaches the new `ideatek:Transform`. |

## ideatek:ideatek_orphan_t

`ideatek_orphan_t` is an enum of 1 byte. It tells what occurs to an entity when its parent is destroyed.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ORPHAN_DESTROY` | The space module destroys the entity. |
| 1 | `IDEATEK_ORPHAN_KEEP_WORLD` | The entity becomes a root at its place in the world. The space module detaches `ideatek:Parent` and attaches the new `ideatek:Transform`. |

## ideatek:Transform

`ideatek:Transform` is the place of an entity relative to its parent, or relative to the world when the entity has no parent.
Its C type `ideatek_transform_t` has 56 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `ideatek_position_t` | 0 | The position. |
| `rotation` | `ideatek_quat_t` | 24 | The rotation. |
| `scale` | `ideatek_vec3_t` | 40 | The scale on each axis. |

The bytes 52 to 55 hold no field, because the size is a multiple of 8.
A point of the entity goes first through `scale`, then through `rotation`, then through `position`.
The value that does not move is `{ { 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 } }`. A value of zeros is not this value: its scale is 0.
The space module refuses a value that has a NaN or an infinity, or a rotation of length 0. It attaches no `ideatek:WorldTransform` for it, and it writes 1 line to the log.

## ideatek:Parent

`ideatek:Parent` names the parent of an entity. An entity with no `ideatek:Parent` is a root.
Its C type `ideatek_parent_t` has 16 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `entity` | `ideatek_entity_t` | 0 | The parent. |
| `order` | `int32_t` | 8 | The rank among the children of the parent. |
| `keep` | `ideatek_keep_t` | 12 | What stays when the parent changes. |
| `orphan` | `ideatek_orphan_t` | 13 | What occurs when the parent is destroyed. |
| `absolute` | `uint8_t` | 14 | The parts of the place that do not come from the parent. |

The byte 15 holds no field, because the size is a multiple of 8.
Each bit of `absolute` takes a part of the place from `ideatek:Transform` alone:

| Bit | Name | Part |
| --- | --- | --- |
| 1 | `IDEATEK_ABSOLUTE_POSITION` | The position. |
| 2 | `IDEATEK_ABSOLUTE_ROTATION` | The rotation. |
| 4 | `IDEATEK_ABSOLUTE_SCALE` | The scale. |

When `absolute` is 0, the entity takes each part from its parent.

## ideatek:WorldTransform

`ideatek:WorldTransform` is the place of an entity in the world. It can hold a shear, which a non-uniform scale under a rotated parent gives.
Its C type `ideatek_world_transform_t` has 64 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `origin` | `ideatek_position_t` | 0 | The position in the world. |
| `basis` | `float[9]` | 24 | The X axis, then the Y axis, then the Z axis of the entity in the world: 3 columns of 3 values. |

The bytes 60 to 63 hold no field, because the size is a multiple of 8.

The space module attaches `ideatek:WorldTransform` to each entity that has `ideatek:Transform`, and detaches it when `ideatek:Transform` goes.
An observer of the space module attaches the value at the barrier that applies the change of `ideatek:Transform` or `ideatek:Parent`.
The deployment gives the space module `observer_passes` of 2 or more.
The space module attaches a value only when its bytes differ from the bytes of the old value. Then it goes on to the children.

In 1 call, the observer places only the changed entities that have no changed ancestor in that call. The walk of the ancestor places the others, so each entity is placed once.
A reader that waits for a change observes `BRIDGE_ON_ATTACH` of `ideatek:WorldTransform`.

```pseudo
PROCEDURE place(e : U64)
   local <- matrix_of(transform_of(e))
   p <- parent_of(e)
   IF p = 0 OR NOT has_world(p) THEN
      new <- local
   ELSE
      new <- compose(world_of(p), local, absolute_of(e))
   ENDIF
   IF new <> world_of(e) THEN
      CALL set_world(e, new)
      c <- first_child_of(e)
      WHILE c <> 0
         CALL place(c)
         c <- next_sibling_of(c)
      ENDWHILE
   ENDIF
ENDPROCEDURE
```

- `matrix_of` gives `position` as the origin, and the rotation times the scale as the basis.
- `compose` gives the origin of the parent plus its basis times the local origin, in `double`. The basis is the basis of the parent times the local basis.
- For each bit of `absolute`, `compose` takes that part from the local place. It splits the basis of the parent as *The split of a basis* tells.
- A parent that is alive and has no `ideatek:WorldTransform` counts as no parent.
- An entity that is its own parent, or that is in a cycle of `ideatek:Parent`, gets no `ideatek:WorldTransform`. The space module writes 1 line to the log.

## The split of a basis

The space module splits a basis into a rotation and a scale in 1 way only:

1. The scale on each axis is the length of the column of that axis.
2. The rotation is the 3 columns made orthonormal by Gram-Schmidt in the order X, Y, Z.
3. If the determinant of the basis is less than 0, the scale of Z is negative.
4. A column of length 0 has a scale of 0. Its column in the rotation is the cross product of the 2 other columns.

A blend of 2 places is a linear blend of the origins, a spherical blend of the rotations and a linear blend of the scales.
A shear does not go through the split: the split gives the nearest place without a shear.

## ideatek:Children

`ideatek:Children` tells the children of an entity. The space module attaches it to each entity that has children, and detaches it when the last child goes.
Its C type is `ideatek_children_t`: 24 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `first` | `ideatek_entity_t` | 0 | The first child. |
| `last` | `ideatek_entity_t` | 8 | The last child. |
| `count` | `uint32_t` | 16 | The number of children. |

The bytes 20 to 23 hold no field, because the size is a multiple of 8.
The children are in the order of `order`. Children with the same `order` are in the order of their ids.

Each module declares `ideatek:Children` with `BRIDGE_ON_DESTROY_KEEP`. So when a parent is destroyed, its husk keeps `ideatek:Children`, as the document of `bridge_abi.h` of Bridge tells of a husk.
An observer of the space module hears the husk leave. It walks the children of the husk by `ideatek:Sibling`, and applies `orphan` of each child.
Then the space module detaches `ideatek:Children`, and the husk goes at that barrier. A child that the space module destroys is a parent that goes in its turn, so a chain goes 1 level at each barrier.

## ideatek:Sibling

`ideatek:Sibling` links a child to the children next to it. The space module attaches it to each entity that has a parent.
Its C type `ideatek_sibling_t` has 24 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `prev` | `ideatek_entity_t` | 0 | The child before, or 0 for the first child. |
| `next` | `ideatek_entity_t` | 8 | The child after, or 0 for the last child. |
| `parent` | `ideatek_entity_t` | 16 | The parent whose list holds the child. |

When `ideatek:Parent` of a child changes, the space module reads `parent` to take the child out of the list of its old parent. Bridge gives an observer no old value.

## ideatek:Interpolated

`ideatek:Interpolated` is a tag. A mover attaches it to an entity that a reader draws between 2 steps.

## ideatek:PreviousWorldTransform

`ideatek:PreviousWorldTransform` is the place in the world at the step before. The space module attaches it only to an entity that has `ideatek:Interpolated`.
Its C type `ideatek_previous_world_transform_t` has 72 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `world` | `ideatek_world_transform_t` | 0 | The place in the world at the step before. |
| `tick` | `uint64_t` | 64 | The value of `tick` of `ideatek:Tick` at that step. |

When `tick` of `ideatek:Tick` changes, the space module copies the old `ideatek:WorldTransform` into `world`.
A reader blends `world` and `ideatek:WorldTransform` with `fraction` of `ideatek:Tick`, as *The split of a basis* tells.

## ideatek:Teleported

`ideatek:Teleported` is a tag. A mover attaches it with the new `ideatek:Transform` of an entity that jumps.
The space module then sets `world` of `ideatek:PreviousWorldTransform` to the new place, for the entity and for each descendant that has `ideatek:Interpolated`.
The space module detaches the tag at the next barrier.

## The attachments

A part of a model that other entities follow, for example a hand, is an entity with a parent. The animation module writes its `ideatek:Transform` at each step.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `ideatek_vec3_t` | `ideatek_vec3_t_desc` | `IDEATEK_VEC3_FIELDS` |
| `ideatek:ideatek_quat_t` | `ideatek_quat_t` | `ideatek_quat_t_desc` | `IDEATEK_QUAT_FIELDS` |
| `ideatek:ideatek_position_t` | `ideatek_position_t` | `ideatek_position_t_desc` | `IDEATEK_POSITION_FIELDS` |
| `ideatek:ideatek_keep_t` | `ideatek_keep_t` | `ideatek_keep_t_desc` | `IDEATEK_KEEP_VALUES` |
| `ideatek:ideatek_orphan_t` | `ideatek_orphan_t` | `ideatek_orphan_t_desc` | `IDEATEK_ORPHAN_VALUES` |
| `ideatek:ideatek_world_transform_t` | `ideatek_world_transform_t` | `ideatek_world_transform_t_desc` | `IDEATEK_WORLD_TRANSFORM_FIELDS` |
| `ideatek:Transform` | `ideatek_transform_t` | `ideatek_transform_t_desc` | `IDEATEK_TRANSFORM_FIELDS` |
| `ideatek:Parent` | `ideatek_parent_t` | `ideatek_parent_t_desc` | `IDEATEK_PARENT_FIELDS` |
| `ideatek:WorldTransform` | `ideatek_world_transform_t` | `ideatek_world_transform_t_desc` | `IDEATEK_WORLD_TRANSFORM_FIELDS` |
| `ideatek:Children` | `ideatek_children_t` | `ideatek_children_t_desc` | `IDEATEK_CHILDREN_FIELDS` |
| `ideatek:Sibling` | `ideatek_sibling_t` | `ideatek_sibling_t_desc` | `IDEATEK_SIBLING_FIELDS` |
| `ideatek:PreviousWorldTransform` | `ideatek_previous_world_transform_t` | `ideatek_previous_world_transform_t_desc` | `IDEATEK_PREVIOUS_WORLD_TRANSFORM_FIELDS` |

`IDEATEK_ABSOLUTE_POSITION`, `IDEATEK_ABSOLUTE_ROTATION` and `IDEATEK_ABSOLUTE_SCALE` are the bits of `absolute`.

```text
{ float x; float y; float z; }
{ float x; float y; float z; float w; }
{ double x; double y; double z; }
{ uint64_t id; }
{ IDEATEK_KEEP_LOCAL = 0, IDEATEK_KEEP_WORLD = 1 }
{ IDEATEK_ORPHAN_DESTROY = 0, IDEATEK_ORPHAN_KEEP_WORLD = 1 }
{ ideatek_position_t position; ideatek_quat_t rotation; ideatek_vec3_t scale; }
{ ideatek_entity_t entity; int32_t order; ideatek_keep_t keep; ideatek_orphan_t orphan; uint8_t absolute; }
{ ideatek_position_t origin; float basis[9]; }
{ ideatek_entity_t first; ideatek_entity_t last; uint32_t count; }
{ ideatek_entity_t prev; ideatek_entity_t next; ideatek_entity_t parent; }
{ ideatek_world_transform_t world; uint64_t tick; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quat_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_keep_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_orphan_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_world_transform_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Transform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Parent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WorldTransform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Children` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_KEEP` |
| `ideatek:Sibling` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PreviousWorldTransform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Interpolated` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Teleported` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares a type before the struct that names it. Bridge finds the type of a field only among the types that are already declared.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| A mover | `ideatek:Transform`, `ideatek:Parent`, `ideatek:Interpolated` and `ideatek:Teleported` | The 7 types of the fields |
| The space module | `ideatek:Transform`, `ideatek:Parent`, `ideatek:WorldTransform`, `ideatek:Children`, `ideatek:Sibling`, `ideatek:PreviousWorldTransform` and `ideatek:Teleported` | `ideatek:Interpolated`, `ideatek:Tick` and the 7 types of the fields |
| A reader | No type. | `ideatek:WorldTransform`, `ideatek:PreviousWorldTransform`, `ideatek:Children`, `ideatek:Sibling`, `ideatek:Interpolated` and the 7 types of the fields |
