# src/kernel/resources_files/resources_arrivals.c

This file applies the results of the reads in the round after their arrival.

## The arrivals

`take_results` walks the reads that have a result. For each, `read_arrival` reads the result: `docs/intern/kernel/resources_files/resources_copy.c.md`. Then `dispatch` acts by the kind of the read:

| Kind | Action |
| --- | --- |
| `READ_META` | `apply_meta`, with no bytes for a file that is not there. |
| `READ_RANGE` | `give_range`: `docs/intern/kernel/resources_files/resources_ranges.c.md`. |
| `READ_FILE` | `take_file`. |

`refuse_file` ends a file read that cannot go on. A `NOT_FOUND` with a variable in the path and no fallback tried yet puts the resource back at `STEP_READ` with the fallback. Another error fails the resource. A file larger than `load_bytes` fails with `IDEATEK_RESOURCE_ERROR_TOO_BIG`, unless its loader is `ranged`.

`take_file` copies the bytes into a block. When no gap holds them, the resource goes back to `STEP_READ`. When the pick waited for the bytes, `step_resource` picks now. Else `give_bytes` gives them to the loader.

A read of a source module is destroyed after the copy. The read record is freed.

## Functions

| Function | Result |
| --- | --- |
| `refuse_file` | `true` when the read ends in a retry or a failure. |
| `take_file` | Copies the bytes of a file and moves the resource. |
| `dispatch` | Acts by the kind of the read. |
| `take_result` | Applies 1 result and frees its read. |
| `take_results` | Walks the reads with a result. |
