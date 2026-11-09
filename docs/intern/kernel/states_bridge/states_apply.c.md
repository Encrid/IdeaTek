# src/kernel/states_bridge/states_apply.c

This file runs the system `apply` in the first phase: it applies the request of each state machine.

## The round

`apply` walks the query of `ideatek:StateRequest`, with `ideatek:State` as an optional term.
For each row:

1. It detaches `ideatek:StateRequest`. So 1 request at most reaches each state machine in a round: the last attach of the phase.
2. With no cell of `ideatek:State`, the entity is no state machine: it writes `no state machine for <entity>`, and stops.
3. It attaches `ideatek:State` with the value of the request, and `ideatek:StateChange` with the old value and the new value.

A request with the current value gives a change from the value to itself. `clear` destroyed the scoped entities of that value in the same phase.

## The branches

| Case | Result |
| --- | --- |
| The entity carries `ideatek:State`. | The state and the change. |
| The entity carries no `ideatek:State`. | 1 line of the log. |

## Functions

| Function | Result |
| --- | --- |
| `change` | Writes the state and the change of 1 state machine. |
| `take_request` | Detaches the request of 1 row, and applies it or writes the line. |
| `apply` | Walks the query. Gives 0. |
