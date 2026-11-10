# src/engine_subsystems/loader_gltf/gltf_table.c

This file finds and forgets the files, and takes and frees the blocks of the output arena.

`take_block` gives each block a new key, and skips the key 0. `forget_file` frees the 3 blocks of a file, and its parts and its dependencies.

## Functions

| Function | Result |
| --- | --- |
| `find_file` | The record of a file, or nothing. |
| `add_file` | A free record for a file, or nothing. |
| `free_key` | Gives the block of a key back, and clears the key. |
| `take_block` | Takes a block of a size with a new key. `false` when no gap holds it. |
| `forget_file` | Frees the blocks, the parts and the dependencies of a file. |
