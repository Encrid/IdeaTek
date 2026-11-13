# src/foundation/math/generic_scalar_range.h

This file is a generic text: it holds the functions of 1 number that have the same text for each width.
`src/foundation/math/math_scalar.h` includes it 1 time for each width: `docs/intern/foundation/math/math_scalar.h.md`, *The generic texts*.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `real_t` | `float` | `double` |
| `real_clamp` | `f32_clamp` | `f64_clamp` |
| `real_blend` | `f32_blend` | `f64_blend` |
| `real_is_finite` | `f32_is_finite` | `f64_is_finite` |

The file has no include guard, because a guard stops the second include.
It includes no header. The header that includes it gives `isfinite` of the C library.

## The clamp

`real_clamp` compares `value` with `low` first, and then with `high`.
A comparison with a NaN is false. Thus a `value` that is a NaN comes back with no change.

## The blend

`real_blend` computes the 2 products first, and then their sum:

```pseudo
FUNCTION real_blend(a : F64, b : F64, share : F64) RETURNS F64
   DECLARE rest : F64
   rest <- 1 - share
   RETURN a * rest + b * share
ENDFUNCTION
```

With a `share` of 0, `rest` is 1 and the result is `a` exactly. With a `share` of 1, `rest` is 0 and the result is `b` exactly.
The shorter form with 1 product, `a` plus the difference times `share`, does not give `b` exactly when `a` is much larger than `b`.
The 2 products and the sum stay 3 operations: `docs/intern/foundation/math/math_scalar.h.md`, *The fusion*.

## The limits

`real_blend` gives a NaN when a value is an infinity and its factor is 0.

## Functions

| Function | Result |
| --- | --- |
| `real_clamp` | Gives `value`, or the bound that `value` is outside. |
| `real_blend` | Gives the blend of `a` and `b`: `a` for a `share` of 0, and `b` for a `share` of 1. |
| `real_is_finite` | Tells if the number is finite. |
