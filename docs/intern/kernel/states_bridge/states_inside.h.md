# src/kernel/states_bridge/states_inside.h

This header holds the slots of the 3 systems and of the 5 types of the states module, and what its sources share.

## The state

`states_module` is the global variable of the module: Bridge gives no argument that carries it between 2 calls. G4 of the style excepts it, as in each module of IdeaTek.

| Field | Value |
| --- | --- |
| `forget`, `clear`, `apply` | The slots of the 3 systems, as `add_system` gave them. The systems read the queries of the 3 slots. |

The module keeps nothing else: the queries of Bridge hold the state machines, the requests and the scoped entities.

## The names

The declarations give the slots in this order: `ENTITY`, `STATE`, `REQUEST`, `CHANGE` and `SCOPED`.
`STATES_TYPE` declares a type of the contract. `STATES_DEPTH` is 8: the most state machines in a chain of scoped entities.
`docs/intern/kernel/states_bridge/states_chain.h.md` gives the types of the chain.
