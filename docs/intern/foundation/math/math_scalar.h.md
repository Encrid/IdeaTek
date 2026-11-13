# src/foundation/math/math_scalar.h

This header gives the functions of 1 number to a caller of the math part.
The readme of the part gives their results: `docs/usage/foundation/math.md`, *The scalars*.

## The generic texts

A function that has the same text in 32 bits and in 64 bits is written 1 time, in a generic text.
A generic text uses generic names, for example `real_t` and `real_clamp`.
For each width, the header does these steps:

1. It defines each generic name as the name of that width, for example `real_clamp` as `f64_clamp`.
2. It includes the generic text.
3. It removes each definition with `#undef`.

Thus the compiler gets 2 functions from 1 text, and no generic name leaves the header.
A search for `f64_clamp` finds its line of `#define` in this header, and the line names the generic text.

2 exceptions of the style make this possible: `.toolchain/project.toml`, `[[style.exception]]`.
The rule H2 has an exception for each file `generic_*.h`, and the rule V1 has an exception for each file `math_*.h`.

## The fusion

The header starts with `#pragma STDC FP_CONTRACT OFF`, after its includes, and ends with `#pragma STDC FP_CONTRACT DEFAULT`.
Between them, the compiler keeps each product and each sum as its own operation, also inside a caller that is built with the fusion on.
Thus each function gives the same bits on a processor with a fused operation and on a processor without it.
`DEFAULT` gives back the state of the flag of the caller, so the code of the caller keeps its own state.

The pragma is at the scope of the file, and not in the body of a function.
`test --branches` writes a probe after the `{` of each function, and the compiler refuses the pragma after a statement.

## The split of a number

`f64_split` gives a number of 64 bits to a reader that has only 32 bits, for example a shader:

```pseudo
PROCEDURE f64_split(value : F64, BYREF high : F32, BYREF low : F32)
   high <- value
   low <- value - high
ENDPROCEDURE
```

The difference is exact in 64 bits, because `high` is the nearest number of 32 bits to `value`. The second cast then rounds it to 32 bits.

## The root of a function

`f64_root_t` holds what `f64_find_root` needs. A structure keeps the function at 2 arguments.
`f64_function_t` is the type of `callback`: it takes `context` and a number, and gives a number.

## Functions

| Function | Result |
| --- | --- |
| `f32_clamp`, `f64_clamp` | `real_clamp` of `src/foundation/math/generic_scalar_range.h`. |
| `f32_blend`, `f64_blend` | `real_blend` of the same file. |
| `f32_is_finite`, `f64_is_finite` | `real_is_finite` of the same file. |
| `f64_split` | Writes a number as a number of 32 bits, and the rest in 32 bits. |
| `f64_get_sin`, `f64_get_cos` | The prototypes. The functions are in `src/foundation/math/scalar_sine.c`. |
| `f64_get_atan2` | The prototype. The function is in `src/foundation/math/scalar_arc.c`. |
| `f64_get_log` | The prototype. The function is in `src/foundation/math/scalar_log.c`. |
| `f32_get_sin`, `f32_get_cos`, `f32_get_atan2` | Call the function of 64 bits, and round its result to 32 bits. |
| `f64_find_root` | The prototype. The function is in `src/foundation/math/scalar_root.c`. |
