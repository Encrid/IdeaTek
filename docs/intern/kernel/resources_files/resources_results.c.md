# src/kernel/resources_files/resources_results.c

This file takes the results of the reads: the observers `results` and `answers`, and the copy of the bytes in the round.

## The observers

`results` hears each `ideatek:FileResult` of the world. It finds the record of the read by the entity, and skips an entity that is no read of the module. It records the status of the result.
A result on a watch entity is the start of the watch: a status that is not `IDEATEK_FILE_STATUS_OK` gets the line `no watch of <folder>: <status>`.
`answers` hears each `ideatek:ResourceSourceResult`, and records the error of the result the same way.

## The copy

The files module destroys the entity of a result in the round after the result. So `take_results` copies the bytes in that round, before the barrier. A result of a source module stays until the module destroys the read entity, after the copy.

| Kind | Result | Effect |
| --- | --- | --- |
| `READ_META` | Not found | The defaults of the meta file. |
| `READ_META` | Ok | `meta_read` reads the bytes: `docs/intern/kernel/resources_files/resources_step.c.md`, *The meta file*. |
| `READ_FILE` | Ok | A block of the arena takes the bytes, then the resource gives them to its loader. |
| `READ_RANGE` | Ok | A block takes the bytes of the range: `docs/intern/kernel/resources_files/resources_ranges.c.md`. |
| Any | Another status | The error of the table of the contract: not found, too big, or the read failed. |

More bytes than `load_bytes` in a file give `IDEATEK_RESOURCE_ERROR_TOO_BIG`. A `ranged` loader is the exception: its first read asks `head_bytes` only.
When no gap of the arena holds the bytes, the read is dropped. The resource goes back to `STEP_READ`, and reads again when a block is free.

## Functions

| Function | Result |
| --- | --- |
| `results` | The observer of `ideatek:FileResult`. |
| `answers` | The observer of `ideatek:ResourceSourceResult`. |
| `error_of` | The error of the contract for a status of the files part. |
| `take_result` | Applies 1 result to its resource, and frees the read. |
| `take_results` | Walks the reads with a result. |
