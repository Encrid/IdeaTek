# src/kernel/resources_files/resources_uses.c

This file gives each new use entity its resource and its handle, in the system `round`.

## A load

For a use with `ideatek:ResourceLoad` and no resource yet:

1. `find_resource` looks for the path and the type. A found resource gets 1 more to its count. Its settings stay.
2. Else `take_resource` takes a record with the settings of the load, and `api->spawn` gives the entity. The record starts at `STEP_NEW`. Its source is the default source, `user`, a named source, or a source module. An unknown source fails the load with `IDEATEK_RESOURCE_ERROR_NOT_FOUND`.
3. A full table, a refused spawn, or long texts fail the load with `IDEATEK_RESOURCE_ERROR_NO_ROOM`, when a record is free. With no record at all, the use stays without a handle. The log gets `no room for the resource <path>`.
4. The handle: a load with no label gets `ideatek:ResourceHandle` with the entity of the resource at once. A load of `path#label` gets its handle when the resource of `path` is done: `docs/intern/kernel/resources_files/resources_labels.c.md`.

The count of the resource of `path` holds the uses of its labels too, as the contract says.
The priority of a resource follows its uses that wait: the highest `priority` among them. A new use raises it when it comes.

## An addition

For a use with `ideatek:ResourceAdd`: `resource_of` looks for the entity. A new entity gets a record with an empty path, the type of the addition, `STEP_DONE` and `IDEATEK_LOAD_STATE_LOADED`, and `ideatek:Resource` is attached to it. The use gets its handle at once.

## Functions

| Function | Result |
| --- | --- |
| `source_of` | The source of a split path: a constant, a record, or `RES_NONE`. |
| `new_resource` | Takes the record and the entity of a new resource. |
| `place_load` | Gives a load its resource and its handle. |
| `place_add` | Gives an addition its resource and its handle. |
| `place_uses` | Walks the uses without a resource. |
