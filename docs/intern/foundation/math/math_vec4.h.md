# src/foundation/math/math_vec4.h

This header gives the vector of 4 numbers of 32 bits, for a renderer.
Such a vector is a point with its `w`, a plane, or a row of a matrix of 4 by 4.
`docs/usage/foundation/math.md`, *The vectors*, lists its functions.

## The type

`vec4f_t` is a vector type of clang with 4 parts, so it takes 16 bytes and fits 1 register.
A quaternion of 32 bits is the same type for the compiler.

The vector has 1 width only. Thus its functions are plain functions of this header, with no generic text.

## The dot product

`vec4f_get_dot` adds the 4 products in the order `x`, `y`, `z`, `w`, and joins no product with a sum.
The header has its own pair of pragmas for that: `docs/intern/foundation/math/math_scalar.h.md`, *The fusion*.

## Functions

| Function | Result |
| --- | --- |
| `vec4f_get_dot` | Gives the sum of the 4 products of the parts. |
| `vec4f_load` | Gives the vector of the 4 numbers at `numbers`. |
| `vec4f_store` | Writes the 4 parts at `numbers`. |
