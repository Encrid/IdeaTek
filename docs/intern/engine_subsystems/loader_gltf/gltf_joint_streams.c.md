# src/engine_subsystems/loader_gltf/gltf_joint_streams.c

This file writes the joints of a mesh as the ranks of the model.

The skin of a mesh is the skin of the first node that draws it. A joint index that the skin does not hold becomes 0. A mesh with no skin keeps its joint indices.

## Functions

| Function | Result |
| --- | --- |
| `skin_of` | The skin of a mesh, or nothing. |
| `remap_of` | The rank of each joint of the skin of a mesh. |
| `write_joints` | Writes the joints of 1 accessor as ranks of 16 bits. |
