# src/foundation/math/math_mat3.h

The matrices of 3 by 3 are in this header.
`docs/usage/foundation/math.md`, *The matrices of 3 by 3*, lists what each function gives.

## The types

`mat3f_t` and `mat3d_t` are matrix types of clang with 3 rows and 3 columns. A source that includes this header needs the flag `-fenable-matrix`.
`m[row][column]` is 1 number. The compiler keeps the 9 numbers column by column, with no room between them.

## The product of 2 matrices

The compiler has an operator `*` for 2 matrices. The part does not use it, and has `mat3_get_product` in its place.
No document fixes the order in which the compiler adds the 3 products of each number. So a new version of the compiler can change the last bits.

## The widths

Each width gets 4 generic texts, included from the top of this table to its end:

| Generic text | Content |
| --- | --- |
| `src/foundation/math/generic_mat3_parts.h` | The columns, the transpose, and the 9 plain numbers. |
| `src/foundation/math/generic_mat3_products.h` | The products, the determinant and the inverse. |
| `src/foundation/math/generic_mat3_rotation.h` | From a matrix to a quaternion, and back. |
| `src/foundation/math/generic_mat3_split.h` | The split of a basis. |

`real_max` and `real_copysign` are `fmaxf` and `copysignf` in 32 bits, and `fmax` and `copysign` in 64 bits.
Each of them selects a number or a sign and computes nothing. Thus their result does not depend on the system.

A program wrote the 2 lists of `#define` and `#undef` from 1 list of names. Thus the 2 widths have the same names.

## Functions

| Function | Result |
| --- | --- |
| The functions of the 2 widths | The 4 generic texts of the table. |
