# src/kernel/resources_files/resources_results.c

This file hears the results of the reads: the observers `results` and `answers`. The round applies them: `docs/intern/kernel/resources_files/resources_arrivals.c.md`.

## The observers

`results` hears each `ideatek:FileResult` of the world. It finds the record of the read by the entity, and skips an entity that is no read of the module. It records the status of the result.
A result on a watch entity is the start of the watch: a status that is not `IDEATEK_FILE_STATUS_OK` gets the line `no watch of <folder>: <status>`.
`answers` hears each `ideatek:ResourceSourceResult`, and records the error of the result the same way.

## The copy

The files module destroys the entity of a result in the round after the result. So the round after the arrival copies the bytes, before the barrier. A result of a source module stays until the module destroys the read entity, after the copy.

## Functions

| Function | Result |
| --- | --- |
| `read_of` | The record of a read by its entity, or `RES_NONE`. |
| `watch_result` | Writes the line of a watch that the files module refused. |
| `results` | The observer of `ideatek:FileResult`. |
| `answers` | The observer of `ideatek:ResourceSourceResult`. |
