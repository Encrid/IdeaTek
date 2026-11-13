# src/foundation/math/trig_reduce.c

This file selects the reduction of an angle, and holds the shortest one: an angle near 1, 2, 3 or 4 quarter turns.

## The choice

`trig_reduce` reads the 32 high bits of the angle with no sign, as musl does:

| High bits | Reduction |
| --- | --- |
| Up to `0x401c463b`, 9 quarters of pi | `reduce_by_count`, if `get_near_count` gives a count. |
| Below `0x413921fb`, 2^20 quarter turns | `trig_reduce_medium`. |
| Above | `trig_reduce_large`. |

The callers handle an infinity and a NaN before the reduction.

## The count near a quarter turn

`get_near_count` gives 1, 2, 3 or 4 from the high bits. It gives 0 in 2 cases, and then the medium reduction runs:

- the angle is above 9 quarters of pi;
- the high bits are those of a multiple of a half of pi. There the subtraction cancels most bits, and the medium reduction has 2 more rounds for that.

## The short reduction

`reduce_by_count` subtracts `count` times a half of pi, which is in 2 parts: `PIO2_1` holds its first 33 bits, and `PIO2_1T` the rest.
The product of `count` and `PIO2_1` is exact. Thus the first subtraction loses nothing.
For a negative angle the count is negative. musl writes this case as a sum, which is the same operation.

## The branches

The cases of Reflection for the choice:

| Case | Result |
| --- | --- |
| An angle near 1, 2, 3 and 4 quarter turns, positive and negative. | The short reduction with the count and its sign. |
| The numbers nearest to a half of pi, pi, 3 halves of pi and 2 pi. | The medium reduction. |
| An angle between 9 quarters of pi and 2^20 quarter turns. | The medium reduction. |
| An angle above 2^20 quarter turns. | The large reduction. |

## Functions

| Function | Result |
| --- | --- |
| `trig_get_high` | Reads the bits of a number, and gives the high half. |
| `get_near_count` | Gives the count of quarter turns for the short reduction, or 0. |
| `reduce_by_count` | Writes the rest of an angle minus `count` quarter turns, and gives `count`. |
| `trig_reduce` | Writes the rest of an angle and gives its count of quarter turns. |
