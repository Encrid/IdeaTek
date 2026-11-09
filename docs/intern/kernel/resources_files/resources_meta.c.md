# src/kernel/resources_files/resources_meta.c

This file reads a meta file in the TOML subset of `docs/usage/kernel/resources_files.md`, *The meta files*. It is pure: the program `resources_meta` of the tests calls it.

## The reader

`meta_read` walks the lines of the bytes of the file. Before the header `[settings]`, a line is 1 of these:

- empty, or spaces only;
- a comment: its first character that is not a space is `#`;
- `action = "<word>"`, with `load`, `process` or `ignore` as the word;
- `loader = "<name>"`, with a name of 1 to 63 bytes;
- the header `[settings]`.

Spaces around `=` and at the ends of the line are allowed. A key 2 times, a word outside the 3, another line, or a second header is a fault. The reader stops and gives the number of the line.
After the header, the rest of the file is the settings text, with no check: the loader reads it.

```pseudo
FUNCTION meta_read(bytes : U64, len : U32, meta : U64) RETURNS BOOLEAN
   line <- 1
   at <- 0
   WHILE at < len
      end <- line_end(bytes, len, at)
      IF is_header(bytes, at, end) THEN
         meta.settings <- end + 1
         RETURN TRUE
      ENDIF
      IF NOT read_key(bytes, at, end, meta) THEN
         meta.line <- line
         RETURN FALSE
      ENDIF
      at <- end + 1
      line <- line + 1
   ENDWHILE
   RETURN TRUE
ENDFUNCTION
```

A line ends with `\n`. A `\r` before it is a space.

## Functions

| Function | Result |
| --- | --- |
| `line_end` | Where a line ends. |
| `read_word` | The quoted word of a `key = "word"` line, or a length of 0. |
| `read_key` | Reads 1 line before the header into the meta. `false` for a fault. |
| `is_header` | `true` when the line is `[settings]`. |
| `meta_read` | Reads the file. `false` with the number of the refused line. |
