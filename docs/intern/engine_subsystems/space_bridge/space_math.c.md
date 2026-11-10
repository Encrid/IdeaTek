# src/engine_subsystems/space_bridge/space_math.c

This file computes the places of the space part. It is pure, and the program `space_math` checks it.

`get_matrix` and `compose` follow `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*.
With a bit of the rotation or of the scale in `absolute`, `compose` splits the basis of the parent. It keeps only the parts that the bits do not name.
The origin of the child then uses that basis too.
`multiply_basis` multiplies 2 bases of 3 columns.

## Functions

| Function | Result |
| --- | --- |
| `get_matrix` | The place of a transform. |
| `multiply_basis` | The product of 2 bases. |
| `mask_basis` | The basis of the parent without the parts that `absolute` names. |
| `compose` | The place of a child. |
