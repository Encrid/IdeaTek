# src/kernel/resources_files/resources_lines.c

This file cuts the lines of a meta file, and starts and ends its reading. It is pure, and `meta_read` calls it.

## The lines

`line_at` gives the line that starts at a position, and where it ends: at the next `\n`, or at the end of the bytes.
`meta_start` gives a meta the defaults: the action `load`, no loader, no settings, and the line 1.
`take_rest` makes the bytes after the header `[settings]` the settings of the meta.

## Functions

| Function | Result |
| --- | --- |
| `line_at` | The line at a position, and its end. |
| `meta_start` | The defaults of a meta. |
| `take_rest` | The settings text after the header. |
