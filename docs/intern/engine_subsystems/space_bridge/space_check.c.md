# src/engine_subsystems/space_bridge/space_check.c

This file tests the values of the space part. It is pure: the program `space_check` of the tests calls it.

## The tests

`transform_ok` refuses each transform that `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:Transform*, refuses.
`world_same` compares the bytes of 2 places. A place with the same bytes is not attached again: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*.

## Functions

| Function | Result |
| --- | --- |
| `transform_ok` | `true` when the transform is finite and its rotation has a length. |
| `world_same` | `true` when the 2 places have the same bytes. |
