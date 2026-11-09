# src/kernel/resources_files/resources_reads.c

This file starts the reads of the files, through the files module or a source module.

## A read

`start_read` takes a free record of the reads, spawns its entity, and asks the bytes:

- For the default source, a named source or `user`: `ideatek:FileRequest` with `IDEATEK_FILE_ACTION_READ` and the storage of the source. Its path is the path of the file joined to the folder of the source. The offset, the length and the priority of the resource follow.
- For a source module: `ideatek:ResourceSourceRead` with the entity of the source, the path in the source, the offset, the length and the priority.

The path goes through `path_resolve` when it holds a variable: the value of the variable, or its `fallback` on a second try after `IDEATEK_RESOURCE_ERROR_NOT_FOUND`.
A read of a meta file adds `.meta` to the path. The kind of the read tells what the result is for: `READ_META`, `READ_FILE` or `READ_RANGE`.
With no free record, or a refused spawn, the read does not start: the resource waits for a later round.

## The order

`start_reads` walks the resources at `STEP_READ` while a record is free, and starts the one with the highest priority first. 2 resources with 1 priority start in the order of their records.

## Functions

| Function | Result |
| --- | --- |
| `write_path` | The path of the file of a resource, joined and resolved, with `.meta` for a meta read. |
| `ask_file` | Attaches `ideatek:FileRequest` to the entity of a read. |
| `ask_source` | Attaches `ideatek:ResourceSourceRead` to the entity of a read. |
| `start_read` | Starts 1 read of a kind for a resource. `false` when no read can start. |
| `start_reads` | Starts the waiting reads by priority. |
