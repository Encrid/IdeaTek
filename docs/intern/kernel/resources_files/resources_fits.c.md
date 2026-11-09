# src/kernel/resources_files/resources_fits.c

This file tells whether a loader fits an extension and the first bytes of a file. It is pure, and the pick calls it.

## The fits

`has_extension` walks the lines of the extensions of a loader. Each line ends with `\n`, and the extension has no dot.
`fits_extension` tells that a loader has the type of the load, or that the load has no type, and that it has the extension.
`fits_magic` tells that the first bytes of the file start with the magic of the loader, when the bytes are known.

## Functions

| Function | Result |
| --- | --- |
| `has_extension` | `true` when the lines of a loader hold the extension. |
| `fits_magic` | `true` when the bytes start with the magic of the loader. |
| `fits_extension` | `true` when the type and the extension fit the loader. |
