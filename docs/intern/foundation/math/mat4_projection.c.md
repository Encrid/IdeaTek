# src/foundation/math/mat4_projection.c

This file makes the 2 perspective projections of `src/foundation/math/math_mat4.h`.

## The width of the calculation

Each number of the matrix is computed in 64 bits from the fields of 32 bits. It is rounded to 32 bits 1 time, at the end.
A camera gets 1 projection in a frame, so the cost of 64 bits does not count. The result is nearer to the exact value.

## The tangents

`get_tangent` is the sine of an angle divided by its cosine, with the 2 functions of the part.
`mat4f_make_perspective` takes the tangent of a half of `fov` as `up`, and `up` times `aspect` as `right`. The 2 other sides are their opposites.
`mat4f_make_frustum` takes the tangent of each of its 4 angles.

## The matrix

`make_shape` writes these numbers. `w` and `h` are the width and the height of the view in tangents:

| Number | Value |
| --- | --- |
| Row 0, column 0 | `2 / w` |
| Row 0, column 2 | `(right + left) / w` |
| Row 1, column 1 | `2 / h`, negated with `y_down` |
| Row 1, column 2 | `(up + down) / h`, negated with `y_down` |
| Row 3, column 2 | -1: the camera looks along -Z, so `w` of the clip space is the distance in front of the camera. |

`fill_depth` writes row 2. `n` and `f` are the 2 distances:

| Depth | Column 2 | Column 3 |
| --- | --- | --- |
| From 0 at the near plane to 1 at the far plane | `-f / (f - n)` | `-(n * f) / (f - n)` |
| The same, with no far plane | -1 | `-n` |
| Reverse | `n / (f - n)` | `(n * f) / (f - n)` |
| Reverse, with no far plane | 0 | `n` |

A reverse depth with no far plane has 2 numbers in row 2 that need no division. It keeps the most bits of the depth far from the camera.

## The branches

The cases of Reflection for this file:

| Case | Result |
| --- | --- |
| The 4 rows of the table of the depth. | Row 2 of each row. |
| `y_down` true and false. | The sign of row 1. |

## Functions

| Function | Result |
| --- | --- |
| `get_tangent` | Gives the tangent of an angle. |
| `fill_depth` | Writes the 2 numbers of the depth. |
| `make_shape` | Gives the matrix of a shape. |
| `mat4f_make_perspective` | Gives the projection of a lens. |
| `mat4f_make_frustum` | Gives the projection of a frustum of 4 angles. |
