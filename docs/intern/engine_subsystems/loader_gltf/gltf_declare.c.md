# src/engine_subsystems/loader_gltf/gltf_declare.c

This file declares the types of the glTF loader, in the order of its slots.

1. The types of the fields: the entity, the 2 enums of a load, and the colors.
   Then the 2 enums of a mesh, the texture slot and the alpha mode.
2. The types of the resources part that the module reads or writes, and the 2 tags `ideatek:ResourceFailed` and `ideatek:ResourceReload`.
3. `ideatek:Model`, `ideatek:Mesh`, the material and its 6 extensions, `ideatek:SkinBinds` and `ideatek:Clip`.
4. The singleton `ideatek_loader_gltf:Settings`.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the types of the fields. |
| `declare_loads` | Declares the types of the resources part. |
| `declare_types` | Declares the fields and the types of the resources part. |
| `declare_materials` | Declares the material, its extensions and the tag of an unlit material. |
| `declare_outputs` | Declares the types that the loader writes, and the settings. |
