# src/engine_subsystems/loader_ktx2/ktx2_inside.h

This header holds the settings, the slots, the records and the state of the module, and the prototypes that its sources share.

## The slots

The slot of a type is its place in the order of the declarations of `docs/intern/engine_subsystems/loader_ktx2/ktx2_declare.c.md`.
| Slot | Value |
| --- | --- |
| `ENTITY` | 0 |
| `RESOURCE` | 6 |
| `LOADER` | 7 |
| `BYTES` | 8 |
| `OUTCOME` | 9 |
| `TEXTURE` | 10 |
| `SAMPLER` | 11 |
| `SETTINGS` | 12 |

## The records

`ktx2_texture_t` is 1 texture of the module: its entity, its revision, and 2 keys of blocks. `now` names the block of its texels, and `old` names the block before a reload.
A key of 0 names no block. The table holds `max_textures` records, and a record with an entity of 0 is free.

## The state

`ktx2_module` holds the state between 2 calls of Bridge, which give no argument for it. It is the 1 global variable of the module, and `.toolchain/project.toml` excepts it from G4.

| Field | Value |
| --- | --- |
| `lists` | The indexes of the lists of the 3 observers. |
| `loader` | The entity of the loader. |
| `started`, `broken` | The first load read the settings. A wrong setting breaks the module. |
| `s` | The 2 settings. |
| `textures` | The table of the textures. |
| `blocks` | The block list of the `texture_bytes` arena: `docs/intern/kernel/resources_files/resources_arena.c.md`. |
| `arena_at` | Where the arena starts in the shared memory. |
| `serial` | The key of the last block. |

The block list is the code of the resources module. The artifact of the module builds `src/kernel/resources_files/resources_arena.c` too, so 1 file holds the blocks of both modules.
