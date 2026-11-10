# src/engine_subsystems/loader_ktx2/ktx2_main.c

This file holds the manifest and the 3 entries of the module.

## The entries

`init` declares the types, spawns the entity of the loader, and adds the observers. The module has no system.
`shutdown` and `reload` give 0, because the module holds nothing of the system.

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares, spawns the loader and adds the observers. Gives 0. |
| `shutdown` | Gives 0. |
| `reload` | Gives 0. |
