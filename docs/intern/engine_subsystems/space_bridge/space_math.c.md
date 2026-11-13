# src/engine_subsystems/space_bridge/space_math.c

This file computes the places of the space part. It is pure, and the program `space_math` checks it.

`get_matrix` and `compose` follow `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*.
With a bit of the rotation or of the scale in `absolute`, `compose` splits the basis of the parent. It keeps only the parts that the bits do not name.
The origin of the child then uses that basis too.

The file holds the rules of the contract, and the math part of IdeaTek does each calculation: `docs/usage/foundation/math.md`.

| Function | Calculation of the math part |
| --- | --- |
| `get_matrix` | `mat3f_make_basis` of the rotation at the length 1 and the scale. |
| `multiply_basis` | `mat3f_get_product`. |
| `compose` | `affine_move` for the origin of the child, in 64 bits. |

`compose` reads the origin of the child before it writes `out`.

## Functions

| Function | Result |
| --- | --- |
| `get_matrix` | The place of a transform. |
| `multiply_basis` | The product of 2 bases. |
| `mask_basis` | The basis of the parent without the parts that `absolute` names. |
| `compose` | The place of a child. |
