# src/foundation/math/generic_quat_checks.h

This generic text holds the 2 checks of a quaternion and the spherical blend.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `quat_is_finite` | `quatf_is_finite` | `quatd_is_finite` |
| `quat_is_unit` | `quatf_is_unit` | `quatd_is_unit` |
| `quat_get_blend` | `quatf_get_blend` | `quatd_get_blend` |

## The blend

```pseudo
FUNCTION quat_get_blend(a : quat_t, b : quat_t, share : F64) RETURNS quat_t
   DECLARE to : quat_t
   DECLARE angle : F64
   DECLARE sine : F64
   IF quat_get_dot(a, b) < 0 THEN
      to <- -b
   ELSE
      to <- b
   ENDIF
   angle <- 2 * real_get_atan2(quat_get_length(a - to), quat_get_length(a + to))
   sine <- real_get_sin(angle)
   IF sine = 0 THEN
      RETURN a
   ENDIF
   RETURN a * (real_get_sin((1 - share) * angle) / sine) + to * (real_get_sin(share * angle) / sine)
ENDFUNCTION
```

- A quaternion and its opposite are the same rotation. With a negative dot product, the function blends to the opposite of `b`, which is nearer.
- The angle between the 2 quaternions comes from the lengths of their difference and their sum. This form loses no bit when the 2 are near.
  The usual form takes the arc cosine of the dot product, and it loses half of the bits there.
- The function has no limit below which it changes to a linear blend. The sine of a small angle is not 0, so the division stays right.
- With an angle of 0 the 2 quaternions are equal, and the function gives `a`.
- The 2 ends are exact, because the sine of 0 is 0 and a number divided by itself is 1. The end of a share of 1 is `to`.

## The branches

The cases of Reflection for the blend:

| Case | Result |
| --- | --- |
| A dot product of 0 or more. | The blend to `b`. |
| A dot product below 0. | The blend to the opposite of `b`. |
| A quaternion and its opposite. | `a`. |

## Functions

| Function | Result |
| --- | --- |
| `quat_is_finite` | Tells if the 4 parts are finite. |
| `quat_is_unit` | Tells if the length is 1, to a margin. |
| `quat_get_blend` | Gives the spherical blend of 2 quaternions of length 1. |
