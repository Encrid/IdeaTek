# src/foundation/math/generic_mat3_rotation.h

This generic text goes from a matrix of a rotation to its quaternion, and from a quaternion to its matrix.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `mat3_get_part` | `mat3f_get_part` | `mat3d_get_part` |
| `mat3_get_rotation` | `mat3f_get_rotation` | `mat3d_get_rotation` |
| `mat3_make_rotation` | `mat3f_make_rotation` | `mat3d_make_rotation` |
| `mat3_make_basis` | `mat3f_make_basis` | `mat3d_make_basis` |

## From a matrix to a quaternion

The square of each part of the quaternion is a sum of 1 and the 3 numbers of the diagonal, with signs. For `x` it is `1 + a - b - c`.
`mat3_get_part` takes a half of the root of that sum, and gives it the sign of a difference of 2 numbers beside the diagonal.
A sum below 0, which the rounding can give, counts as 0.

This form has no test of which part is the largest. So it has no branch, and a small change of the matrix gives a small change of the result.
`mat3_get_rotation` ends with `quat_get_unit`, because the 4 roots do not give the length 1 exactly.
The space module used this form before the math part existed, and the part keeps it.

## From a quaternion to a matrix

`mat3_make_rotation` writes the 9 numbers from the products of 2 parts of the quaternion. The quaternion must have the length 1.
`mat3_make_basis` multiplies each column of that matrix by the scale of its axis.

## Functions

| Function | Result |
| --- | --- |
| `mat3_get_part` | Gives 1 part of the quaternion from its sum and its sign. |
| `mat3_get_rotation` | Gives the quaternion of a matrix of a rotation. |
| `mat3_make_rotation` | Gives the 9 numbers of the rotation of a quaternion. |
| `mat3_make_basis` | Gives the matrix of a rotation and a scale. |
