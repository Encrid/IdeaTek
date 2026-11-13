# src/foundation/math/math_place.h

This header gives the 2 places of the math part: a rigid place and an affine place.
`docs/usage/foundation/math.md`, *The places*, lists their functions.

## The types

| Type | Fields | For |
| --- | --- | --- |
| `rigid_t` | `rotation`, a `quatd_t`, and `origin`, a `vec3d_t` | A body of a simulation: no scale, all in 64 bits. |
| `affine_t` | `basis`, a `mat3f_t`, and `origin`, a `vec3d_t` | A place in the world as the space contract holds it: a basis in 32 bits and an origin in 64 bits. |

They are structures, and a function takes them through pointers. The output of a function can be 1 of its inputs.

## The 2 widths of an affine place

A basis in 32 bits is exact enough for a direction and a size. An origin far from the center of the world needs 64 bits.
So `affine_move` widens the basis with `mat3f_get_wide`, which loses nothing, and multiplies the point in 64 bits.
A product in 32 bits would cut a far point to 24 bits before the sum with the origin.

## Functions

| Function | Result |
| --- | --- |
| `mat3f_get_wide` | Gives a matrix of 32 bits in 64 bits. |
| `rigid_move` | Gives a point of a rigid place in the outer space: the point turned, plus the origin. |
| `rigid_move_back` | Gives a point of the outer space in the rigid place. |
| `affine_move` | Gives a point of an affine place in the outer space. |
| `rigid_compose`, `rigid_invert` | The prototypes. The functions are in `src/foundation/math/place_rigid.c`. |
| `affine_compose`, `affine_invert` | The prototypes. The functions are in `src/foundation/math/place_affine.c`. |
