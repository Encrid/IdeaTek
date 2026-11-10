# src/engine_subsystems/space_bridge/space_observe.c

This file registers the 5 observers and the system of the module.

| Observer | Terms | Events | Changes |
| --- | --- | --- | --- |
| `link` | `ideatek:Parent` | Enter, attach and leave | `ideatek:Sibling`, `ideatek:Children`, `ideatek:Transform` |
| `place` | `ideatek:Transform` | Enter and attach | `ideatek:WorldTransform`, `ideatek:PreviousWorldTransform`, `ideatek:Teleported` |
| `relink` | `ideatek:Sibling` | Enter, attach and leave | The changes of `place` |
| `previous` | `ideatek:Interpolated` | Enter and leave | `ideatek:PreviousWorldTransform` |
| `gone` | `ideatek:Transform` | Leave | The places, the lists, `ideatek:Parent` and `ideatek:Transform` |

Each observer runs after the observer before it in the table. The query of `step` names `ideatek:Interpolated`, `ideatek:WorldTransform` and `ideatek:PreviousWorldTransform`.

## Functions

| Function | Result |
| --- | --- |
| `add_observer` | Registers 1 observer of the table, and gives the slot of its list. |
| `add_step` | Registers the system and its query. |
| `add_observers` | Registers the 5 observers and the system. |
