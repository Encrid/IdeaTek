# src/engine_subsystems/loader_gltf/gltf_node_kit.c

This file gives the weights of a node, the parts of a matrix, a light and a projection.

A matrix gives its translation, and `mat3f_split` of the math part gives the rotation and the scale of its 3 by 3 part.
The rule is that of `docs/usage/foundation/math.md`, *The split of a basis*. So a mirrored node has its negative scale on Z.

## Functions

| Function | Result |
| --- | --- |
| `weight_of` | A weight of the node, or of its mesh, or 0. |
| `split_matrix` | Writes the place of a matrix. |
| `fill_light` | Writes a light of `KHR_lights_punctual`. |
| `fill_camera` | Writes the projection of a camera. |
