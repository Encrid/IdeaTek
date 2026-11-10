# src/engine_subsystems/loader_gltf/gltf_node_kit.c

This file gives the weights of a node, the parts of a matrix, a light and a projection.

A matrix gives its translation, the lengths of its columns as the scale, and the rotation of the columns divided by the scale. A negative determinant makes the first scale negative.
The quaternion comes from the diagonal and the signs of the rotation, with no chain of branches. A column of length 0 gives a rotation of 0.

## Functions

| Function | Result |
| --- | --- |
| `weight_of` | A weight of the node, or of its mesh, or 0. |
| `column_length` | The length of a column of a matrix. |
| `split_matrix` | Writes the place of a matrix. |
| `fill_light` | Writes a light of `KHR_lights_punctual`. |
| `fill_camera` | Writes the projection of a camera. |
