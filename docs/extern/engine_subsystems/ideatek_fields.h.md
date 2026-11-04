# include/engine_subsystems/ideatek_fields.h

`include/engine_subsystems/ideatek_fields.h` gives the types of the fields of force: the wind and the force fields.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the space part for `ideatek_vec3_t`.

## The roles

- The game or a level is the author. It writes `ideatek:Wind` and `ideatek:ForceField` on entities with a place.
- A reader reads them with `ideatek:WorldTransform`. The effects module reads them for each particle. The physics module reads them for each soft body and each rope.

## The meaning of a field

A wind is a speed of the air. A reader drags its things toward the velocity of the wind, by its own drag.
A force field is an acceleration. A reader adds it to the motion of its things.
The direction of a wind is -Z of its `ideatek:WorldTransform`, as the direction of a light. A vortex turns about the Y axis of its entity.

## The shape and the falloff

A field acts inside its shape: the whole world, a sphere of radius `size.x`, or a box of half extents `size`.
`falloff` shapes the strength by the distance `d` to the center. The strength is `(1 - d / size) ^ falloff`, so a falloff of 0 keeps it constant. A field that fills the world has no falloff.
The fields add where they overlap.

## The defaults

The defaults are neutral: a speed and an acceleration of 0, a shape that fills the world, and no falloff. The references do not agree on a size, so the game sets it.

## ideatek:ideatek_field_shape_t

`ideatek_field_shape_t` tells the place where a field acts.
`ideatek_field_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FIELD_SHAPE_INFINITE` | The field fills the world. |
| 1 | `IDEATEK_FIELD_SHAPE_SPHERE` | A sphere of radius `size.x`. |
| 2 | `IDEATEK_FIELD_SHAPE_BOX` | A box of half extents `size`. |

## ideatek:ideatek_force_kind_t

`ideatek_force_kind_t` tells the kind of a force field.
`ideatek_force_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FORCE_KIND_ATTRACTOR` | A pull to the center. A negative acceleration pushes away. |
| 1 | `IDEATEK_FORCE_KIND_VORTEX` | A turn about the Y axis of the entity. |

## ideatek:Wind

`ideatek:Wind` is a wind along -Z of its entity.
`ideatek_wind_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `speed` | `float` | 12 | 0 | The speed of the air, in m / s. |
| `falloff` | `float` | 16 | 0 | The exponent of the falloff. |
| `shape` | `ideatek_field_shape_t` | 20 | `IDEATEK_FIELD_SHAPE_INFINITE` | The place where it blows. |

Bytes 21 to 23 hold no field.

## ideatek:ForceField

`ideatek:ForceField` pulls or turns the things in its shape.
`ideatek_force_field_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `acceleration` | `float` | 12 | 0 | The strength, in m / s^2. |
| `falloff` | `float` | 16 | 0 | The exponent of the falloff. |
| `kind` | `ideatek_force_kind_t` | 20 | No default | The kind. |
| `shape` | `ideatek_field_shape_t` | 21 | `IDEATEK_FIELD_SHAPE_INFINITE` | The place where it acts. |

Bytes 22 to 23 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_field_shape_t` | `ideatek_field_shape_t` | `ideatek_field_shape_t_desc` | `IDEATEK_FIELD_SHAPE_VALUES` |
| `ideatek:ideatek_force_kind_t` | `ideatek_force_kind_t` | `ideatek_force_kind_t_desc` | `IDEATEK_FORCE_KIND_VALUES` |
| `ideatek:Wind` | `ideatek_wind_t` | `ideatek_wind_t_desc` | `IDEATEK_WIND_FIELDS` |
| `ideatek:ForceField` | `ideatek_force_field_t` | `ideatek_force_field_t_desc` | `IDEATEK_FORCE_FIELD_FIELDS` |

```text
{ IDEATEK_FIELD_SHAPE_INFINITE = 0, IDEATEK_FIELD_SHAPE_SPHERE = 1, IDEATEK_FIELD_SHAPE_BOX = 2 }
{ IDEATEK_FORCE_KIND_ATTRACTOR = 0, IDEATEK_FORCE_KIND_VORTEX = 1 }
{ ideatek_vec3_t size; float speed; float falloff; ideatek_field_shape_t shape; }
{ ideatek_vec3_t size; float acceleration; float falloff; ideatek_force_kind_t kind; ideatek_field_shape_t shape; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_field_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_force_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wind` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ForceField` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| An author | `ideatek:Wind`, `ideatek:ForceField` | The types of the fields |
| A reader | No type. | `ideatek:Wind`, `ideatek:ForceField`, `ideatek:WorldTransform`, and the types of the fields |
