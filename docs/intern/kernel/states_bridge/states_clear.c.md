# src/kernel/states_bridge/states_clear.c

This file runs the system `clear` in the first phase: it destroys each scoped entity that leaves.

## The round

`clear` walks the query of `ideatek:StateScoped`. For each entity, it reads the chain of its state machines, then takes the verdict of the chain: `docs/intern/kernel/states_bridge/states_chain.c.md`.

1. The first link is the state machine that `ideatek:StateScoped` of the entity names, with the value of the entity.
2. `read_link` fills the link from the entity index. A dead state machine has no component there. `has_state` and `state` come from `ideatek:State`, `has_request` from `ideatek:StateRequest`, and `scoped` from `ideatek:StateScoped` of the state machine.
3. When the link is scoped, the next link is the state machine that its `ideatek:StateScoped` names, up to `STATES_DEPTH` links.
4. `CHAIN_LEAVES` destroys the entity. `CHAIN_DEEP` keeps the entity and writes the line of the depth: `docs/usage/kernel/states_bridge.md`, *The messages*.

The destroys apply at the barrier of the phase, with the writes of `apply`. A phase sees the old value with its entities, or the new value without them.
A sub state is a scoped entity with `ideatek:State`: its own scoped entities find it in their chain, and go at the same barrier.
A state machine destroyed by another module is a dead link: its scoped entities go in the first phase of the next round.

## The branches

| Case | Result |
| --- | --- |
| The chain leaves. | The entity is destroyed. |
| The chain stays. | Nothing. |
| The chain is deeper than `STATES_DEPTH`. | 1 line of the log. |

## Functions

| Function | Result |
| --- | --- |
| `read_link` | Fills a link from a state machine and the value that ties it. |
| `read_chain` | Reads the links of the chain of a scoped entity. Gives their count. |
| `judge` | Reads the chain of 1 row, and destroys the entity or writes the line. |
| `clear` | Walks the query. Gives 0. |
