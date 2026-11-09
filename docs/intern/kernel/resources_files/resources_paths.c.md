# src/kernel/resources_files/resources_paths.c

This file splits the paths of the resources. It is pure: the program `resources_paths` of the tests calls it.

## The split

`path_split` reads `source://path#label`: the source ends at the first `://`, the label starts at the last `#`, the path is between them.
`source_name_is_allowed` tells that a name holds only the characters that the contract allows: lower-case letters, digits and `_`.

## The extension

`path_extension` gives the text after the last dot of the last name of a path. With no dot, the extension is empty.
The variables and the joins are in `docs/intern/kernel/resources_files/resources_joins.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `find_last` | The last place of a character in a text, or a null pointer. |
| `find_scheme` | The place of `://` in a text, or a null pointer. |
| `path_split` | Fills the 3 parts of a path. |
| `source_name_is_allowed` | `true` when a name holds only the allowed characters, and is not empty. |
| `path_extension` | The extension of a path. |
