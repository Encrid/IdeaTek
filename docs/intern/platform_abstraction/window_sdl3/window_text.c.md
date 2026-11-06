# src/platform_abstraction/window_sdl3/window_text.c

This file copies the texts that the module gets.

## The cut

`cut_text` copies `len` bytes of a text of UTF-8 into an array of 128 bytes, and ends the copy with a zero:

```pseudo
FUNCTION cut_text(BYREF to : ARRAY[0:127] OF U8, from : STRING, len : U32) RETURNS BOOLEAN
   DECLARE keep : U32
   keep <- len
   IF len > 127 THEN
      keep <- 127
      WHILE keep > 0 AND from[keep] >= 128 AND from[keep] < 192
         keep <- keep - 1
      ENDWHILE
   ENDIF
   CALL memcpy(to, from, keep)
   to[keep] <- 0
   RETURN keep < len
ENDFUNCTION
```

A byte from 128 to 191 continues a code point. Thus the loop goes back to the first byte of the code point that does not fit, and keeps the bytes before it.
The result is `true` when the copy lost bytes. The caller then writes its line: a title or the name of a display.
The title of a request is `len` bytes long up to its first zero, or 128 bytes with no zero.

## The copy of the arena

`copy_arena_text` copies a `bridge_text_t` into a new C string, which the caller frees:

```pseudo
FUNCTION copy_arena_text(text : bridge_text_t) RETURNS STRING
   DECLARE copy : STRING
   IF text.at < arena_lo OR text.at > arena_hi OR text.len > arena_hi - text.at THEN
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the text at <at> with <len> bytes is not in the persistent arena")
      RETURN nullptr
   ENDIF
   copy <- malloc(text.len + 1)
   IF copy = nullptr THEN
      CALL bridge_native_say(BRIDGE_LOG_ERR, "no memory for a text of <count> bytes")
      RETURN nullptr
   ENDIF
   CALL memcpy(copy, shared_base + text.at, text.len)
   copy[text.len] <- 0
   RETURN copy
ENDFUNCTION
```

`arena_lo` and `arena_hi` are the bounds of the arena that the module can read: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#bridge_header_t.
The test compares `len` with the bytes after `at`, so no sum can wrap around.
SDL3 needs a zero at the end of a path and of a text, and a `bridge_text_t` has none. Thus the copy adds it.

## The branches

| Case | Result |
| --- | --- |
| A text of 127 bytes or less. | The whole text, and `false`. |
| A text of 128 bytes or more that ends a code point at byte 127. | 127 bytes, and `true`. |
| A text with a code point of 2, 3 or 4 bytes across byte 127. | The bytes before that code point, and `true`. |
| A text of the arena. | A C string with its bytes and a zero. |
| A text with an `at` or a `len` outside of the arena. | The line of the arena, and null. |
| No memory. | The line of the memory, and null. |

## Functions

| Function | Result |
| --- | --- |
| `cut_text` | Copies a text of UTF-8 into an array of 128 bytes, cut at a whole code point. Gives `true` if it cut. |
| `copy_arena_text` | Copies a text of the persistent arena into a C string. Gives null after a line. |
