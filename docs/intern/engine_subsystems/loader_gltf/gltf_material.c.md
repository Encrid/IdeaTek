# src/engine_subsystems/loader_gltf/gltf_material.c

This file gives the slots and the base fields of a material.

A slot names the resource of its image, and the transform of `KHR_texture_transform`. A transform with no `texCoord` keeps the `texCoord` of its texture.
`GLTF_DEFAULT` is the default material of glTF 2.0.

## Functions

| Function | Result |
| --- | --- |
| `slot_of` | The slot of a texture view. |
| `give_slots` | Writes the 5 slots of a material. |
| `base_of` | The fields of `ideatek:Material`. |
| `give_material` | Attaches the material and its extensions. |
