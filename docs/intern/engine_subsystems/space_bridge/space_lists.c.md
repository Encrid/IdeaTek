# src/engine_subsystems/space_bridge/space_lists.c

This file writes the list of the children of 1 parent as a whole.

1. `gather_children` copies the old list into the buffer, then adds the entities of the list of the observer. It keeps only the entities whose `ideatek:Parent` names the parent, once each.
2. `sort_children` sorts the buffer by `order`, then by id.
3. `write_list` attaches `ideatek:Sibling` to each child, and `ideatek:Children` to the parent. An empty list detaches `ideatek:Children`.

A destroyed child is a husk that keeps its `ideatek:Sibling`, so the old list still goes through it. It has no `ideatek:Parent`, so it drops out.

## Functions

| Function | Result |
| --- | --- |
| `add_child` | Adds 1 child to the buffer when it belongs there. |
| `gather_children` | The children of the new list, in the buffer. |
| `is_before` | `true` when a child comes before another one. |
| `sort_children` | The buffer in the order of the contract. |
| `write_list` | Attaches the links and the list. |
