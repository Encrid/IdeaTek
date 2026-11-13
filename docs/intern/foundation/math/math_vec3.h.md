# src/foundation/math/math_vec3.h

This header gives the vectors of 3 to a caller of the math part.
It holds the 2 types, the functions of the 2 widths, and the functions that go from one width to the other.
Their results are in `docs/usage/foundation/math.md`, *The vectors*.

## The types

`vec3f_t` and `vec3d_t` are vector types of clang with 3 parts.
The compiler keeps such a vector in the room of 4 numbers. Thus a vector of 3 fits 1 register of 4 numbers.
The fourth number has no meaning, and no function reads it.

## The widths

The header includes 3 generic texts 1 time for each width, as `docs/intern/foundation/math/math_scalar.h.md`, *The generic texts*, tells.
`real_sqrt` is `sqrtf` in 32 bits and `sqrt` in 64 bits. IEEE 754 fixes the result of both, so the C library gives the same bits on each system.

## From one width to the other

- `vec3d_get_narrow` and `vec3f_get_wide` use `__builtin_convertvector`, which converts each part as a cast does.
- `vec3d_split` does the steps of `f64_split` on the 3 parts at the same time:

```pseudo
PROCEDURE vec3d_split(a : vec3d_t, BYREF high : vec3f_t, BYREF low : vec3f_t)
   high <- vec3d_get_narrow(a)
   low <- vec3d_get_narrow(a - vec3f_get_wide(high))
ENDPROCEDURE
```

The difference loses nothing, for the reason of `docs/intern/foundation/math/math_scalar.h.md`, *The split of a number*.

## Functions

| Function | Result |
| --- | --- |
| The functions of the 2 widths | `src/foundation/math/generic_vec3_products.h`, `src/foundation/math/generic_vec3_shapes.h` and `src/foundation/math/generic_vec3_numbers.h`. |
| `vec3d_get_narrow` | Gives the vector in 32 bits, each part rounded to the nearest. |
| `vec3f_get_wide` | Gives the vector in 64 bits, with no loss. |
| `vec3d_split` | Writes the vector in 32 bits, and the rest of each part in 32 bits. |
