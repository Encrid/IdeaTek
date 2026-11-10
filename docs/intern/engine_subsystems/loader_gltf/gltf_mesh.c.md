# src/engine_subsystems/loader_gltf/gltf_mesh.c

This file writes 1 mesh: its tables, its vertices and its indices.

The first pass says the lines of the faults of the mesh. A fault is a compressed primitive, a primitive with no positions, or a primitive with another topology.
The streams above `MAX_STREAMS` are a fault too.

## Functions

| Function | Result |
| --- | --- |
| `stride_of` | The bytes of 1 value of a stream. |
| `indices_of` | The indices of 1 primitive after a fan or a loop is turned into a list. |
| `say_faults` | Says the lines of the faults of a mesh. |
| `write_tables` | Writes the records of the streams and of the sub-meshes. |
| `write_mesh` | Plans and writes 1 mesh. |
