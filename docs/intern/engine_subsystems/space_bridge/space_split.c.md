# src/engine_subsystems/space_bridge/space_split.c

This file gives the rotation and the scale of a basis. It is pure: the program `space_split` of the tests calls it.

## The split

`split_basis` follows the 4 steps of `docs/extern/engine_subsystems/ideatek_space.h.md`, *The split of a basis*, in their order.
Its only freedom is the order of the floating point operations, which the program `space_split` fixes by its cases.

`rotation_of` turns the 3 orthonormal columns into a quaternion of length 1.

## Functions

| Function | Result |
| --- | --- |
| `split_basis` | The rotation and the scale of a basis. |
| `rotation_of` | The quaternion of an orthonormal basis. |
