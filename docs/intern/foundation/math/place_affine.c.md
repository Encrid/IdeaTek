# src/foundation/math/place_affine.c

This file composes and inverts an affine place of `src/foundation/math/math_place.h`.

## The compose

`affine_compose` multiplies the 2 bases in 32 bits with `mat3f_get_product`.
It moves the origin of `local` by `parent` with `affine_move`, in 64 bits.
The origin is computed first, so `out` can be `parent`.

## The inverse

`affine_invert` inverts the basis in 32 bits. If the basis has no inverse, the function gives `false` and leaves `out` as it is.
The origin of the result is the inverse basis, widened to 64 bits, times the origin of the place, with the opposite sign.

## The branches

The cases of Reflection for the inverse:

| Case | Result |
| --- | --- |
| A basis with a determinant that is not 0. | `true`, and `out` is the opposite place. |
| A basis with a determinant of 0. | `false`, and `out` keeps its value. |

## Functions

| Function | Result |
| --- | --- |
| `affine_compose` | Chains 2 affine places into `out`. |
| `affine_invert` | Writes the place that undoes `place` and gives `true`, or gives `false`. |
