# src/kernel/resources_files/resources_table.c

This file holds the table of the resources: it finds and drops the records, and gives their texts.

## The records

A free record has an entity of 0. `docs/intern/kernel/resources_files/resources_records.c.md` takes a record.
The slot of the record holds its texts: the path, a zero, the type, a zero, the settings. `text_of` gives a part of the slot as a `res_text_t`.

`find_resource` hashes the path and the type, asks `by_key`, then compares the texts of the record, because 2 keys can collide.
`resource_of` asks `by_entity` for the record of an entity: a resource, or a sub-resource.
`drop_resource` frees the record, its block of bytes and its 2 keys.

## Functions

| Function | Result |
| --- | --- |
| `slot_of` | The text slot of a record. |
| `text_of` | A part of the slot of a record, as a text. |
| `find_resource` | The record of a path and a type, or `RES_NONE`. |
| `resource_of` | The record of an entity, or `RES_NONE`. |
| `drop_resource` | Frees a record. |
