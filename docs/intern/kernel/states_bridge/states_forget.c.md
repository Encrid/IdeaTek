# src/kernel/states_bridge/states_forget.c

This file runs the system `forget` in the first phase.

## The round

`forget` walks the query of `ideatek:StateChange`, and detaches it from each state machine that carries it.
So a change lives from its attach by `apply` to the barrier of `forget` in the round after.
`apply` runs after `forget` in the same phase. A barrier applies the changes in the order of the systems, so the new change of the round stays.

## Functions

| Function | Result |
| --- | --- |
| `drop_change` | Detaches `ideatek:StateChange` of 1 row. |
| `forget` | Walks the query and detaches each change. Gives 0. |
