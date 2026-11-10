# src/engine_subsystems/loader_gltf/gltf_model.c

This file writes 1 model from the roots of a scene.

The walk visits each root, then its children, so a parent comes before its children. Then the tables are counted, laid out and filled: `docs/extern/engine_subsystems/ideatek_scene.h.md`, *The bytes of a model*.

## Functions

| Function | Result |
| --- | --- |
| `visit` | Adds a node and its children to the walk. |
| `size_of` | The bytes of a model. |
| `point_tables` | Points the tables into the bytes, and resets the counts. |
| `model_of` | The fields of the model. |
| `write_model` | Writes 1 model. |
