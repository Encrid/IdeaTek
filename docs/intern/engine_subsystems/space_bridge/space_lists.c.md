# src/engine_subsystems/space_bridge/space_lists.c

This file writes the list of the children of 1 parent: `ideatek:Children` of the parent and `ideatek:Sibling` of each child.

## The list

The module writes a list again as a whole. Bridge applies the attaches at the next barrier, so 2 changes of 1 list in 1 call would read the same old list.

1. `gather` copies the old list into a buffer of the scratch arena, from the first child to the last.
2. The caller takes out the children that leave, and adds the children that arrive.
3. `sort_children` sorts the buffer by `order` of `ideatek:Parent`, then by id.
4. `write_list` attaches `ideatek:Sibling` to each child with `prev`, `next` and `parent`, and `ideatek:Children` with `first`, `last` and `count`.

A list with no child detaches `ideatek:Children`. A buffer that the scratch arena cannot hold leaves the list as it was, and the log gets 1 line.

## Functions

| Function | Result |
| --- | --- |
| `gather` | The children of the old list, in the buffer. |
| `sort_children` | The buffer in the order of the contract. |
| `write_list` | Attaches the links and the list. |
