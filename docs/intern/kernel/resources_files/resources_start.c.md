# src/kernel/resources_files/resources_start.c

This file builds the tables and starts the watches, in the first round of the module.

## The start

1. `read_settings` reads the settings: `docs/intern/kernel/resources_files/resources_settings.c.md`. A wrong setting stops the start.
2. `take_tables` asks the persistent arena for the 6 tables, the text slots, the 3 hash maps and the block list. Each has the size of its bound. The arena of `load_bytes` comes last.
3. `read_sources` walks the items `name=folder` of `sources`, and fills 1 named source for each item. An item above `max_sources` is not taken, with the line of the room of the sources. A second item with 1 name is not taken either, with the line of the 2 names.
4. With `hot_reload` at `true`, `start_watches` spawns 1 entity of `ideatek:FileRequest` for each source folder. The request is `IDEATEK_FILE_ACTION_WATCH` on the folder of that source.

In the mode `processed`, the default source is `processed_folder`; else it is the content storage itself.
A spawn that Bridge refuses gets the line `no watch of <folder>: no entity`.

## Functions

| Function | Result |
| --- | --- |
| `take_tables` | Takes the memory of the tables from the arena. |
| `read_sources` | Fills the named sources from the setting `sources`. |
| `start_watches` | Spawns the watch of each source folder. |
| `start_resources` | Runs the 4 steps. Gives `false` when the module is broken. |
