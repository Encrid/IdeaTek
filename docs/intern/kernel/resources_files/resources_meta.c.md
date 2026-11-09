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
      verdict <- read_key(bytes, at, meta)
      IF verdict = 0 THEN
         RETURN FALSE
      ENDIF
      IF verdict = 2 THEN
         CALL take_rest(bytes, meta, at)
         RETURN TRUE
      ENDIF
      at <- line_end(bytes, at) + 1
      meta.line <- meta.line + 1
   ENDWHILE
   RETURN TRUE
ENDFUNCTION
```

A line ends with `\n`. A `\r` before it is a space. The words of a line are in `docs/intern/kernel/resources_files/resources_words.c.md`, and the lines in `docs/intern/kernel/resources_files/resources_lines.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `read_key` | Reads 1 line: 0 for a fault, 1 for a key or an empty line, 2 for the header. |
| `meta_read` | Reads the file. `false` with the number of the refused line. |
