# src/engine_subsystems/loader_gltf/gltf_plan.c

This file plans the streams and the layout of 1 mesh.

The mesh takes the topology of its first primitive that is not compressed. Each attribute of a usable primitive adds its stream, if no stream has its semantic, its set and its target.
A morph target adds its positions, normals and tangents. A mesh keeps at most `MAX_STREAMS` streams, and the plan marks the rest.
The streams follow the tables of the mesh, one after the other, then the indices.

## Functions

| Function | Result |
| --- | --- |
| `add_unique` | Adds a stream that the plan does not have. |
| `add_morph` | Adds the stream of 1 attribute of a morph target. |
| `add_streams` | Adds the streams of 1 primitive. |
| `lay_out` | Gives each stream its offset, and the size of the mesh. |
| `plan_mesh` | Plans 1 mesh. |
