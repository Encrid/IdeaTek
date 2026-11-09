# src/kernel/states_bridge/states_chain.h

This header gives the types of the chain of a scoped entity, and the prototype of its verdict.

## The types

`chain_link_t` is 1 state machine of the chain, as `clear` read it:

| Field | Value |
| --- | --- |
| `has_state` | It is an entity of the world, and it carries `ideatek:State`. |
| `has_request` | It carries `ideatek:StateRequest`. |
| `scoped` | It carries `ideatek:StateScoped`: the chain goes on. |
| `state` | Its value of `ideatek:State`. |
| `value` | The value that ties the entity below it to this state machine. |

`chain_verdict_t` is the result of the verdict: `CHAIN_STAYS`, `CHAIN_LEAVES` or `CHAIN_DEEP`.
