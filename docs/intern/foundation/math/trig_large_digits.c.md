# src/foundation/math/trig_large_digits.c

This file takes the count of quarter turns out of the product of a large angle and 2/pi.

## The steps

`trig_large_count` does 3 steps:

1. `distill` goes from the last term of `q` to the first. It carries each overflow of 24 bits to the next term, and writes the low digits.
2. `take_count` scales the top term by `scale` and removes each multiple of 8. The whole part is the count, and the fraction stays.
3. If the fraction is a half or more, `negate_fraction` adds 1 to the count and negates the fraction.

After step 3, the rest is negative when `half` is not 0, and `trig_large_get_rest` gives it its sign.

## The half

`take_count` finds `half` in 3 ways, by `scale`:

| `scale` | Where the bit of a half is |
| --- | --- |
| Above 0 | In the top digit. The bits above it belong to the count. |
| 0 | The top bit of the top digit. |
| Below 0 | In `fraction`. Then `half` is 2, and `fraction` itself is negated. |

## The negation

`borrow_digits` subtracts the digits from 1, from the lowest digit up. A digit of 0 below the first digit that is not 0 stays 0.
It gives 1 if a digit was not 0. `negate_fraction` then also subtracts that borrow from `fraction`.

The test of that borrow has a side that no case reaches: `.toolchain/project.toml`, `[[branches.exception]]`.
With no borrow, each low digit is 0, which is 96 bits at 0 in a row after the fraction.
The worst angle of 64 bits has 61 such bits: `docs/intern/foundation/math/trig_large_rest.c.md`, *The branches*.

## The branches

The cases of Reflection for this file:

| Case | Result |
| --- | --- |
| A scale of 1, of 2, of 0 and below 0. | The count and the half come from the right place. |
| A fraction below a half, and a fraction of a half or more. | The rest stays, or it is negated. |
| A lowest digit of 0 in a negation. | The digit stays 0. |

## Functions

| Function | Result |
| --- | --- |
| `distill` | Writes the low digits of the product, and its top into `fraction`. |
| `take_count` | Writes the count, the fraction and `half`. |
| `borrow_digits` | Subtracts the digits from 1, and tells if a digit was not 0. |
| `negate_fraction` | Replaces the fraction by 1 minus the fraction, and adds 1 to the count. |
| `trig_large_count` | Does the 3 steps. |
