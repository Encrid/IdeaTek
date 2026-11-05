# include/engine_subsystems/ideatek_debug_draw.h

`include/engine_subsystems/ideatek_debug_draw.h` gives the types of the drawings for a debug: lines, shapes and text in the world or on the screen.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields hold only bytes and masks, so the header includes no other part.

## The roles

- Each module that draws keeps 1 entity. In each round in which it draws, it attaches `ideatek:DebugDraw` again.
- The game writes `ideatek:DebugView` on 1 entity. It turns the groups of the drawings on and off.
- The draw module, or the render module, draws each batch whose groups the view has.
- A profiler is a matter of the host or of a library, not of this part.

## The batch

`data` of `ideatek:DebugDraw` is a run of records, with no bytes between them. The fields are not aligned: a reader copies each field with `memcpy`. All numbers are little-endian.

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `kind` | `ideatek_debug_kind_t` | 0 | The shape. |
| `flags` | `ideatek_debug_flags_t` | 1 | The options. |
| `color` | `uint8_t[4]` | 2 | Red, green, blue and alpha, from 0 to 255. |
| `a` | `double[3]` | 6 | The first point: the start, the center or the tip. |
| `b` | `double[3]` | 30 | The second point: the end, the half extents, the normal or the scale. |
| `c` | `double[3]` | 54 | The third corner of a triangle. |
| `rotation` | `float[4]` | 78 | A quaternion: x, y, z and w. |
| `size` | `float` | 94 | A radius, the size of a point, the width of a line, or the size of the text. |
| `lifetime` | `float` | 98 | The seconds that the shape stays. 0 means 1 frame. |
| `entity` | `uint64_t` | 102 | The mesh or the physics shape of `IDEATEK_DEBUG_KIND_MESH`, else 0. |
| `owner` | `uint64_t` | 110 | The entity that the drawing is about. 0 means none. |
| `length` | `uint32_t` | 118 | The bytes of the text of `IDEATEK_DEBUG_KIND_TEXT`, else 0. |
| The text | `uint8_t[length]` | 122 | The text, in UTF-8. |

A point with `IDEATEK_DEBUG_FLAGS_SCREEN` is in pixels of the screen, from the top left. Each other point is in the world, in meters, as `ideatek_position_t` of the space part.
The `size` of a line or an arrow is its width in pixels, and 0 means 1 pixel. A wire shape keeps a width of 1 pixel, because its `size` is a radius.
A mesh draws the mesh resource or the physics shape `entity` at `a`, with `rotation`, and with the scale `b`. So the shapes of a world need no triangles in each round.

With `IDEATEK_DEBUG_FLAGS_SOLID`, a shape is filled. With `IDEATEK_DEBUG_FLAGS_CAST_SHADOW`, a solid shape also casts a shadow.

## The groups

The draw module draws a batch only when `groups` of the batch and `groups` of the view share a bit.
A module reads `ideatek:DebugView` first, and skips the build of a batch whose groups are off. So a group that is off costs nothing.
The view has no group by default, so each drawing is off.
A viewer can show only the drawings of 1 `owner`, for example 1 enemy.

A recorder of the changes part also records the `ideatek:DebugDraw` batches with their round. So a replay shows them on its timeline, as the Visual Logger of Unreal does.

## ideatek:ideatek_debug_kind_t

`ideatek_debug_kind_t` tells the shape of a record.
`ideatek_debug_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DEBUG_KIND_LINE` | A line from `a` to `b`. |
| 1 | `IDEATEK_DEBUG_KIND_POINT` | A point at `a`. |
| 2 | `IDEATEK_DEBUG_KIND_BOX` | A box at `a`, with half extents `b`. |
| 3 | `IDEATEK_DEBUG_KIND_SPHERE` | A sphere at `a`, of radius `size`. |
| 4 | `IDEATEK_DEBUG_KIND_CAPSULE` | A capsule from `a` to `b`, of radius `size`. |
| 5 | `IDEATEK_DEBUG_KIND_ARROW` | An arrow from `a` to `b`. |
| 6 | `IDEATEK_DEBUG_KIND_AXES` | The 3 axes of a frame at `a`. |
| 7 | `IDEATEK_DEBUG_KIND_TEXT` | A text at `a`. |
| 8 | `IDEATEK_DEBUG_KIND_TRIANGLE` | A triangle of the corners `a`, `b` and `c`. |
| 9 | `IDEATEK_DEBUG_KIND_CYLINDER` | A cylinder from `a` to `b`, of radius `size`. |
| 10 | `IDEATEK_DEBUG_KIND_CONE` | A cone with its tip at `a` and its base at `b`, of base radius `size`. |
| 11 | `IDEATEK_DEBUG_KIND_CIRCLE` | A circle at `a` with the normal `b`, of radius `size`. |
| 12 | `IDEATEK_DEBUG_KIND_MESH` | The mesh or the shape `entity`, placed by `a`, `rotation` and `b`. |

