# src/foundation/math/generic_vec3_products.h

The products and the lengths of a vector of 3 are in this generic text.
`docs/intern/foundation/math/math_vec3.h.md`, *The widths*, tells how the 2 widths come from it.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `real_t` | `float` | `double` |
| `real_sqrt` | `sqrtf` | `sqrt` |
| `vec3_t` | `vec3f_t` | `vec3d_t` |
| `vec3_get_dot` | `vec3f_get_dot` | `vec3d_get_dot` |
| `vec3_get_cross` | `vec3f_get_cross` | `vec3d_get_cross` |
| `vec3_get_square` | `vec3f_get_square` | `vec3d_get_square` |
| `vec3_get_length` | `vec3f_get_length` | `vec3d_get_length` |
| `vec3_get_distance` | `vec3f_get_distance` | `vec3d_get_distance` |

## The order of the operations

The result of a sum of 3 numbers depends on its order, so each function fixes the order:

- `vec3_get_dot` computes the 3 products, adds the product of `x` and the product of `y`, and then adds the product of `z`.
- Each part of `vec3_get_cross` is 1 product minus 1 other product, in the order of the formula of a right-handed cross product.
- `vec3_get_square` is `vec3_get_dot` of a vector with itself, so the 2 functions give the same bits.

No product joins a sum: `docs/intern/foundation/math/math_scalar.h.md`, *The fusion*.

## The limits

`vec3_get_length` takes the root of the square. Thus it gives 0 for a vector so short that its square is 0.
It gives an infinity for a vector so long that its square is an infinity.
The readme of the part gives these 2 lengths for each width.

## Functions

| Function | Result |
| --- | --- |
| `vec3_get_dot` | Gives the dot product. |
| `vec3_get_cross` | Gives the cross product. |
| `vec3_get_square` | Gives the square of the length. |
| `vec3_get_length` | Gives the length. |
| `vec3_get_distance` | Gives the length of the second point minus the first point. |
