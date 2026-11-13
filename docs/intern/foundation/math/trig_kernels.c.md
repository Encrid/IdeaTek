# src/foundation/math/trig_kernels.c

This file holds the 2 series of the sine and the cosine, for a rest of at most a quarter of pi.

## The sine

`trig_get_sine` computes a polynomial of degree 13 with the 6 coefficients `S1` to `S6` of musl.
The polynomial is within 2^-58 of the sine on this range, as the source of musl tells.

- With `has_tail` false, the angle is exact and the function reads no `tail`.
- With `has_tail` true, `tail` is the low part of the rest, and a second formula adds its effect.

## The cosine

`trig_get_cosine` computes a polynomial of degree 14 with the 6 coefficients `C1` to `C6` of musl.
It first computes 1 minus a half of the square, and then adds the error of that difference and the rest of the polynomial.
Thus the result keeps its last bits when the angle is small.

## The order

Each formula has the order of its parentheses in musl. A different order gives other last bits.
The line of `r` of the cosine of musl is too long for 1 line of 80 columns. `low` holds its first half, and the sum has the same order.

## Functions

| Function | Result |
| --- | --- |
| `trig_get_sine` | Gives the sine of the rest `x`, with its low part `tail` when `has_tail` is true. |
| `trig_get_cosine` | Gives the cosine of the rest `x` with its low part `tail`. |
