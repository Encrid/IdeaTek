# src/engine_subsystems/loader_ktx2/ktx2_start.c

This file reads the settings, and takes the table, the block list and the arena.

## The start

The settings come only after `init`, so the first load starts the module.
`start_module` checks `texture_bytes` against 4096 to 1073741824, and `max_textures` against 1 to 65536. Each value outside its range writes its line.
A wrong setting breaks the module: each load then gets its line and a failed outcome.

Else the module takes `max_textures` records, 2 blocks for each record, and `texture_bytes` bytes, in its persistent arena. A texture holds at most 2 blocks: its texels, and the texels before a reload.

## Functions

| Function | Result |
| --- | --- |
| `check_count` | `true` when a setting is in its range, else writes the line. |
| `take` | Bytes of the persistent arena, set to 0. |
| `start_module` | Reads and checks the settings, and takes the memory. `false` for a wrong setting. |
