# src/foundation/math/scalar_root.c

This file holds the bisection of `f64_find_root` of `src/foundation/math/math_scalar.h`.

## The steps

```pseudo
FUNCTION f64_find_root(BYREF ask : f64_root_t, BYREF root : F64) RETURNS BOOLEAN
   DECLARE at_low : F64
   DECLARE at_high : F64
   at_low <- get_result(ask, ask.low)
   at_high <- get_result(ask, ask.high)
   IF at_low = 0 OR at_high = 0 THEN
      IF at_low = 0 THEN
         root <- ask.low
      ELSE
         root <- ask.high
      ENDIF
      RETURN TRUE
   ENDIF
   IF NOT has_other_sign(at_low, at_high) THEN
      RETURN FALSE
   ENDIF
   RETURN halve_range(ask, at_low, root)
ENDFUNCTION
```

`halve_range` halves the range `steps` times:

```pseudo
FUNCTION halve_range(BYREF ask : f64_root_t, at_low : F64, BYREF root : F64) RETURNS BOOLEAN
   DECLARE low : F64
   DECLARE high : F64
   DECLARE middle : F64
   DECLARE at_middle : F64
   low <- ask.low
   high <- ask.high
   middle <- low + (high - low) * 0.5
   FOR i <- 1 TO ask.steps
      at_middle <- get_result(ask, middle)
      IF at_middle <> at_middle THEN
         RETURN FALSE
      ENDIF
      IF at_middle = 0 THEN
         root <- middle
         RETURN TRUE
      ENDIF
      IF has_other_sign(at_low, at_middle) THEN
         high <- middle
      ELSE
         low <- middle
      ENDIF
      middle <- low + (high - low) * 0.5
   NEXT i
   root <- middle
   RETURN TRUE
ENDFUNCTION
```

- The new low bound always has the sign of the first low bound. Thus `at_low` does not change in the loop.
- The middle is `low` plus a half of the range, and not a half of the sum. Thus 2 large bounds do not overflow.
- A result that is a NaN is not equal to itself. Then the function gives `false` before it writes `root`.
- `get_result` calls `callback` of `ask` with `context` of `ask`. The search calls it 2 times, and then 1 time for each halving.

## The limits

- The range gets no smaller when `low` and `high` are 2 numbers with no number between them. More steps then give the same middle.
- With 2 roots or more between the bounds, the function finds 1 of them.

## The branches

Reflection has 1 case for each row of this table:

| Case | Result |
| --- | --- |
| The result at the low bound is 0. | `true`, and `root` is the low bound. |
| The result at the high bound is 0. | `true`, and `root` is the high bound. |
| The 2 results are above 0. | `false`, and `root` does not change. |
| The 2 results are below 0. | `false`, and `root` does not change. |
| A rising function with a root. | `true`, and `root` is the middle of the last range. |
| A falling function with a root. | `true`, and `root` is the middle of the last range. |
| A result at a middle is 0. | `true`, and `root` is that middle. |
| A result at a middle is a NaN. | `false`, and `root` does not change. |

## Functions

| Function | Result |
| --- | --- |
| `get_result` | Gives the result of the function of `ask` at a number. |
| `has_other_sign` | Tells if one number is below 0 and the other is above 0. |
| `halve_range` | Halves the range `steps` times, and writes the last middle. Gives `false` for a result that is a NaN. |
| `f64_find_root` | Writes a root of the function of `ask`, and gives `true`. Gives `false` when the 2 bounds give the same sign. |
