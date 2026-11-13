# src/foundation/math/generic_quat_turns.h

This generic text links a quaternion of length 1 to what it does: it turns a vector, and it has an axis and an angle.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `quat_turn` | `quatf_turn` | `quatd_turn` |
| `quat_turn_back` | `quatf_turn_back` | `quatd_turn_back` |
| `quat_make_turn` | `quatf_make_turn` | `quatd_make_turn` |
| `quat_get_angle` | `quatf_get_angle` | `quatd_get_angle` |
| `quat_get_axis` | `quatf_get_axis` | `quatd_get_axis` |

It also uses `real_abs`, `real_get_sin`, `real_get_cos`, `real_get_atan2`, `vec3_get_cross` and `vec3_get_length`.

## The turn of a vector

`quat_turn` uses 2 cross products, and no product of quaternions:

```pseudo
FUNCTION quat_turn(q : quat_t, v : vec3_t) RETURNS vec3_t
   DECLARE axis : vec3_t
   DECLARE twice : vec3_t
   axis.x <- q.x
   axis.y <- q.y
   axis.z <- q.z
   twice <- vec3_get_cross(axis, v) * 2
   RETURN v + twice * q.w + vec3_get_cross(axis, twice)
ENDFUNCTION
```

The product by 2 is exact. The sum adds `v` and the second term first, and then the third term.
`quat_turn_back` is the same turn with the conjugate.

## The axis and the angle

- `quat_make_turn` takes the sine and the cosine of a half of the angle. The axis must have the length 1; the function does not check it.
- `quat_get_angle` is 2 times the arc tangent of the length of the axis part and the real part with no sign. Thus the angle is from 0 to pi.
- `quat_get_axis` divides the axis part by its length. For a negative real part it gives the opposite axis, which fits the angle from 0 to pi.
  A part of 0 of that opposite axis is -0.

## The branches

The cases of Reflection for the axis:

| Case | Result |
| --- | --- |
| An axis part of length 0. | The X axis. |
| A negative real part. | The opposite axis. |
| A positive real part. | The axis. |

## Functions

| Function | Result |
| --- | --- |
| `quat_turn` | Gives the vector turned by the quaternion. |
| `quat_turn_back` | Gives the vector turned the other way. |
| `quat_make_turn` | Gives the quaternion of an axis and an angle. |
| `quat_get_angle` | Gives the angle of the rotation. |
| `quat_get_axis` | Gives the axis of the rotation. |
