# src/kernel/resources_files/resources_start.c

This file builds the tables and starts the watches, in the first round of the module.

## The start

1. `read_settings` reads the settings: `docs/intern/kernel/resources_files/resources_settings.c.md`. A wrong setting stops the start.
2. `take_tables` and `take_maps` ask the persistent arena for the tables, the slots, the maps and the block list. Each has the size of its bound. The arena of `load_bytes` comes last.
3. `read_sources` reads the items of `sources` into the named sources: `docs/intern/kernel/resources_files/resources_named.c.md`.
4. With `hot_reload` at `true`, `start_watch` starts 1 watch for the default source, and 1 for each named source.

## Functions

| Function | Result |
| --- | --- |
| `take` | Takes a cleared array from the arena. |
| `take_maps` | Takes the 3 hash maps, the block list and the arena of `load_bytes`. |
| `take_tables` | Takes the 6 tables, the views, the folds, the text slots and the path slots. |
| `start_resources` | Runs the 4 steps. Gives `false` when the module is broken. |
