# src/engine_subsystems/loader_ktx2/ktx2_head.c

This file reads the header of a file, runs the 4 steps of the read, and gives the shape of a texture.

## The header

`ktx2_read_head` refuses a file shorter than `KTX2_HEAD`, and a file whose first 12 bytes are not the identifier of KTX 2.0.
Then it copies the fields of the header and the place of the data format descriptor. A `levelCount` of 0 becomes 1 level.

## The read

```pseudo
FUNCTION ktx2_read(file : U64, f : U64) RETURNS U64
   reason <- ktx2_read_head(file, f)
   IF reason = 0 THEN
      reason <- ktx2_check_shape(f)
   ENDIF
   IF reason = 0 THEN
      reason <- ktx2_read_block(file, f)
   ENDIF
   IF reason = 0 THEN
      reason <- ktx2_check_levels(file, f)
   ENDIF
   RETURN reason
ENDFUNCTION
```

The result is the text of the first reason, or nothing for a file that the module takes.

## Functions

| Function | Result |
| --- | --- |
| `ktx2_read_head` | Reads the header and the index, or gives the reason of a refusal. |
| `ktx2_read` | Runs the 4 steps, and gives the first reason. |
| `ktx2_dimension` | The shape: a cube for 6 faces, a volume for a depth, an array for layers, else a 2D image. |
