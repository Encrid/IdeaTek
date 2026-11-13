# src/foundation/math/generic_vec3_numbers.h

This generic text moves a vector of 3 from and to 3 plain numbers.
`src/foundation/math/math_vec3.h` includes it last of its 3 generic texts.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `vec3_load` | `vec3f_load` | `vec3d_load` |
| `vec3_store` | `vec3f_store` | `vec3d_store` |

It also uses `real_t` and `vec3_t`.

## The 3 numbers

A vector type takes the room of 4 numbers, and a field of a contract takes the room of 3.
Thus the 2 functions move the parts one by one, and they never copy the bytes of a vector.
`vec3_store` writes 3 numbers. The number after them keeps its value.

## Functions

| Function | Result |
| --- | --- |
| `vec3_load` | Gives the vector of the 3 numbers at `numbers`. |
| `vec3_store` | Writes the 3 parts at `numbers`. |
