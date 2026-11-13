# src/foundation/math/generic_mat3_products.h

This generic text holds the products of a matrix of 3 by 3, its determinant and its inverse.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `mat3_turn` | `mat3f_turn` | `mat3d_turn` |
| `mat3_get_product` | `mat3f_get_product` | `mat3d_get_product` |
| `mat3_get_determinant` | `mat3f_get_determinant` | `mat3d_get_determinant` |
| `mat3_get_inverse` | `mat3f_get_inverse` | `mat3d_get_inverse` |

## The order of the sums

- Each part of `mat3_turn` is the sum of 3 products of 1 row, added in the order of the columns 0, 1, 2.
- Each number of `mat3_get_product` is the sum of 3 products of a row of `a` and a column of `b`. It adds them in the order of the columns of `a`.

## The determinant and the inverse

Let `x`, `y` and `z` be the columns of the matrix. The determinant is the dot product of `x` and the cross product of `y` and `z`.
The 3 rows of the inverse are 3 cross products, each divided by the determinant:

| Row of the inverse | Cross product |
| --- | --- |
| 0 | `y` and `z` |
| 1 | `z` and `x` |
| 2 | `x` and `y` |

`mat3_get_inverse` computes the determinant from the first of these cross products, so it gives the bits of `mat3_get_determinant`.
It divides each number by the determinant. A product with 1 divided by the determinant has 1 more rounding.
A determinant of 0 ends the function with `false`, before a write of the result.

## Functions

| Function | Result |
| --- | --- |
| `mat3_turn` | Gives the matrix times a vector. |
| `mat3_get_product` | Gives the matrix `a` times the matrix `b`. |
| `mat3_get_determinant` | Gives the determinant. |
| `mat3_get_inverse` | Writes the inverse and gives `true`, or gives `false`. |
