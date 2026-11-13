# src/foundation/math/trig_large_sum.c

This file turns the digits of the fraction of a large angle into the rest, as 2 numbers.

## The steps

`trig_large_get_rest` does 3 steps:

1. `scale_digits` gives each digit its power of 2, from `scale` down by 24 for each digit. A digit times a power of 2 is exact.
2. `sum_parts` multiplies the digits by a half of pi, which `PIO2` holds as 5 pieces of 24 bits. It writes 1 sum for each digit.
3. The function adds the sums from the smallest to the largest into `rest[0]`. Then it adds what `rest[0]` lost into `rest[1]`.

When `half` of the state is not 0, the 2 numbers get the opposite sign.

## The table

`PIO2` holds the first 5 pieces of a half of pi of musl. musl has 8 pieces, and a number of 64 bits uses 5 of them.
Each piece is written as a hexadecimal float, which holds its bits with no rounding by the compiler.

## Functions

| Function | Result |
| --- | --- |
| `scale_digits` | Writes each digit with its power of 2 into `q`. |
| `sum_parts` | Writes the products of the digits and a half of pi. |
| `trig_large_get_rest` | Writes the rest as 2 numbers. |
