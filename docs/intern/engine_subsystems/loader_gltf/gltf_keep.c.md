# src/engine_subsystems/loader_gltf/gltf_keep.c

This file copies the bytes of a buffer file into the output arena, in the round of its `ideatek:RawBytes`.

The copy takes its block before it frees the old copy, so a failed copy keeps the old bytes. A copy that has no record or no room refuses each file that needs it.

## Functions

| Function | Result |
| --- | --- |
| `store` | Copies the bytes into a new block, and frees the old one. |
| `keep_bytes` | Copies the bytes of a used buffer, then wakes or refuses its files. |
