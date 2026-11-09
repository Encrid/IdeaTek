# src/kernel/resources_files/resources_ranges.c

This file reads the ranges of a file for a `ranged` loader.

## A range

The observer `ranges` hears `ideatek:ResourceRead` on a resource, and records the range: its offset, its length and its priority. The module detaches the request.
In the round, `start_ranges` starts a read of the kind `READ_RANGE` for each resource with a range and no read that runs, through `start_read`.

When the result comes, `give_range` takes a block for the bytes. It attaches `ideatek:ResourceBytes` with the offset, the size of the whole file, the bytes and no error. The loader answers with `ideatek:ResourceOutcome`, and the module frees the block.
A range that ends past the file, or a failed read, attaches `ideatek:ResourceBytes` with no bytes and `IDEATEK_RESOURCE_ERROR_READ_FAILED` in `error`. The state of the resource does not change.

## Functions

| Function | Result |
| --- | --- |
| `ranges` | The observer of `ideatek:ResourceRead`. |
| `start_ranges` | Starts the reads of the ranges. |
| `give_range` | Attaches the bytes of a range to the resource. |
