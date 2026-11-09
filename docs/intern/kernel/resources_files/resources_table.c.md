# src/kernel/resources_files/resources_table.c

This file holds the table of the resources: it finds, takes and drops the records, and keeps the 2 hash maps of the resources.

## The records

A free record has an entity of 0. `take_resource` finds the first free record, fills its texts and puts it in the 2 maps.
The slot of the record takes its texts: the path, a zero, the type, a zero, the settings. `text_of` gives a part of the slot as a `bridge_text_t`.
A path or a type that does not fit in `path_bytes` gets no record: the caller gives `IDEATEK_RESOURCE_ERROR_NO_ROOM`.

`find_resource` hashes the path and the type, asks `by_key`, then compares the texts of the record, because 2 keys can collide.
`resource_of` asks `by_entity` for the record of an entity: a resource, or a sub-resource.
`drop_resource` frees the record, its block of bytes and its 2 keys.

## Functions

| Function | Result |
| --- | --- |
| `text_of` | A part of the slot of a record, as a text. |
| `key_of` | The hash of a path and a type. |
| `find_resource` | The record of a path and a type, or `RES_NONE`. |
| `take_resource` | A new record with its texts, or `RES_NONE` when the table or the slot is full. |
| `resource_of` | The record of an entity, or `RES_NONE`. |
| `drop_resource` | Frees a record. |
