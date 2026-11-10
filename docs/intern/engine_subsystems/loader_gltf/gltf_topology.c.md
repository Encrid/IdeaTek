# src/engine_subsystems/loader_gltf/gltf_topology.c

This file maps the modes and the attributes of glTF to the topologies and the streams of the rendering part.

A fan maps to `IDEATEK_TOPOLOGY_TRIANGLE_LIST`, and a loop maps to `IDEATEK_TOPOLOGY_LINE_LIST`. cgltf refuses a mode that glTF does not know.
A primitive is compressed when it has `KHR_draco_mesh_compression`, or when the view of its positions has `EXT_meshopt_compression`.

## Functions

| Function | Result |
| --- | --- |
| `topology_of` | The topology of a mode. |
| `position_of` | The accessor of the positions, or nothing. |
| `is_packed` | `true` for a compressed primitive. |
| `is_usable` | `true` for a primitive with positions, no compression and the topology of the mesh. |
| `stream_of` | The stream of an attribute. `false` for an unknown attribute. |
