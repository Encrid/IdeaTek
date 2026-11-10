# src/engine_subsystems/loader_gltf/gltf_material_list.c

This file writes the materials of the nodes, and attaches each material with its label.

## Functions

| Function | Result |
| --- | --- |
| `fill_materials` | Writes the material of each sub-mesh of a node. |
| `give_materials` | Attaches the label and the material of each material, and of the default material. |
| `has_bare` | `true` when a primitive of a mesh has no material. |
| `needs_default` | `true` when a primitive of the file has no material. |
