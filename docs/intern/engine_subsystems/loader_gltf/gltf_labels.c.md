# src/engine_subsystems/loader_gltf/gltf_labels.c

This file gives the label of each part: its kind, `/`, and its name.

The rule of a label is `docs/usage/engine_subsystems/loader_gltf.md`, *The labels*.

## Functions

| Function | Result |
| --- | --- |
| `name_at` | The name of a part, or nothing. |
| `is_shared` | `true` when another part of the kind has the name. |
| `label_of` | Writes the label of a part, and gives its length. |
