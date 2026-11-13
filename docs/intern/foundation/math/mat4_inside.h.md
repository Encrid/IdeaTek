# src/foundation/math/mat4_inside.h

This header is for the 4 sources of the matrix of 4 by 4. It stays out of the release of the part.

## mat4_shape_t

`mat4_shape_t` is a perspective as 4 tangents and 2 distances, in 64 bits, with the 2 flags of the clip space.
`mat4f_make_perspective` and `mat4f_make_frustum` both fill it, and 1 function makes the matrix from it.
Thus the 2 kinds of perspective share 1 formula.

| Field | Content |
| --- | --- |
| `left`, `right`, `down`, `up` | The tangent of the angle of each side. `left` and `down` are negative for a centered view. |
| `near_plane`, `far_plane` | The 2 distances. `far_plane` can be an infinity. |
| `reverse` | The flag of a reverse depth. |
| `y_down` | +Y of the clip space points down. |

## Functions

| Function | Result |
| --- | --- |
| `mat4_make_zero` | The prototype. The function is in `src/foundation/math/mat4_flat.c`. |
