# src/kernel/resources_files/resources_pick.h

This header gives the view of a loader that the pick reads, and its result.

## The types

`res_loader_view_t` is 1 loader as the pick sees it. It holds the name, the type, the lines of extensions and the magic as texts, and `ranged`.
`res_pick_t` is the result. It holds the index of the loader, or `RES_NONE` with 1 of the 3 errors of the contract. `needs_bytes` tells that the pick waits for the bytes.
