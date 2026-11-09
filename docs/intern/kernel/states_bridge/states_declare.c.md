# src/kernel/states_bridge/states_declare.c

This file gives Bridge the 5 types and the 3 systems of the states module.

## The declarations

The module declares `ideatek:ideatek_entity_t` first, because `ideatek:StateScoped` names it.
Then it declares the 4 types of the states contract: `ideatek:State`, `ideatek:StateRequest`, `ideatek:StateChange` and `ideatek:StateScoped`.

| System | Phase | Query | Changes | Before |
| --- | --- | --- | --- | --- |
| `forget` | `first` | `ideatek:StateChange`, write | `ideatek:StateChange` | None |
| `clear` | `first` | `ideatek:StateScoped`, read; `ideatek:State` and `ideatek:StateRequest`, optional, read | None | `forget` |
| `apply` | `first` | `ideatek:StateRequest`, read; `ideatek:State`, optional, write | `ideatek:State`, `ideatek:StateRequest`, `ideatek:StateChange` | `forget`, `clear` |

`clear` states no change, because it only destroys entities, and no statement names a destroy.
The 2 optional terms of `clear` tell the scheduler what it reads through the entity index.

## Functions

| Function | Result |
| --- | --- |
| `declare_types` | Declares the 5 types. |
| `add_runs` | Declares the 3 systems, and keeps their slots in `states_module`. |
