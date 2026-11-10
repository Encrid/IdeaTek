# src/engine_subsystems/space_bridge/space_set.c

This file gives a set of entities in a buffer of the scratch arena. It is pure: the program `space_set` of the tests calls it.

## The set

The set is an open table of `uint64_t` ids, with twice as many slots as its room, so a walk stays short.
An id of 0 marks a free slot: no entity has the id 0.
`set_put` gives `false` when the set is full. Then the caller places the rest without the test of the ancestors.

## Functions

| Function | Result |
| --- | --- |
| `set_start` | An empty set in a buffer of a given room. |
| `set_put` | Adds an id. `false` when the set is full. |
| `set_has` | `true` when the set holds the id. |
