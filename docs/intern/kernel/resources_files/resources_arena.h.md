# src/kernel/resources_files/resources_arena.h

This header gives the block list of the `load_bytes` arena.

## The types

`res_block_t` is 1 block of bytes that a resource holds. It tells where the block starts in the arena, how many bytes it has, and its resource.
`res_blocks_t` is the list: where the blocks are, how many the list can hold, how many are used, and the size of the arena. The blocks of the list are sorted by their start.
