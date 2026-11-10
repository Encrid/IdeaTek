# src/engine_subsystems/space_bridge/space_rotation.c

This file turns a quaternion into a basis, and an orthonormal basis into a quaternion. No state goes in or out of it, and the cases of `space_math` reach it.

`get_rotation` takes each part of the quaternion from the diagonal of the basis, and its sign from the other values, with no branch. Then it makes the quaternion of length 1.
`get_rotation_matrix` makes the quaternion of length 1 first, so a rotation that is not of length 1 still turns.

## Functions

| Function | Result |
| --- | --- |
| `get_part` | 1 part of the quaternion, with its sign. |
| `get_rotation` | The quaternion of a basis. |
| `get_rotation_matrix` | The basis of a quaternion. |
