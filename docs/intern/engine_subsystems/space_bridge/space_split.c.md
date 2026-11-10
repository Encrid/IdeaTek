# src/engine_subsystems/space_bridge/space_split.c

This file gives the rotation and the scale of a basis. It reads only its arguments. The cases of the split in `space_math` check it.

`split_basis` follows the 4 steps of `docs/extern/engine_subsystems/ideatek_space.h.md`, *The split of a basis*, in their order, and with no branch.
A column of length 0 takes the cross of the others, made of length 1. Two columns of length 0 give no rotation of use.

## Functions

| Function | Result |
| --- | --- |
| `get_length` | The length of a column. |
| `remove_part` | Takes out of a column its part along an axis. |
| `cross` | The cross product of 2 columns. |
| `make_unit` | Makes a column of length 1, or fills a column of length 0. |
| `split_basis` | The rotation and the scale of a basis. |
