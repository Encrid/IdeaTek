# src/foundation/math/generic_vec3_shapes.h

This generic text makes a new vector of 3 from 1 or 2 vectors, and checks a vector.
`src/foundation/math/math_vec3.h` includes it after `src/foundation/math/generic_vec3_products.h`, because it calls `vec3_get_length`.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `vec3_get_unit` | `vec3f_get_unit` | `vec3d_get_unit` |
| `vec3_get_blend` | `vec3f_get_blend` | `vec3d_get_blend` |
| `vec3_get_least` | `vec3f_get_least` | `vec3d_get_least` |
| `vec3_get_most` | `vec3f_get_most` | `vec3d_get_most` |
| `vec3_is_finite` | `vec3f_is_finite` | `vec3d_is_finite` |

It also uses `real_t` and `vec3_t`.

## The unit

`vec3_get_unit` divides each part by the length. A division is exact to the last bit, and a product with 1 divided by the length is not.
With a length of 0, the function gives the vector with no change, so it divides by no 0.

## The blend

`vec3_get_blend` has the formula of `real_blend`, on the 3 parts: `docs/intern/foundation/math/generic_scalar_range.h.md`, *The blend*.

## The least and the most

Each part is 1 comparison. A comparison with a NaN is false, so the part of the second vector comes out.

## The branches

The cases of Reflection for this file:

| Case | Result |
| --- | --- |
| A vector with a length above 0. | Each part divided by the length. |
| A vector of length 0. | The same vector. |
| Each part of the first vector is below the part of the second, or above it. | The least and the most of each part, for the 2 orders. |
| Each part is finite. | `true`. |
| An infinity or a NaN in `x`, in `y` or in `z`. | `false`, with 1 case for each part. |

## Functions

| Function | Result |
| --- | --- |
| `vec3_get_unit` | Gives the vector at the length 1, or the same vector for a length of 0. |
| `vec3_get_blend` | Gives the blend of 2 vectors. |
| `vec3_get_least` | Gives the smaller number of each part. |
| `vec3_get_most` | Gives the larger number of each part. |
| `vec3_is_finite` | Tells if each part is finite. |
