# src/platform_abstraction/window_sdl3/window_text.c

This file copies the texts of the persistent arena.

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
| A text of the arena. | A C string with its bytes and a zero. |
| A text with an `at` or a `len` outside of the arena. | The line of the arena, and null. |
| No memory. | The line of the memory, and null. |

## Functions

| Function | Result |
| --- | --- |
| `copy_arena_text` | Copies a text of the persistent arena into a C string. Gives null after a line. |
