# src/engine_subsystems/loader_ktx2/ktx2_levels.c

This file checks each level, and copies the levels into the bytes of the texture.

## The bytes of a level

For the level `k`, each side is the side of level 0 shifted right by `k`, and at least 1. A depth of 0 is 1.
The bytes are the blocks of the 3 sides, times the bytes of a block, the layers and the faces. An array with no layer counts 1 layer.
`ckd_mul` of C23 tells an overflow of 64 bits, and the module then refuses the level.

```pseudo
FUNCTION check_level(file : U64, f : U64, level : U32) RETURNS U64
   entry <- KTX2_HEAD + KTX2_LEVEL * level
   at <- ktx2_u64(file, entry)
   len <- ktx2_u64(file, entry + 8)
   IF at + len > file.len THEN
      RETURN "a level is outside the file"
   ENDIF
   IF ktx2_u64(file, entry + 16) <> len THEN
      RETURN "uncompressedByteLength is not byteLength"
   ENDIF
   IF NOT ktx2_level_bytes(f, level, bytes) OR bytes <> len THEN
      RETURN "a level has not the bytes of its size"
   ENDIF
   f.texel_bytes <- f.texel_bytes + len
   RETURN 0
ENDFUNCTION
```

An end of the level that passes 64 bits is outside the file too: `ckd_add` tells it.
`ktx2_check_levels` first refuses a level index outside the file. Then it checks each level from level 0.

## The copy

`ktx2_copy` copies level 0 first, then each smaller level, with no byte between 2 levels. A file holds the smallest level first, so the copy reverses the order of the file.

## Functions

| Function | Result |
| --- | --- |
| `count_blocks` | The blocks of 1 side of a level. |
| `ktx2_level_bytes` | The bytes of a level. `false` on an overflow. |
| `check_level` | The reason of a fault of 1 level, or nothing. |
| `ktx2_check_levels` | The reason of a fault of the level index or of a level, or nothing. |
| `ktx2_copy` | Copies the levels into the bytes of the texture. |
