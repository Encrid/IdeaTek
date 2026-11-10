# src/engine_subsystems/space_bridge/space_check.c

This file tests the values of the space part. It is pure: the program `space_check` of the tests calls it.

`is_transform_ok` refuses each transform that `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:Transform*, refuses. It sums the values, so 1 test of the sum finds a NaN or an infinity.
`is_finite` tells a finite value by `v - v == 0.0`: an infinity or a NaN gives a NaN, which no value equals.
The C library of macOS gives the macro `isfinite` a condition that the branches gate cannot measure, so `is_finite` is a function.
`is_same_world` compares the bytes of 2 places, so 0 and -0 differ.

## Functions

| Function | Result |
| --- | --- |
| `is_finite` | `true` for a value that is not an infinity or a NaN. |
| `is_transform_ok` | `true` when the transform is finite and its rotation has a length. |
| `is_same_world` | `true` when the 2 places have the same bytes. |
