# src/kernel/resources_files/resources_arena.c

This file gives and takes back the blocks of the `load_bytes` arena. It is pure: the program `resources_arena` of the tests calls it on a list of its own.

## The blocks

The arena is `load_bytes` bytes. Each resource holds at most 1 block: the bytes of its `ideatek:ResourceBytes`.
`blocks_take` walks the sorted list from the start of the arena. It takes the first gap that holds the size: before the first block, between 2 blocks, or after the last block. It inserts the new block in the list, and gives its start.

It gives `false` when no gap holds the size, or when the list is full.
`blocks_free` removes the block of a resource, and closes the list over it. The gap joins its neighbors by itself, since the list holds only the used blocks.
A size of 0 takes a block of 0 bytes, so an empty file has a block too.

```pseudo
FUNCTION blocks_take(list : U64, size : U32, resource : U32, start : U64) RETURNS BOOLEAN
   at <- 0
   FOR i <- 0 TO list.used - 1
      IF list.block[i].start - at >= size THEN
         CALL insert(list, i, at, size, resource)
         RETURN TRUE
      ENDIF
      at <- list.block[i].start + list.block[i].size
   NEXT i
   IF list.size - at >= size THEN
      CALL insert(list, list.used, at, size, resource)
      RETURN TRUE
   ENDIF
   RETURN FALSE
ENDFUNCTION
```

## Functions

| Function | Result |
| --- | --- |
| `insert` | Puts a block at an index of the list, and moves the next blocks up. |
| `blocks_take` | Takes a block of a size for a resource. `false` when no gap holds it. |
| `blocks_free` | Gives the block of a resource back. |
| `blocks_room` | The size of the largest gap: the test of a read that waits. |
