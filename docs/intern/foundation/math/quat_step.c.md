# src/foundation/math/quat_step.c

This file holds `quatd_step` of `src/foundation/math/math_quat.h`: the step of a rotation that turns at a constant angular speed.

## The steps

```pseudo
FUNCTION quatd_step(q : quatd_t, speed : vec3d_t, time : F64) RETURNS quatd_t
   DECLARE rate : F64
   DECLARE angle : F64
   DECLARE turn : quatd_t
   rate <- vec3d_get_length(speed)
   angle <- rate * time
   IF angle = 0 THEN
      RETURN q
   ENDIF
   turn <- quatd_make_turn(speed / rate, angle)
   RETURN quatd_get_unit(quatd_get_product(turn, q))
ENDFUNCTION
```

- The step is exact for a constant speed: it is 1 rotation about the axis of the speed, and no sum of small steps.
  The usual step adds a half of the speed times the quaternion times the time, and it is right only for a small angle.
- The speed is given in the outer space, the one that the turned vectors are in. Thus the turn of the step comes on the left of the product.
- The test of the angle covers a speed of 0 and a time of 0. Thus the function never divides by a length of 0.
- The product of 2 quaternions of length 1 has the length 1 only to the rounding. The last call brings it back to 1, so many steps do not drift.

## Functions

| Function | Result |
| --- | --- |
| `quatd_step` | Gives the rotation after a time at an angular speed. |
