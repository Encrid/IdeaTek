# src/engine_subsystems/loader_gltf/gltf_skin.c

This file writes the binds of 1 skin in the ranks of the model.

Each rank first gets the identity. Then each joint of the skin gets its inverse bind matrix, if the skin has the matrices.

## Functions

| Function | Result |
| --- | --- |
| `fill_binds` | Writes the matrices of the joints of a skin. |
| `write_skin` | Writes the binds of 1 skin. |
