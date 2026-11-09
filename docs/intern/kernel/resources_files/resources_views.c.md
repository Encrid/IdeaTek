# src/kernel/resources_files/resources_views.c

This file fills the views that the pick reads from the loaders.

## The views

`build_views` walks the records of the loaders, and reads `ideatek:ResourceLoader` of each loader that is alive. `fill_view` copies the 4 texts and `ranged` of the component into a view, with the index of the record.
The views are in the arena, 1 for each record of the loaders, so the pick allocates nothing.

## Functions

| Function | Result |
| --- | --- |
| `fill_view` | Fills 1 view from the component of a loader. |
| `build_views` | Fills the views of the loaders that are alive. Their count. |
