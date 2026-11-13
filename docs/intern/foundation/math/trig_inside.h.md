# src/foundation/math/trig_inside.h

This header is for the sources of the 3 functions of an angle only: sine, cosine and arc tangent. The release of the part does not hold it.
It gives the prototypes that these sources share, and the state of the reduction of a large angle.

## The origin

The sine, the cosine, the arc tangent and the reduction of an angle follow the steps of musl 1.2.5, which come from Sun Microsystems: `third_party/musl/license`.
Each source keeps the order of the operations of musl, so it gives the same bits as musl gives without a fused operation.
The functions are shorter than those of musl, because a function of the project has at most 20 lines.

## The sources

| Source | Content |
| --- | --- |
| `src/foundation/math/scalar_sine.c` | `f64_get_sin` and `f64_get_cos`. |
| `src/foundation/math/trig_kernels.c` | The 2 series, for an angle of at most a quarter of pi. |
| `src/foundation/math/trig_reduce.c` | The choice of a reduction, and the reduction near 1 to 4 quarter turns. |
| `src/foundation/math/trig_reduce_medium.c` | The reduction below 2^20 quarter turns. |
| `src/foundation/math/trig_large_terms.c` | The start of the reduction of a large angle, with the table of 2/pi. |
| `src/foundation/math/trig_large_digits.c` | The count of quarter turns of a large angle. |
| `src/foundation/math/trig_large_rest.c` | The loop of the reduction of a large angle. |
| `src/foundation/math/trig_large_sum.c` | The rest of a large angle, as 2 numbers. |
| `src/foundation/math/scalar_arc.c` | `f64_get_atan2`. |
| `src/foundation/math/trig_arc.c` | The arc tangent of 1 number. |

## A reduction

A reduction gives the count of quarter turns of an angle, and writes the rest as 2 numbers in `rest`.
`rest[0]` is the rest, between minus a quarter of pi and a quarter of pi. `rest[1]` is the part of the rest that `rest[0]` cannot hold.
The 2 numbers together hold the rest to more than 100 bits. Thus the series gets a rest that is exact enough for the last bit.

## trig_large_t

`trig_large_t` is the state of the reduction of a large angle. It lives on the stack of `trig_reduce_large`.

| Field | Content |
| --- | --- |
| `input` | The 53 bits of the angle as 3 digits of 24 bits. |
| `q` | The products of the angle and the digits of 2/pi. |
| `fraction` | The top of the product, after the count of quarter turns leaves it. |
| `digits` | The lower part of the product, as digits of 24 bits. |
| `last_input` | The index of the last digit of `input` that is not 0. |
| `first_term` | The index of the first digit of 2/pi that the angle needs. |
| `scale` | The power of 2 of the top digit. |
| `last` | The index of the last term of `q`. |
| `count` | The count of quarter turns. |
| `half` | 0 when the rest is below a half of a quarter turn. Then the rest stays as it is. |

`TRIG_TERMS` is the count of the terms of `q` at the start, minus 1. `TRIG_DIGITS` is the room of each array.

## Functions

| Function | Result |
| --- | --- |
| `trig_get_high` | Gives the 32 high bits of a number of 64 bits. |
| `trig_get_sine`, `trig_get_cosine` | The series of the sine and of the cosine of a rest. |
| `trig_get_arc_tangent` | The arc tangent of 1 number. |
| `trig_reduce` | Reduces an angle above a quarter of pi. |
| `trig_reduce_medium`, `trig_reduce_large` | The reductions of a medium angle and of a large angle. |
| `trig_large_start`, `trig_large_get_sum`, `trig_large_count`, `trig_large_get_rest` | The steps of the reduction of a large angle. |
