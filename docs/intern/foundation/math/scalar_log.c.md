# src/foundation/math/scalar_log.c

This file holds `f64_get_log` of `src/foundation/math/math_scalar.h`.
Its steps are those of `log` of musl 1.2.5, which come from Arm Limited: `third_party/musl/license`.

## The differences from musl

- musl has 2 forms of 1 step: one with a fused operation, and one without it. This file has only the form without it, on each target.
  Thus a processor with a fused operation gives the same bits as a processor without it.
- For 0, musl gives a negative infinity. `f64_get_log` gives the fixed NaN, as the readme of the part tells.

## The ranges

| Number | Way |
| --- | --- |
| From 0.9375 up to 1.064697265625 | `get_near_one`: a series of the number minus 1. |
| A NaN, 0 or less | The fixed NaN. |
| A positive infinity | The same infinity. |
| Below the smallest normal number | The number times 2^52, with 52 subtracted from its exponent, then `get_far`. |
| Each other number | `get_far`. |

The first test is 1 comparison of the bits of the number: `NEAR_LOW` and `NEAR_HIGH` are the bits of the 2 bounds.

## Near 1

`get_near_series` computes the terms of the series from the third power, with `LOG_NEAR`.
`get_near_one` adds the first 2 terms with care. It cuts the number minus 1 into a high half of 26 bits and a low half.
The square of the high half is exact, so the second term loses no bit. musl states an error of about 0.507 unit in the last place there.

## Far from 1

`get_far` writes the number as 2 to a power `k`, times a number `z` near 1. A table of 128 rows cuts the range of `z`.
For the row of `z`, `LOG_CENTER` holds a center `c` as 2 numbers, and `LOG_INVERSE` holds 1 divided by `c` and the logarithm of `c`.
The result is `k` times the logarithm of 2, plus the logarithm of `c`, plus a short series of `z / c - 1` with `LOG_FAR`.
The difference of `z` and the 2 numbers of `c` is exact. That is the form of musl without a fused operation.

## The accuracy

On 800 numbers from the smallest to the largest, no result was more than 0.499 unit in the last place from the exact value.

## Functions

| Function | Result |
| --- | --- |
| `get_bits` | Gives the bits of a number. |
| `get_near_series` | Gives the terms of the series near 1 from the third power. |
| `get_near_one` | Gives the logarithm of a number near 1. |
| `get_far` | Gives the logarithm of a normal number from its bits. |
| `f64_get_log` | Gives the natural logarithm of a number. |
