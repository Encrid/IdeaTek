# src/kernel/resources_files/resources_observe.c

This file adds the system `round` and the observers of the module.

## The system and the observers

`round` runs in the phase `first`. Its changes are each type that the module attaches or detaches: `ideatek:FileRequest`, the 12 types of its `write` grants, and `ideatek:ResourceOutcome` for its detach.
Each observer records what it hears in the tables, and `round` acts on the tables. An observer spawns nothing and attaches nothing.

| Observer | Events | Term | What it records |
| --- | --- | --- | --- |
| `loads` | enter, leave | `ideatek:ResourceLoad` | A new use entity, or a use entity that went: `docs/intern/kernel/resources_files/resources_loads.c.md`. |
| `adds` | enter, leave | `ideatek:ResourceAdd` | The same, for a resource of a module. |
| `loaders` | enter, leave | `ideatek:ResourceLoader` | A loader that came or went: `docs/intern/kernel/resources_files/resources_registry.c.md`. |
| `sources` | enter, leave | `ideatek:ResourceSource` | A source module that came or went. |
| `variables` | enter, attach, leave | `ideatek:ResourceVariable` | A variable that came, changed or went. |
| `outcomes` | enter, attach | `ideatek:ResourceOutcome` | The end of the work of a loader: `docs/intern/kernel/resources_files/resources_finish.c.md`. |
| `labels` | enter | `ideatek:ResourceLabel` | A sub-resource: `docs/intern/kernel/resources_files/resources_labels.c.md`. |
| `ranges` | enter, attach | `ideatek:ResourceRead` | A range that a loader asks: `docs/intern/kernel/resources_files/resources_ranges.c.md`. |
| `reloads` | enter | `ideatek:ResourceReload` | A reload that a module asks: `docs/intern/kernel/resources_files/resources_reload.c.md`. |
| `results` | enter | `ideatek:FileResult` | The result of a read or of a watch of the module. A result of another entity is skipped: `docs/intern/kernel/resources_files/resources_reads.c.md`. |
| `changes` | enter, attach | `ideatek:FileChange` | The changed paths of a watch of the module. |
| `answers` | enter | `ideatek:ResourceSourceResult` | The result of a read of a source module. |

The 12 observers need 12 functions. This file adds them in 3 groups of 4, with 1 helper that builds the term and keeps the index of the list.

## Functions

| Function | Result |
| --- | --- |
| `add_observer` | Adds 1 observer on 1 term, and keeps the index of its list. |
| `add_use_observers` | Adds `loads`, `adds`, `loaders` and `sources`. |
| `add_work_observers` | Adds `variables`, `outcomes`, `labels` and `ranges`. |
| `add_file_observers` | Adds `reloads`, `results`, `changes` and `answers`. |
| `add_runs` | Adds `round` and the 3 groups. |
