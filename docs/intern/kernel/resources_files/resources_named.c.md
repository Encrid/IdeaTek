# src/kernel/resources_files/resources_named.c

This file fills the named sources from the setting `sources`, gives the folder of a source, and starts the watches.

## The named sources

`read_sources` walks the items of `sources`, with a comma between 2 items. `add_named` splits an item at its `=` into a name and a folder, and fills a free record: `free_source` gives it.
An item whose name a source holds already is not taken, nor an item with no free record; each gets its line of `LINES`.
The folder of a named source stays in the text of the setting, which lives in the arena.

## The folders and the watches

`folder_of` gives the folder of a source in the content storage. The default source has `processed_folder` in the mode `processed`, else no folder. A named source has its folder. `user` and a source module have none.
`start_watch` spawns 1 entity of `ideatek:FileRequest` with `IDEATEK_FILE_ACTION_WATCH` on the folder of a source. A refused spawn gets the line `no watch of <folder>: no entity`.

## Functions

| Function | Result |
| --- | --- |
| `free_source` | A free record of the sources, or a null pointer. |
| `add_named` | Fills a named source from 1 item. |
| `read_sources` | Walks the items of the setting. |
| `folder_of` | The folder of a source, as a text of the arena. |
| `start_watch` | Starts the watch of a source. |
