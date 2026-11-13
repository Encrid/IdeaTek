# src/foundation/math/mat4_inverse.c

This file inverts a matrix of 4 by 4, for `mat4f_get_inverse` of `src/foundation/math/math_mat4.h`.

## The method

The determinant of a matrix of 4 by 4 is a sum of 6 products of 2 small determinants.
In each product, one comes from the rows 0 and 1, and the other from the rows 2 and 3.
`get_minors` gives the 6 small determinants of 2 rows that are next to each other. Each one uses 2 columns, in this order of the pairs:

| Index | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| Columns | 0, 1 | 0, 2 | 0, 3 | 1, 2 | 1, 3 | 2, 3 |

`s` holds those of the rows 0 and 1, and `c` holds those of the rows 2 and 3. The determinant is:

```text
s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]
```

Each number of the inverse is a sum of 3 products of a number of the matrix and a small determinant, divided by the determinant.
`fill_low` writes the columns 0 and 1 of the inverse from `c`, and `fill_high` writes the columns 2 and 3 from `s`.

## The check

A case compares the inverse of a full matrix with a calculation in exact fractions. The matrix has a determinant of 1, so its inverse is exact in 32 bits.

## Functions

| Function | Result |
| --- | --- |
| `get_minors` | Writes the 6 small determinants of the rows `first` and `first + 1`. |
| `fill_low` | Writes the columns 0 and 1 of the inverse. |
| `fill_high` | Writes the columns 2 and 3 of the inverse. |
| `mat4f_get_inverse` | Writes the inverse and gives `true`. Gives `false` with no write when the determinant is 0. |
