# src/engine_subsystems/loader_gltf/gltf_arrays.c

This file takes the arrays of a build from the parse arena in 1 allocation.

The arrays hold the spans, the labels and the parts of each kind, and the mesh plans.
They also hold the clip data, the model counts, the ranks and the walk of the nodes.
The temporary buffer and the joint map are as large as the largest accessor or skin, so no step of a build allocates more.

## Functions

| Function | Result |
| --- | --- |
| `carve` | Cuts an array at a multiple of 16 from the allocation. |
| `most_of` | The count of the largest accessor or skin. |
| `bytes_of` | The bytes of all the arrays. |
| `carve_kinds` | Cuts the arrays of each kind. |
| `take_arrays` | Takes and cuts all the arrays. `false` when they do not fit. |
