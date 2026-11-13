# src/foundation/math/trig_large_terms.c

This file starts the reduction of an angle above 2^20 quarter turns. It holds the table of 2/pi.

## The method

An angle times 2/pi tells how many quarter turns it holds. For a large angle, most bits of that product are whole turns, which change nothing.
So the reduction multiplies the angle only by the digits of 2/pi that give the last 3 bits of the count and the fraction.
All numbers are digits of 24 bits in a `double`. A product of 2 such digits and a sum of 3 products are exact.

## The table

`IPIO2` holds 66 digits of 24 bits of 2/pi, after the binary point. They are the digits of musl for a number of 64 bits.
A program copied them from the source of musl, and no person typed them.

## The start

`split_input` cuts the 53 bits of the angle into 3 digits of 24 bits, with the top digit first. It reads the bits of the number, and no sign.
It then lowers `last_input` while the last digit is 0. Thus a power of 2 has 1 digit.

`trig_large_start` computes `first_term` and `scale` from the exponent of the angle, and then the first 5 terms of `q`.
The exponent of a large angle is at least 20. Thus `first_term` is never below 0, and the test of musl for that case is not here.

`trig_large_get_sum` gives 1 term of `q`: the sum of the products of each digit of the angle and 1 digit of 2/pi.
`get_term` gives a digit of 2/pi. Its index is below 0 for the first terms of an angle below 2^72, and the digit is then 0.

## Functions

| Function | Result |
| --- | --- |
| `get_term` | Gives 1 digit of 2/pi for an index of the product, or 0 before the table. |
| `trig_large_get_sum` | Gives the term `index` of the product of the angle and 2/pi. |
| `split_input` | Cuts the bits of the angle into `input`. |
| `trig_large_start` | Fills the state for an angle. |
