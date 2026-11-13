# src/foundation/math/generic_mat3_split.h

This generic text holds the split of a basis, by the 4 steps of `docs/usage/foundation/math.md`, *The split of a basis*.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `mat3_fill_unit` | `mat3f_fill_unit` | `mat3d_fill_unit` |
| `mat3_get_frame` | `mat3f_get_frame` | `mat3d_get_frame` |
| `mat3_split` | `mat3f_split` | `mat3d_split` |

## The frame

`mat3_get_frame` makes the 3 columns orthonormal by Gram-Schmidt:

```pseudo
FUNCTION mat3_get_frame(m : mat3_t) RETURNS mat3_t
   DECLARE x : vec3_t
   DECLARE y : vec3_t
   DECLARE z : vec3_t
   x <- mat3_get_column(m, 0)
   y <- mat3_get_column(m, 1)
   z <- mat3_get_column(m, 2)
   x <- mat3_fill_unit(x, y, z)
   y <- mat3_fill_unit(y - x * vec3_get_dot(y, x), z, x)
   z <- z - x * vec3_get_dot(z, x)
   z <- mat3_fill_unit(z - y * vec3_get_dot(z, y), x, y)
   RETURN mat3_make_columns(x, y, z)
ENDFUNCTION
```

`mat3_fill_unit` gives a column at the length 1. For a column of length 0 it gives the cross product of the 2 next columns, at the length 1.
A second column that is along the first one has the length 0 after the subtraction, so it is filled too.
With 2 columns of length 0 that cross product is 0, and the frame is no rotation of use.

## The split

`mat3_split` takes the scale from the lengths of the 3 columns of the basis, before the frame changes them.
If the determinant of the basis is below 0, it negates the Z column of the frame and the scale of Z.
Then the frame is a rotation, and `mat3_get_rotation` gives its quaternion.

## The branches

The cases of Reflection for the split:

| Case | Result |
| --- | --- |
| 3 columns with a length. | Each column divided by its length. |
| A first, a second or a third column of length 0. | That column is filled from the 2 others. |
| A positive determinant, and a negative determinant. | The scale of Z keeps its sign, or changes it. |

## Functions

| Function | Result |
| --- | --- |
| `mat3_fill_unit` | Gives a column at the length 1, or the cross product of 2 other columns for a length of 0. |
| `mat3_get_frame` | Gives the basis with orthonormal columns. |
| `mat3_split` | Writes the 2 results of the split. |
