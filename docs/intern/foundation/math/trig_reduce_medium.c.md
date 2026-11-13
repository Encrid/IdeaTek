# src/foundation/math/trig_reduce_medium.c

This file reduces an angle below 2^20 quarter turns, in 1, 2 or 3 rounds.

## The count

`round_count` multiplies the angle by 2/pi and rounds the product to a whole number.
It rounds with the sum and the difference of `TOINT`, which is 1.5 times 2^52. At that size a number of 64 bits has no fraction.
Then it computes the rest of the first round. A rest outside a quarter of pi tells that the count went to the wrong side. The function then moves it by 1.

## The rounds

A half of pi is cut into 3 pairs of constants. Each pair has a head of 33 bits and a tail.

| Round | Constants | The rest is good to |
| --- | --- | --- |
| 1 | `PIO2_1`, `PIO2_1T` | 85 bits |
| 2 | `SECOND` | 118 bits |
| 3 | `THIRD` | 151 bits |

`trig_reduce_medium` compares the exponent of the angle with the exponent of the rest.
A difference above 16 tells that the subtraction cancelled more than 16 bits, and round 2 runs. A difference above 49 after round 2 starts round 3.
`refine_rest` does 1 more round: it subtracts the head of the pair, and puts the error of that subtraction into the tail.

## The branches

The cases of Reflection for this reduction:

| Case | Result |
| --- | --- |
| The rest of the first round is inside a quarter of pi. | The count stays. |
| The rest is below minus a quarter of pi, or above a quarter of pi. | The count goes down or up by 1. |
| 100, 1000000, and the number nearest to a half of pi. | 1 round, 2 rounds and 3 rounds. |

## Functions

| Function | Result |
| --- | --- |
| `get_exponent` | Gives the 11 bits of the exponent of a number. |
| `round_count` | Gives the angle times 2/pi, rounded to a whole number. |
| `refine_rest` | Does 1 more round on the rest `r` and its tail `w`. |
| `trig_reduce_medium` | Writes the rest and gives the count. |
