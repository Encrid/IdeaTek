# src/foundation/math/scalar_arc.c

This file holds `f64_get_atan2` of `src/foundation/math/math_scalar.h`. Its steps are those of `atan2` of musl, with 1 difference.

## The difference from musl

When `x` and `y` are both 0, the function gives 0, as the readme of the part tells.
musl gives 0, -0, pi or minus pi there, from the signs of the 2 zeros.

## The order of the tests

`f64_get_atan2` tests these cases in this order:

| Case | Result |
| --- | --- |
| A NaN in `x` or in `y` | The fixed NaN. musl gives the sum of the 2 numbers there. |
| The 2 numbers are 0 | 0. |
| `x` is 1 | The arc tangent of `y`. |
| A 0 or an infinity in `x` or in `y` | `get_edge`. |
| Each other pair | `get_inner`. |

## The edges

`get_edge_size` gives the size of the angle, and `get_edge` gives it the sign of `y`:

| Pair | Size |
| --- | --- |
| 2 infinities | A quarter of pi for a positive `x`, and 3 quarters of pi for a negative `x`. |
| `y` is 0, or `x` is an infinity | 0 for a positive `x`, and pi for a negative `x`. |
| `x` is 0, or `y` is an infinity | A half of pi. |

The sign of a `y` of 0 counts, as in each other row.

## The inner pairs

`get_inner` compares the exponents of the 2 numbers before it divides:

- `y` is more than 2^64 times `x`: the angle is upright, so the result is a half of pi with the sign that `y` has.
- `x` is negative, and more than 2^64 times `y`: the arc tangent counts as 0. Thus the division does not underflow.
- Else the function takes the arc tangent of `y / x` with no sign.

For a negative `x`, the result is pi minus that arc tangent. `PI_LOW` holds the bits of pi that `PI` cannot hold, and the function subtracts it first.

## The accuracy

On 600 pairs with exponents from -30 to 30, the largest distance from the exact value was 0.98 unit in the last place.

## Functions

| Function | Result |
| --- | --- |
| `get_edge_size` | Gives the size of the angle of a pair with a 0 or an infinity. |
| `get_edge` | Gives that angle with the sign of `y`. |
| `get_inner` | Gives the angle of a pair of finite numbers that are not 0. |
| `f64_get_atan2` | Gives the angle of the point `x`, `y`. |
