# src/kernel/resources_files/resources_joins.c

This file resolves the variables of a path, and joins a folder and a path. It is pure, and the reads and the labels call it.

## The variables

A path can hold `{name}`. `path_variable` gives the first name between braces, or a length of 0 when the path has none.
`path_resolve` writes the path with `value` in the place of each `{name}`, piece by piece through `put_piece`. A result that does not fit in the slot of the caller, or a brace with no end, gives `false`.

## The join

`path_join` writes `folder/path` into a slot of the caller, or `path` alone when the folder is empty. A result that does not fit gives `false`.
`path_is_under` tells that a path is the folder itself, or a path below it. An empty folder holds every path.

## Functions

| Function | Result |
| --- | --- |
| `path_variable` | The first `{name}` of a path, or a length of 0. |
| `put_piece` | Adds bytes to a buffer. `false` when they do not fit. |
| `path_resolve` | The path with the value of its variable. `false` when it does not fit. |
| `path_join` | The folder and the path with a slash between them. `false` when it does not fit. |
| `path_is_under` | `true` when a path is in a folder. |
