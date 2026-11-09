# src/kernel/resources_files/resources_reads.c

This file writes the path of a read, and asks the bytes of the files module or of a source module.

## A read

`docs/intern/kernel/resources_files/resources_queue.c.md` takes the record of a read and spawns its entity. Then the read asks its bytes:

- For the default source, a named source or `user`: `ideatek:FileRequest` with `IDEATEK_FILE_ACTION_READ` and the storage of the source. Its path is the path of the file joined to the folder of the source. The offset, the length and the priority of the resource follow.
- For a source module: `ideatek:ResourceSourceRead` with the entity of the source, the path in the source, the offset, the length and the priority.

The path goes through `path_resolve` when it holds a variable: the value of the variable, or its `fallback` on a second try after `IDEATEK_RESOURCE_ERROR_NOT_FOUND`.
A read of a meta file adds `.meta` to the path. The kind of the read tells what the result is for: `READ_META`, `READ_FILE` or `READ_RANGE`.
The order of the reads is in `docs/intern/kernel/resources_files/resources_order.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `value_of` | The value of the variable of a path, or its fallback. `false` with no variable of that name. |
| `write_path` | The path of the file of a resource, joined and resolved, with `.meta` for a meta read. |
| `ask_file` | Attaches `ideatek:FileRequest` to the entity of a read. |
| `ask_source` | Attaches `ideatek:ResourceSourceRead` to the entity of a read. |
| `is_module_source` | `true` when a source is a source module. |
