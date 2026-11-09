# src/kernel/resources_files/resources_pick.c

This file picks the loader of a load, with the rules of `docs/extern/kernel/ideatek_resources.h.md`, *The loaders*. It is pure: the program `resources_pick` of the tests calls it with views that it builds.

## The pick

`pick_loader` gets the views of the loaders, the type of the load, the extension of the path and the name of the meta file. It also gets the first bytes of the file when they are known.

1. With a name: the loader with that name. 2 loaders with the name give `IDEATEK_RESOURCE_ERROR_TWO_LOADERS`. No loader gives `IDEATEK_RESOURCE_ERROR_NO_LOADER`. A type that is not the type of the load gives `IDEATEK_RESOURCE_ERROR_WRONG_TYPE`.
2. Else, with a type that exactly 1 loader has: that loader.
3. Else, among the loaders of the type, the loaders whose lines hold the extension. An empty type allows each loader. 0 gives `IDEATEK_RESOURCE_ERROR_NO_LOADER`. 1 is the pick.
4. With 2 or more, and no bytes yet, the pick needs the bytes. With the bytes, the loader whose magic starts the bytes is the pick. 0 or 2 such loaders give `IDEATEK_RESOURCE_ERROR_TWO_LOADERS`.

`has_extension` walks the lines of a loader; each line ends with `\n`. `starts_with` compares the magic with the start of the bytes.

## Functions

| Function | Result |
| --- | --- |
| `same_text` | `true` when 2 texts are equal. |
| `has_extension` | `true` when the lines of a loader hold the extension. |
| `pick_named` | The pick by the name of the meta file. |
| `pick_typed` | The pick by the type, when 1 loader has it. |
| `pick_loader` | The pick by the rules, or its error. |
