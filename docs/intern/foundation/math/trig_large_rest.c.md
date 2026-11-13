# src/foundation/math/trig_large_rest.c

This file holds `trig_reduce_large`, which runs the steps of a large angle in a loop.

## The loop

```pseudo
FUNCTION trig_reduce_large(x : F64, BYREF rest : ARRAY[0:1] OF F64) RETURNS I32
   DECLARE state : trig_large_t
   DECLARE count : I32
   CALL trig_large_start(state, x)
   REPEAT
      CALL trig_large_count(state)
   UNTIL NOT extend_terms(state)
   IF state.fraction = 0 THEN
      CALL chop_digits(state)
   ELSE
      CALL break_fraction(state)
   ENDIF
   CALL trig_large_get_rest(state, rest)
   count <- state.count MOD 8
   IF x >= 0 THEN
      RETURN count
   ENDIF
   rest[0] <- -rest[0]
   rest[1] <- -rest[1]
   RETURN -count
ENDFUNCTION
```

musl writes the loop with a `goto`. The style refuses `goto`, so the loop is a `do` and `while`.

## More terms

When the angle is very near a multiple of a quarter turn, the fraction and the digits above the first 4 are all 0.
The terms of `q` then do not hold enough bits of the rest. `extend_terms` adds terms, and the count runs again.
It adds 1 term, and 1 more for each low digit that is also 0.

## The end of the digits

- `chop_digits` runs when the fraction is 0. It removes the digits of 0 at the top, and lowers `scale` by 24 for each one.
- `break_fraction` runs when the fraction is not 0. It writes the fraction as 1 digit, or as 2 digits when it does not fit 24 bits.

## The branches

The cases of Reflection for this file:

| Case | Result |
| --- | --- |
| 10^22. | More terms, 1 time. |
| The angle of 64 bits that is nearest to a quarter turn. | More terms with 2 low digits of 0, and 2 digits removed. |
| A fraction that fits 24 bits, and a fraction that does not. | 1 digit, or 2 digits. |
| A negative large angle. | The opposite count and the opposite rest. |

## Functions

| Function | Result |
| --- | --- |
| `extend_terms` | Adds terms to `q` when the rest has no bit yet. Tells if it added terms. |
| `chop_digits` | Removes the top digits that are 0. |
| `break_fraction` | Writes the fraction as digits. |
| `trig_reduce_large` | Does the reduction of a large angle. |
