# src/kernel/resources_files/resources_settings.c

This file reads and checks the 18 settings when the module starts.

## The settings

`read_settings` reads the singleton `ideatek_resources_files:Settings` and fills `settings` of the module.
Each count goes through `check_count` with its range. Each choice goes through `pick_choice` with its words.
Each text goes through `check_text`: the text is inside the arena and has at most 4095 bytes.
A wrong setting breaks the module, with the line of `readme.md`, *The contracts*: its system gives 1 in each round.

| Setting | Check |
| --- | --- |
| `reload_delay_ms` | 0 to 60000. |
| `load_bytes` | 1 to 1073741824. |
| `head_bytes` | 1 to `load_bytes`. |
| `max_resources`, `max_uses`, `max_loaders`, `max_sources`, `max_variables`, `max_reads`, `max_labels` | 1 to the top of `docs/usage/kernel/resources_files.md`, *Usage*. |
| `path_bytes` | 16 to 4096. |
| `settings_bytes` | 0 to 65536. |
| `hot_reload` | `true` or `false`: a `bool` needs no check. |
| `meta_check` | `always`, `never` or `paths`. |
| `mode` | `unprocessed` or `processed`. |
| `meta_paths`, `processed_folder`, `sources` | A text of at most 4095 bytes. `processed_folder` also obeys the path rule of `docs/extern/platform_abstraction/ideatek_files.h.md`, *The paths*. |

## Functions

| Function | Result |
| --- | --- |
| `check_count` | `true` when a count is in its range, else the line. |
| `pick_choice` | The index of the word of a text among 3 words, or -1 with the line. |
| `check_text` | `true` when a text is inside the arena and short enough, else the line. |
| `read_settings` | Fills `settings` of the module. `false` for a setting outside its range. |
