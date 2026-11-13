# src/foundation/math/math_quat.h

This header gives the quaternions to a caller of the math part.
Their results are in `docs/usage/foundation/math.md`, *The quaternions*.

## The types

`quatf_t` and `quatd_t` are vector types of clang with 4 parts: the axis part in `x`, `y` and `z`, and the real part in `w`.
The operators of a vector work on them part by part. The product of 2 quaternions is a function, and not `*`.

## The widths

The header includes 3 generic texts for each width, in this order, because each one calls the one before:

| Generic text | Content |
| --- | --- |
| `src/foundation/math/generic_quat_products.h` | The dot product, the length, the product, the conjugate, the unit. |
| `src/foundation/math/generic_quat_turns.h` | The turn of a vector, the rotation of an axis and an angle, and the way back. |
| `src/foundation/math/generic_quat_checks.h` | The 2 checks and the blend. |

The generic texts call functions of 1 number and of a vector of 3 by generic names too.
So the header also gives the names of the width to `real_sqrt`, `real_abs`, the 3 functions of an angle, `vec3_get_cross` and `vec3_get_length`.

## Functions

| Function | Result |
| --- | --- |
| The functions of the 2 widths | The 3 generic texts of the table. |
| `quatd_get_narrow` | Converts the 4 parts to 32 bits, as `vec3d_get_narrow` does. |
| `quatf_get_wide` | Gives the quaternion in 64 bits, with no loss. |
| `quatd_step` | The prototype. The function is in `src/foundation/math/quat_step.c`. |
