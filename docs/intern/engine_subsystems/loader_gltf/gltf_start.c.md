# src/engine_subsystems/loader_gltf/gltf_start.c

This file reads the 4 settings of the glTF loader, and takes its tables and its 2 arenas at the first load.

Each setting has its range. A value outside it writes 1 line and breaks the module. `parse_bytes` is rounded down to a multiple of 16, so the aligned cursor of the parse never passes its end.
A file holds at most 3 blocks: its output, its output before a reload, and the texts of its loads. A copy of a buffer holds 1 block. So the block list has 4 blocks for each file.

## Functions

| Function | Result |
| --- | --- |
| `check_counts` | Checks the 4 settings, and writes the line of each value outside its range. |
| `take` | Bytes of the persistent arena, cleared. |
| `take_tables` | Takes the tables, the block list and the 2 arenas. |
| `start_module` | Starts the module 1 time. `false` for a broken module. |
