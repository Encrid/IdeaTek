# src/engine_subsystems/space_bridge/space_math.c

This file computes the places of the space part. It is pure: the program `space_math` of the tests calls it.

## The places

`matrix_of` and `compose` follow `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*, word for word.
`compose` splits the basis of the parent for the bits of `absolute`: `docs/intern/engine_subsystems/space_bridge/space_split.c.md`.

`local_of` gives the transform that keeps a place in the world under a new parent. The module needs it for `IDEATEK_KEEP_WORLD` and `IDEATEK_ORPHAN_KEEP_WORLD`.
It applies the inverse of the place of the parent, then splits the basis. A parent basis with a determinant of 0 has no inverse: `local_of` gives `false`, and the transform stays.

## Functions

| Function | Result |
| --- | --- |
| `matrix_of` | The place of a transform. |
| `compose` | The place of a child, from the place of its parent and its transform. |
| `local_of` | The transform of a place under a parent. `false` when the parent has no inverse. |
