# src/foundation/math/trig_arc.c

This file holds the arc tangent of 1 number, which `f64_get_atan2` calls. Its steps are those of `atan` of musl.

## The ranges

`trig_get_arc_tangent` selects a way from the 32 high bits of the number with no sign:

| Size of the number | Way |
| --- | --- |
| 2^66 or more | A half of pi, with the sign of the number. |
| Below 2^-27 | The number itself. |
| Below 0.4375 | The series on the number. |
| Each other size | A reduction by `reduce_ratio`, then the series, then the constants of the reduction. |

## The reduction

`reduce_ratio` uses the formula of the arc tangent of a difference. It moves the number near 0, around 1 of 4 points:

| Number | Point | New number |
| --- | --- | --- |
| Below 0.6875 | 0.5 | `(2x - 1) / (2 + x)` |
| Below 1.1875 | 1 | `(x - 1) / (x + 1)` |
| Below 2.4375 | 1.5 | `(x - 1.5) / (1 + 1.5x)` |
| Above | An infinity | `-1 / x` |

`ATANHI` and `ATANLO` hold the arc tangent of each point as 2 numbers. The result is that constant plus the series of the new number.

## The series

`get_series` computes a polynomial with the 11 coefficients `AT` of musl, in 2 halves: the odd terms and the even terms.
musl writes each half as 1 long line of nested products. Here each half is a loop from the last coefficient to the first, with the same operations.

## Functions

| Function | Result |
| --- | --- |
| `reduce_ratio` | Gives the number moved near 0, and writes which point it used into `part`. |
| `get_series` | Gives the part of the arc tangent that the number itself does not give. |
| `trig_get_arc_tangent` | Gives the arc tangent of a number that is no NaN. |
