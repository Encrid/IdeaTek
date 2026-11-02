# include/space/ideatek_space.h

`include/space/ideatek_space.h` gives the types of the place of an entity in the world: their C types and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.

## The space

- The unit of length is 1 meter.
- The axes are right-handed. +Y is up. The forward direction of a camera is -Z.
- The front of a model faces +Z.
- A position in the world has 3 `double` values. A direction, a velocity and a scale have 3 `float` values.

## The roles

- A mover writes `ideatek:Transform` and `ideatek:Parent`. Physics, animation and a character controller are movers.
- The space module reads `ideatek:Transform` and `ideatek:Parent`, and writes `ideatek:WorldTransform`.
- A reader reads `ideatek:WorldTransform`. Render, audio and the net modules are readers.

`ideatek:WorldTransform` has one writer: the space module.
In each round, the movers write before the space module reads, and the space module writes before the readers read.

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

## ideatek:ideatek_entity_t

`ideatek_entity_t` names an entity. It is a struct of 8 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The id of the entity that Bridge gives, or 0 for no entity. |

Bridge never gives the id 0 to an entity.
An id of a destroyed entity names nothing: the document of `bridge_abi.h` of Bridge tells the generation of an id.

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

## ideatek:Parent

`ideatek:Parent` names the parent of an entity. An entity with no `ideatek:Parent` is a root.
Its C type `ideatek_parent_t` has 8 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `entity` | `ideatek_entity_t` | 0 | The parent. |

## ideatek:WorldTransform

`ideatek:WorldTransform` is the place of an entity in the world. It can hold a shear, which a non-uniform scale under a rotated parent gives.
Its C type `ideatek_world_transform_t` has 64 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `origin` | `ideatek_position_t` | 0 | The position in the world. |
| `basis` | `float[9]` | 24 | The X axis, then the Y axis, then the Z axis of the entity in the world: 3 columns of 3 values. |

The bytes 60 to 63 hold no field, because the size is a multiple of 8.

The space module attaches `ideatek:WorldTransform` to each entity that has `ideatek:Transform`, and detaches it when `ideatek:Transform` goes.
At each round, it writes the value of each such entity:

```pseudo
PROCEDURE place(e : U64)
   local <- matrix_of(transform_of(e))
   p <- parent_of(e)
   IF p = 0 OR NOT has_world(p) THEN
      CALL set_world(e, local)
   ELSE
      CALL place(p)
      CALL set_world(e, compose(world_of(p), local))
   ENDIF
ENDPROCEDURE
```

- `matrix_of` gives `position` as the origin, and the rotation times the scale as the basis.
- `compose` gives the origin of the parent plus its basis times the local origin, in `double`. The basis is the basis of the parent times the local basis.
- A parent that is not alive, or that has no `ideatek:Transform`, counts as no parent.
- The space module places each parent before its children. In a cycle of `ideatek:Parent`, it writes no value for the entities of the cycle.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `ideatek_vec3_t` | `ideatek_vec3_t_desc` | `IDEATEK_VEC3_FIELDS` |
| `ideatek:ideatek_quat_t` | `ideatek_quat_t` | `ideatek_quat_t_desc` | `IDEATEK_QUAT_FIELDS` |
| `ideatek:ideatek_position_t` | `ideatek_position_t` | `ideatek_position_t_desc` | `IDEATEK_POSITION_FIELDS` |
| `ideatek:ideatek_entity_t` | `ideatek_entity_t` | `ideatek_entity_t_desc` | `IDEATEK_ENTITY_FIELDS` |
| `ideatek:Transform` | `ideatek_transform_t` | `ideatek_transform_t_desc` | `IDEATEK_TRANSFORM_FIELDS` |
| `ideatek:Parent` | `ideatek_parent_t` | `ideatek_parent_t_desc` | `IDEATEK_PARENT_FIELDS` |
| `ideatek:WorldTransform` | `ideatek_world_transform_t` | `ideatek_world_transform_t_desc` | `IDEATEK_WORLD_TRANSFORM_FIELDS` |

```text
{ float x; float y; float z; }
{ float x; float y; float z; float w; }
{ double x; double y; double z; }
{ uint64_t id; }
{ ideatek_position_t position; ideatek_quat_t rotation; ideatek_vec3_t scale; }
{ ideatek_entity_t entity; }
{ ideatek_position_t origin; float basis[9]; }
```

## The declarations


| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quat_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Transform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Parent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WorldTransform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares a type before the struct that names it. Bridge finds the type of a field only among the types that are already declared.

## The grants


| Role | `write` | `read` |
| --- | --- | --- |
| A mover | `ideatek:Transform` and `ideatek:Parent` | `ideatek:ideatek_vec3_t`, `ideatek:ideatek_quat_t`, `ideatek:ideatek_position_t` and `ideatek:ideatek_entity_t` |
| The space module | `ideatek:WorldTransform` | `ideatek:Transform`, `ideatek:Parent` and the 4 types of the fields |
| A reader | No type. | `ideatek:WorldTransform` and `ideatek:ideatek_position_t` |
