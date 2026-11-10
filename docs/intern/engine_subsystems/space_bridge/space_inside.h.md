# src/engine_subsystems/space_bridge/space_inside.h

This header holds the slots of the space module, and what its sources share.

## The state

`space_module` holds the slots between 2 calls, since no argument of a call carries them. It is the 1 global of the module, which G4 of the style excepts.

| Field | Value |
| --- | --- |
| `types` | The slot of each type of the space part, and of `ideatek:Tick`, as the declarations gave them. |
| `lists` | The slot of the list of each of the 5 observers: `link`, `place`, `relink`, `previous` and `gone`. |
| `step` | The slot of the query of the system `step`. |
| `tick` | The `tick` of `ideatek:Tick` at the last run of `step`. |

The places and the lists live in the components of the space part, not in the module.

## The names

The slots come in this order: the 7 types of the fields, `TRANSFORM`, `PARENT`, `WORLD`, `CHILDREN`, `SIBLING`, `PREVIOUS`, `INTERPOLATED`, `TELEPORTED` and `TICK`.
`SPACE_NONE` is `UINT32_MAX`, the index of nothing.
