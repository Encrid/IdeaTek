# src/engine_subsystems/space_bridge/space_inside.h

This header holds the slots of the space module, its types, and the functions that its sources share.

## The state

`space_module` holds the slots between 2 calls, since no argument of a call carries them. It is the 1 global of the module, which G4 of the style excepts.

| Field | Value |
| --- | --- |
| `lists` | The slot of the list of each of the 5 observers. |
| `step` | The slot of the query of the system `step`. |
| `tick` | The `tick` of `ideatek:Tick` at the last run of `step`. |

The places and the lists live in the components of the space part, not in the module.

## The names

The slots come in the order of the declarations: the 7 types of the fields from `ENTITY`, then `TRANSFORM` to `TICK`.
`LIST_LINK` to `LIST_GONE` name the observers, and `ANCESTOR_ROOT`, `ANCESTOR_CHANGED` and `ANCESTOR_CYCLE` name the results of `get_ancestor_state`.
`STACK_ROOM` is 1024, the most items of the stack of a walk.

## The types

| Type | Value |
| --- | --- |
| `space_item_t` | 1 entity of a walk, with the place of its parent and the mark of a teleport. |
| `space_stack_t` | The stack of a walk in the scratch arena, with its top, its room and the mark of a full stack. |
| `space_ids_t` | A buffer of ids in the scratch arena, with its count and its room. |

`SPACE_FIND` reads a value of an entity as a typed pointer, or gives `nullptr`. `SPACE_EDIT` gives the same pointer for a write in place.
