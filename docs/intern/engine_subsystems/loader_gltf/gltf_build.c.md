# src/engine_subsystems/loader_gltf/gltf_build.c

This file builds 1 file in 2 passes.

```pseudo
FUNCTION build_file(api : U64, f : U64, data : U64) RETURNS BOOLEAN
   IF NOT wire_buffers(b) OR NOT plan_parts(b) THEN
      RETURN FALSE
   ENDIF
   CALL rank_joints(b)
   CALL say_images(b)
   CALL write_all(b)
   IF NOT take_output(b, key) THEN
      RETURN FALSE
   ENDIF
   CALL write_all(b)
   CALL free_key(f.old)
   f.old <- f.now
   f.now <- key
   CALL attach_all(b)
   RETURN TRUE
ENDFUNCTION
```

The first pass has no base, so it only counts the bytes and says the lines of the faults. The second pass writes the same bytes into the block.

## Functions

| Function | Result |
| --- | --- |
| `text_of` | The text of a span of the block. |
| `take_output` | Takes the block of the output, or writes the line of its size. |
| `build_file` | Builds and attaches each part of a file. |
