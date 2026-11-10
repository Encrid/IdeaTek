# src/engine_subsystems/loader_gltf/gltf_parts.c

This file gives each part of a file its label entity.

A part keeps its entity across reloads: its key is the hash of its label. A new label spawns a new entity.
The default material is a part only when a primitive has no material.

## Functions

| Function | Result |
| --- | --- |
| `count_of` | The number of parts of a kind. |
| `get_part` | The part of a label, or a new one, or nothing when the table is full. |
| `name_parts` | Takes the part of each label, and of the default material when it is needed. |
| `plan_parts` | Takes the arrays and the parts, or writes the line of the fault. |
