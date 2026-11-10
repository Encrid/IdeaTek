# src/engine_subsystems/loader_gltf/gltf_parse.c

This file gives cgltf its memory from the parse arena, and parses a file.

The parse arena is a cursor that only grows. `parse_file` resets it, so the memory of 1 parse is free at the next parse. cgltf frees nothing, because the reset frees all.
After a parse, `cgltf_validate` checks the indices and the counts of the data.

## Functions

| Function | Result |
| --- | --- |
| `parse_take` | Bytes of the parse arena at a multiple of 16, or nothing when they do not fit. |
| `take_for_cgltf` | The allocation of cgltf. |
| `free_for_cgltf` | The free of cgltf, which does nothing. |
| `parse_options` | The options of cgltf with the memory of the module. |
| `parse_file` | Parses and checks 1 file in a new parse arena. |
