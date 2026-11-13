# src/engine_subsystems/space_bridge/space_split.c

This file gives the rotation and the scale of a basis. It reads only its arguments. The cases of the split in `space_math` check it.

`split_basis` calls `mat3f_split` of the math part, which follows the 4 steps of `docs/usage/foundation/math.md`, *The split of a basis*.
`docs/intern/foundation/math/generic_mat3_split.h.md`, *The frame*, tells what a column of length 0 gets.

## Functions

| Function | Result |
| --- | --- |
| `split_basis` | The rotation and the scale of a basis. |
