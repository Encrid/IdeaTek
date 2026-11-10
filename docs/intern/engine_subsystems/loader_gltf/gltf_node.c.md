# src/engine_subsystems/loader_gltf/gltf_node.c

This file counts and fills the records of the nodes of a model.

## Functions

| Function | Result |
| --- | --- |
| `targets_of` | The morph targets of the mesh of a node. |
| `count_tables` | Counts the lights, the materials, the weights, the cameras and the names. |
| `place_node` | Writes the place of a node, its mesh and its binds. |
| `fill_extras` | Writes the weights, the light and the camera of a node. |
| `fill_node` | Writes 1 node record and its name. |
