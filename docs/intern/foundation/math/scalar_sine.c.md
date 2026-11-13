# src/foundation/math/scalar_sine.c

This file holds `f64_get_sin` and `f64_get_cos` of `src/foundation/math/math_scalar.h`.
`docs/intern/foundation/math/trig_inside.h.md`, *The origin*, tells where the method comes from.

## The ranges

Each function selects 1 of 4 ways from the size of the angle:

| Angle | `f64_get_sin` | `f64_get_cos` |
| --- | --- | --- |
| Very small: below 2^-26 for the sine, below about 2^-27 for the cosine | The angle itself. | 1. |
| Up to a quarter of pi | The series of the sine. | The series of the cosine. |
| An infinity or a NaN | The fixed NaN. | The fixed NaN. |
| Each other angle | The reduction, then 1 of the 2 series. | The same. |

The sine of -0 is -0, because the first way gives the angle back.
musl computes the angle minus the angle for an infinity. That NaN has another sign on x64 than on arm64, so a constant is here.

## The quarter turns

After the reduction, `pick_sine` selects the series by the count of quarter turns modulo 4:

| Count modulo 4 | Sine of the angle |
| --- | --- |
| 0 | The sine of the rest. |
| 1 | The cosine of the rest. |
| 2 | Minus the sine of the rest. |
| 3 | Minus the cosine of the rest. |

The cosine of an angle is the sine of the angle plus 1 quarter turn. Thus `f64_get_cos` calls `pick_sine` with the count plus 1.
This gives the 4 cases of the cosine of musl with the same calls.

## The accuracy

The result is within 1 unit in the last place of the exact value. On 746 angles from 2^-40 to the largest number, the largest distance was 0.67 unit.
`f32_get_sin` and `f32_get_cos` of the header round the result of 64 bits to 32 bits.

## Functions

| Function | Result |
| --- | --- |
| `pick_sine` | Gives the sine of an angle from its rest and its count of quarter turns. |
| `f64_get_sin` | Gives the sine of an angle in radians. |
| `f64_get_cos` | Gives the cosine of an angle in radians. |
