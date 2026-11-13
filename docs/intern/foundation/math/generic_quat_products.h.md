# src/foundation/math/generic_quat_products.h

The products of a quaternion are in this generic text: `docs/intern/foundation/math/math_quat.h.md`, *The widths*.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `quat_t` | `quatf_t` | `quatd_t` |
| `quat_get_dot` | `quatf_get_dot` | `quatd_get_dot` |
| `quat_get_length` | `quatf_get_length` | `quatd_get_length` |
| `quat_get_product` | `quatf_get_product` | `quatd_get_product` |
| `quat_get_conjugate` | `quatf_get_conjugate` | `quatd_get_conjugate` |
| `quat_get_unit` | `quatf_get_unit` | `quatd_get_unit` |

It also uses `real_t` and `real_sqrt`.

## The product

`quat_get_product` is the product of Hamilton. Each part is a sum of 4 products, added from left to right as the source writes them:

```pseudo
FUNCTION quat_get_product(a : quat_t, b : quat_t) RETURNS quat_t
   DECLARE out : quat_t
   out.x <- a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y
   out.y <- a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x
   out.z <- a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
   out.w <- a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
   RETURN out
ENDFUNCTION
```

The product turns by `b` first, and then by `a`. A case checks this order with 2 exact rotations.

## The unit

`quat_get_unit` divides the 4 parts by the length. A length of 0 has no direction, so the function gives the quaternion that turns nothing.
The length comes from the square, with the limits of `docs/intern/foundation/math/generic_vec3_products.h.md`, *The limits*.

## Functions

| Function | Result |
| --- | --- |
| `quat_get_dot` | Gives the dot product, added in the order `x`, `y`, `z`, `w`. |
| `quat_get_length` | Gives the root of the dot product of a quaternion with itself. |
| `quat_get_product` | Gives the product of Hamilton of 2 quaternions. |
| `quat_get_conjugate` | Gives the quaternion with the opposite axis part. |
| `quat_get_unit` | Gives the quaternion at the length 1, or `{ 0, 0, 0, 1 }` for a length of 0. |
