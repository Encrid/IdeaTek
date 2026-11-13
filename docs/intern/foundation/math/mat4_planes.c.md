# src/foundation/math/mat4_planes.c

This file holds `mat4f_get_planes` of `src/foundation/math/math_mat4.h`, which reads the sides of the view from a matrix.

## The method

A point is in the view when its clip position passes the 6 tests of the clip space. Each test is a row of the matrix, or a sum or a difference of 2 rows.
So each plane comes from the rows with no inverse of the matrix. The method is that of Gribb and Hartmann.

| Index | Test | Plane before the division |
| --- | --- | --- |
| 0 | `x >= -w` | Row 3 plus row 0 |
| 1 | `x <= w` | Row 3 minus row 0 |
| 2 | `y >= -w` | Row 3 plus row 1 |
| 3 | `y <= w` | Row 3 minus row 1 |
| 4 | `z >= 0` | Row 2 |
| 5 | `z <= w` | Row 3 minus row 2 |

`make_plane` divides the 4 numbers of a plane by the length of its first 3 numbers. Then the first 3 numbers are a normal of length 1.

## A plane that does not exist

A projection with no far plane gives a row whose first 3 numbers are 0. Such a plane has no normal.
`make_plane` then gives `{ 0, 0, 0, 1 }`. The dot product of that plane and a point is 1, so each point passes the test with no branch in the caller.

## Functions

| Function | Result |
| --- | --- |
| `get_row` | Gives 1 row of the matrix as a vector of 4. |
| `make_plane` | Gives a plane with a normal of length 1, or the plane that holds each point. |
| `mat4f_get_planes` | Writes the 6 planes at `planes`. |