## ideatek:ideatek_debug_flags_t

`ideatek_debug_flags_t` names the options of a record.
`ideatek_debug_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_DEBUG_FLAGS_ON_TOP` | The shape shows through the world: no test of the depth. |
| 0x02 | `IDEATEK_DEBUG_FLAGS_SCREEN` | The points are on the screen. |
| 0x04 | `IDEATEK_DEBUG_FLAGS_SOLID` | The shape is filled. |
| 0x08 | `IDEATEK_DEBUG_FLAGS_CAST_SHADOW` | A solid shape casts a shadow. |

## ideatek:DebugDraw

`ideatek:DebugDraw` is a batch of drawings of 1 module for 1 round.
`ideatek_debug_draw_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | The groups of the batch. |
| `data` | `bridge_text_t` | 8 | The records: *The batch*. |

## ideatek:DebugView

`ideatek:DebugView` turns the groups of the drawings on and off.
`ideatek_debug_view_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | 0 | The groups that draw. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_debug_kind_t` | `ideatek_debug_kind_t` | `ideatek_debug_kind_t_desc` | `IDEATEK_DEBUG_KIND_VALUES` |
| `ideatek:ideatek_debug_flags_t` | `ideatek_debug_flags_t` | `ideatek_debug_flags_t_desc` | `IDEATEK_DEBUG_FLAGS_VALUES` |
| `ideatek:DebugDraw` | `ideatek_debug_draw_t` | `ideatek_debug_draw_t_desc` | `IDEATEK_DEBUG_DRAW_FIELDS` |
| `ideatek:DebugView` | `ideatek_debug_view_t` | `ideatek_debug_view_t_desc` | `IDEATEK_DEBUG_VIEW_FIELDS` |

```text
{ IDEATEK_DEBUG_KIND_LINE = 0, IDEATEK_DEBUG_KIND_POINT = 1, IDEATEK_DEBUG_KIND_BOX = 2, IDEATEK_DEBUG_KIND_SPHERE = 3, IDEATEK_DEBUG_KIND_CAPSULE = 4, IDEATEK_DEBUG_KIND_ARROW = 5, IDEATEK_DEBUG_KIND_AXES = 6, IDEATEK_DEBUG_KIND_TEXT = 7, IDEATEK_DEBUG_KIND_TRIANGLE = 8, IDEATEK_DEBUG_KIND_CYLINDER = 9, IDEATEK_DEBUG_KIND_CONE = 10, IDEATEK_DEBUG_KIND_CIRCLE = 11, IDEATEK_DEBUG_KIND_MESH = 12 }
{ IDEATEK_DEBUG_FLAGS_ON_TOP = 0x01, IDEATEK_DEBUG_FLAGS_SCREEN = 0x02, IDEATEK_DEBUG_FLAGS_SOLID = 0x04, IDEATEK_DEBUG_FLAGS_CAST_SHADOW = 0x08 }
{ uint64_t groups; bridge_text_t data; }
{ uint64_t groups; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_debug_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_debug_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DebugDraw` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DebugView` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| A module that draws | `ideatek:DebugDraw` | `ideatek:DebugView`, and the types of the fields |
| The game | `ideatek:DebugView` | The types of the fields |
| The draw module | No type. | `ideatek:DebugDraw`, `ideatek:DebugView`, and the types of the fields |
