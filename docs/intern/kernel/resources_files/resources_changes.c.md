# src/kernel/resources_files/resources_changes.c

This file hears the changes of the watched folders, and marks the resources to reload.

## The changes

`changes` is the observer of `ideatek:FileChange`. `watch_source` tells which source a watch entity belongs to; a change of another entity is skipped.

`mark_lines` walks the lines of `paths`. A line under the folder of the source loses that folder, and `mark_changed` gets the rest.

`mark_changed` takes `.meta` off the end of the path. Each resource of the source whose path is the changed path, or is under it, gets `reload_at`: `now` plus `reload_delay_ms`. A sub-resource is not marked: its parent reloads it.
`say_no_clock` writes the line of the clock at the first change of a deployment with no `ideatek:Frame`.

## Functions

| Function | Result |
| --- | --- |
| `watch_source` | The source of a watch entity, or `RES_NONE`. |
| `mark_changed` | Marks the resources under a changed path. |
| `mark_lines` | Walks the lines of a change. |
| `say_no_clock` | Writes the line of the clock 1 time. |
| `changes` | The observer of the changes. |
