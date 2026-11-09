# src/kernel/resources_files/resources_paths.c

This file splits and joins the paths of the resources. It is pure: the program `resources_paths` of the tests calls it.

## The split

`path_split` reads `source://path#label`: the source ends at the first `://`, the label starts at the last `#`, the path is between them.
`source_name_is_allowed` tells that a name holds only the characters that the contract allows: lower-case letters, digits and `_`.

## The variables

A path can hold `{name}`. `path_variable` gives the first name between braces of a path, or a length of 0 when the path has none.
`path_resolve` writes the path with `value` in the place of each `{name}`. A result that does not fit in the slot of the caller, or a brace with no end, gives `false`.

## The extension and the join

`path_extension` gives the text after the last dot of the last name of a path, or a length of 0.
`path_join` writes `folder/path` into a slot of the caller, or `path` alone when the folder is empty. A result that does not fit gives `false`.
`path_is_under` tells that a path is the folder itself, or a path below it: the test of the reloads.

## Functions

| Function | Result |
| --- | --- |
| `path_split` | Fills the 3 parts of a path. |
| `source_name_is_allowed` | `true` when a name holds only the allowed characters, and is not empty. |
| `path_variable` | The first `{name}` of a path, or a length of 0. |
| `path_resolve` | The path with the value of its variable. `false` when it does not fit. |
| `path_extension` | The extension of a path. |
| `path_join` | The folder and the path with a slash between them. `false` when it does not fit. |
| `path_is_under` | `true` when a path is in a folder. |
