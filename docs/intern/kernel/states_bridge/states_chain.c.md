# src/kernel/states_bridge/states_chain.c

This file gives the verdict of a chain of state machines: the scoped entity leaves, stays, or the chain is too deep.
It is pure: the program `states_chain` of the tests calls it with chains that it builds.

## The verdict

A scoped entity leaves when a link of its chain leaves:

- the state machine is dead, or carries no `ideatek:State`;
- the state machine has a request, and its value is the value that ties the link.

A link that stays and is not scoped ends the chain: the entity stays.
A chain of `STATES_DEPTH` links that all stay and are all scoped is too deep.

```pseudo
FUNCTION chain_verdict(links : U64, count : U32) RETURNS chain_verdict_t
   FOR i <- 0 TO count - 1
      IF link_leaves(links[i]) THEN
         RETURN CHAIN_LEAVES
      ENDIF
      IF NOT links[i].scoped THEN
         RETURN CHAIN_STAYS
      ENDIF
   NEXT i
   RETURN CHAIN_DEEP
ENDFUNCTION
```

`link_leaves` is arithmetic on the 4 flags and the 2 values, with no branch.

## Functions

| Function | Result |
| --- | --- |
| `link_leaves` | `true` when the link leaves. |
| `chain_verdict` | The verdict of the chain. |
