# src/foundation/math/math_mat4.h

This header gives the matrix of 4 by 4 in 32 bits, for a renderer: a projection, a view and a model.
`docs/usage/foundation/math.md`, *The matrices of 4 by 4*, tells the result of each function.

## The types

`mat4f_t` is a matrix type of clang with 4 rows and 4 columns: 16 numbers of 32 bits, column by column.
The 3 structures hold what a projection needs:

| Type | Shape |
| --- | --- |
| `mat4f_lens_t` | A perspective with 1 vertical angle and an aspect. |
| `mat4f_frustum_t` | A perspective with 4 angles. |
| `mat4f_box_t` | An orthographic box. |

The 2 distances have the names `near_plane` and `far_plane`. The names `near` and `far` are macros in the headers of Windows.

## The small functions

The header holds the 5 functions that are a few lines.
They are the product with a vector, the product of 2 matrices, the transpose, and the 2 moves of the 16 plain numbers.
They add their products in the order of the columns, as `docs/intern/foundation/math/generic_mat3_products.h.md`, *The order of the sums*, tells for 3 by 3.
The other functions are in the archive, and the header declares them.

## Functions

| Function | Result |
| --- | --- |
| `mat4f_turn` | Gives the matrix times a vector of 4. |
| `mat4f_get_product` | Multiplies 2 matrices of 4 by 4. |
| `mat4f_get_transpose` | Swaps the rows and the columns. |
| `mat4f_load`, `mat4f_store` | Move the 16 numbers from and to plain numbers, column by column. |
| `mat4f_make_perspective`, `mat4f_make_frustum` | The prototypes. The functions are in `src/foundation/math/mat4_projection.c`. |
| `mat4f_make_orthographic`, `mat4f_make_view`, `mat4f_make_model` | The prototypes. The functions are in `src/foundation/math/mat4_flat.c`. |
| `mat4f_get_inverse` | The prototype. The function is in `src/foundation/math/mat4_inverse.c`. |
| `mat4f_get_planes` | The prototype. The function is in `src/foundation/math/mat4_planes.c`. |
