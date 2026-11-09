# src/platform_abstraction/files_mosl/files_paths.c

This file checks and joins the paths, with no call of Bridge or of the system, so a program tests it alone.

## The rule

`path_is_allowed` gives `true` for a path of the contract: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The paths*.
A path is a sequence of names with a `/` between two names, or an empty text for the folder of the storage.
A path is refused when a name is `..`, `.` or empty, or when a byte is `\` or `:`. A `/` at the end makes an empty last name, so it is refused too.

## The join and the split

`path_join` writes the path of a storage, a `/` and a path into a buffer of `FILES_PATH` bytes. An empty path gives the path of the storage alone. It gives `false` when the result does not fit.
`path_split` gives the folder part and the last name of a path. For `a/b/c`, the folder is `a/b` and the name is `c`. For `c`, the folder is empty.

`path_parent` cuts the last name of a path of the system, and the `/` or `\` before it. For a path with no separator, the result is empty.

## The branches

| Case | Result |
| --- | --- |
| A path with `..`, `.`, an empty name, `\` or `:`. | Refused. |
| An empty path. | Allowed: the folder of the storage. |
| A join that does not fit in `FILES_PATH`. | `false`. |
| A parent of a path with no separator. | Empty. |

## Functions

| Function | Result |
| --- | --- |
| `path_is_allowed` | Gives `true` for a path that the rule of the contract allows. |
| `path_join` | Joins a storage and a path into a buffer. Gives `false` when the result does not fit. |
| `path_split` | Gives the folder part and the last name of a path. |
| `path_parent` | Cuts the last name of a path of the system. |
