# src/engine_subsystems/loader_gltf/gltf_vertices.c

This file writes the vertices of each stream of a mesh.

Each accessor is unpacked into floats with `cgltf_accessor_unpack_floats`, also a sparse or a normalized accessor. A color with 3 values gets 1 as its fourth value.
A stream that a primitive lacks gets the default of `docs/usage/engine_subsystems/loader_gltf.md`, *The meshes*.

## Functions

| Function | Result |
| --- | --- |
| `accessor_of` | The accessor of a stream in a primitive or its morph target, or nothing. |
| `put_defaults` | Writes the default values of a stream. |
| `expand` | Writes a color of 3 values with an alpha of 1. |
| `fill_stream` | Writes the values of 1 stream of 1 primitive. |
| `write_vertices` | Writes each stream of each usable primitive. |
