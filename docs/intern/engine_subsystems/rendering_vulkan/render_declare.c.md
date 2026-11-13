# src/engine_subsystems/rendering_vulkan/render_declare.c

This file declares the types that the module reads and writes, and finds a value of an entity.

The module declares, in this order:

1. The 12 types that its fields use, from the rendering, space and GPU parts.
2. The types that it reads: the camera, its projection, the capture request and the place.
   It also reads the 4 types of the GPU part, and `ideatek:Frame` as a singleton.
3. The types that it writes: `ideatek:Capture`, and `ideatek:RenderStats` as a singleton.
4. Its settings, `ideatek_rendering_vulkan:Settings`, as a singleton.

`declare_types` keeps the slot of each type by its name of `render_inside.h`, so the order of the declarations sets no slot.
`render_find` gives a value of an entity as a pointer into the shared memory, or `nullptr` when the entity has no such value.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the types of the fields. |
| `declare_reads` | Declares the types that the module reads, and keeps their slots. |
| `declare_types` | Declares the types of the module and keeps their slots. |
| `render_find` | The value of 1 type of 1 entity, or `nullptr`. |
