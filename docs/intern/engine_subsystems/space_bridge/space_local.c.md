# src/engine_subsystems/space_bridge/space_local.c

This file gives the transform that keeps a place in the world under a parent. As the math of the places, it holds no state, and `space_math` checks it too.

`get_local` applies the inverse of the place of the parent, then splits the basis: `docs/intern/engine_subsystems/space_bridge/space_split.c.md`. It ignores `absolute`.
A basis of the parent with a determinant of 0 has no inverse: `get_local` gives `false`, and the caller keeps the transform.
The inverse is `mat3f_get_inverse` of the math part. The position is computed in 64 bits, with the inverse widened by `mat3f_get_wide`.

## Functions

| Function | Result |
| --- | --- |
| `get_local` | The transform under the parent. `false` when the parent has no inverse. |
