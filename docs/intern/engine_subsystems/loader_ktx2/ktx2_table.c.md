# src/engine_subsystems/loader_ktx2/ktx2_table.c

This file finds, adds and frees the records of the textures.

## The records

`find_texture` walks the table, so `find_texture(0)` gives a free record. Before the start, the table holds no record.
`free_block` gives a block back to the list, and sets its key to 0. A key of 0 frees nothing.

## Functions

| Function | Result |
| --- | --- |
| `find_texture` | The record of an entity, or nothing. |
| `add_texture` | A free record for an entity, or nothing when the table is full. |
| `free_block` | Frees the block of a key, and sets the key to 0. |
| `forget_texture` | Frees both blocks of a record, and frees the record. |
