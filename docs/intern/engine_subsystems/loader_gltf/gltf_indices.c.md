# src/engine_subsystems/loader_gltf/gltf_indices.c

This file writes the indices of a mesh.

The indices of each primitive are moved by the vertices of the primitives before it. A primitive with no indices gets 0 to its vertex count minus 1.
A fan gives the triangles 0, i, i plus 1. A loop gives each line, and a line from the last vertex to the first.

## Functions

| Function | Result |
| --- | --- |
| `put` | Writes 1 index of 16 or 32 bits. |
| `source_of` | The indices of a primitive, or its vertex order. |
| `emit` | Writes the indices of a primitive in its topology. |
| `write_indices` | Writes the indices of each usable primitive. |
