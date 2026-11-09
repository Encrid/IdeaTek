# src/kernel/resources_files/resources_records.c

This file takes the record of a resource, binds it to its entity, and finds a use by its entity.

## A record

`take_resource` finds the first free record and clears it with `clear_record`. `write_texts` puts the texts in the slot of the record, in the order of `docs/intern/kernel/resources_files/resources_table.c.md`.
A path and a type above `path_bytes`, or settings above `settings_bytes`, give a record with empty texts. That record fails at once with `IDEATEK_RESOURCE_ERROR_NO_ROOM`.

A record that fits goes in `by_key`. Until `bind_resource`, the record holds the entity 1, so that no other load takes it.
`bind_resource` writes the entity, the texts of the view, and the record goes in `by_entity`. The record is fresh and changed: the round writes its view.
`use_of` finds the record of a use by its entity.

## Functions

| Function | Result |
| --- | --- |
| `clear_record` | Clears a record to its start. |
| `write_texts` | Writes the texts of a record in its slot. |
| `take_resource` | A new record with its texts, or `RES_NONE` when the table is full. |
| `bind_resource` | Binds a record to the entity of its resource. |
| `use_of` | The record of a use by its entity, or `RES_NONE`. |
