# src/engine_subsystems/loader_ktx2/ktx2_give.c

This file takes the bytes of a texture, copies the texels, and attaches `ideatek:Texture`.

## The texture

```pseudo
FUNCTION give_texture(api : U64, id : U64, f : U64, file : U64) RETURNS BOOLEAN
   t <- get_record(api, id)
   IF t = 0 THEN
      RETURN FALSE
   ENDIF
   CALL next_key()
   IF NOT take_room(api, id, f, start) THEN
      IF t.now = 0 THEN
         CALL forget_texture(t)
      ENDIF
      RETURN FALSE
   ENDIF
   CALL ktx2_copy(file, f, start)
   CALL free_block(t.old)
   t.old <- t.now
   t.now <- serial
   CALL attach_texture(api, t, f, start)
   RETURN TRUE
ENDFUNCTION
```

A first load that does not fit frees its record, so the table keeps no record with no texture.
A reload keeps the old block in `old` until `swap` hears the new texture. `free_block` of `old` first frees an older block that no barrier applied.
`next_key` skips the key 0, which names no block.

## Functions

| Function | Result |
| --- | --- |
| `next_key` | The key of the next block. |
| `take_room` | Takes a block for the texels, or writes the line of the room: `docs/usage/engine_subsystems/loader_ktx2.md`, *The messages*. |
| `attach_texture` | Attaches `ideatek:Texture` with the fields of the file and a new revision. |
| `get_record` | The record of the resource, or writes the line of a full table. |
| `give_texture` | Takes the record and the block, copies the texels and attaches the texture. |
