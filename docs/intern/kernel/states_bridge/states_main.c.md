# src/kernel/states_bridge/states_main.c

This file loads the module.

## The load

`init` declares the types and the systems: `docs/intern/kernel/states_bridge/states_declare.c.md`.
It reads no setting, because the module has none.
`reload` and `shutdown` give 0. The module took nothing from the system, and its manifest states `reloadable` at `false`.

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares the types and the systems. |
| `reload` | Gives 0. |
| `shutdown` | Gives 0. |
