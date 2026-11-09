# src/kernel/resources_files/resources_main.c

This file loads the module.

## The load

`init` declares the types: `docs/intern/kernel/resources_files/resources_declare.c.md`. Then it adds the system and the observers: `docs/intern/kernel/resources_files/resources_observe.c.md`.
The settings come after `init`, so the first round reads them: `docs/intern/kernel/resources_files/resources_settings.c.md`.
`reload` and `shutdown` give 0. The module took no object of the system: the files module holds the files, and the arena of Bridge holds the tables.

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares the types, the system and the observers. |
| `reload` | Gives 0. |
| `shutdown` | Gives 0. |
