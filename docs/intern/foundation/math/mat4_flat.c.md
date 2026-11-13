# src/foundation/math/mat4_flat.c

This file makes the matrices of 4 by 4 whose last row is `0 0 0 1`: the orthographic projection, the view and the model.

## The orthographic projection

`mat4f_make_orthographic` maps the box to the clip space. `w`, `h` and `d` are the width, the height and the depth of the box:

| Number | Value |
| --- | --- |
| Row 0 | `2 / w` in column 0, and `-(right + left) / w` in column 3 |
| Row 1 | `2 / h` in column 1, and `-(top + bottom) / h` in column 3, both negated with `y_down` |
| Row 2 | `-1 / d` and `-near_plane / d`. With `reverse`: `1 / d` and `far_plane / d` |

The numbers are computed in 64 bits, as `docs/intern/foundation/math/mat4_projection.c.md`, *The width of the calculation*, tells.

## The view and the model

`make_place` puts a basis of 3 by 3 and an origin into a matrix of 4 by 4. It rounds the origin to 32 bits.

- `mat4f_make_view` is the inverse of the place of the camera. Its basis is the matrix of the conjugate of the rotation.
  Its origin is the origin of the camera minus `origin`, turned by the conjugate, with the opposite sign.
- `mat4f_make_model` keeps the basis of the place, and its origin is the origin of the place minus `origin`.

Each subtraction of `origin` is in 64 bits. The difference is small when `origin` is near the camera, and a small number keeps its bits in 32 bits.

## Functions

| Function | Result |
| --- | --- |
| `mat4_make_zero` | Gives a matrix of 16 zeros. |
| `make_place` | Gives the matrix of a basis and an origin. |
| `mat4f_make_orthographic` | Gives the projection of a box. |
| `mat4f_make_view` | Gives the matrix from the space of `origin` to the space of the camera. |
| `mat4f_make_model` | Gives the matrix from a place to the space of `origin`. |
