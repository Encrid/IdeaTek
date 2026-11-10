# src/engine_subsystems/space_bridge/space_set.c

This file gives a set of entities in a buffer. It is pure: the program `space_set` of the tests calls it.

The set is an open table of `uint64_t` ids, with twice as many slots as its room, so a walk of the slots stays short.
An id of 0 marks a free slot, so the set holds no id 0. A set of no room holds nothing.

## Functions

| Function | Result |
| --- | --- |
| `get_home` | The first slot of the walk of an id. |
| `start_set` | An empty set in a buffer. |
| `set_put` | Adds an id. `false` when the set is full. |
| `set_has` | `true` when the set holds the id. |
