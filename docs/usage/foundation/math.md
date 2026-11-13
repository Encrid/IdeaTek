# BL-IdeaTek-Math

The math of IdeaTek: scalars, vectors, quaternions, matrices and places, with the same results on each platform.

## Install

The release of `BL-IdeaTek-Math` has the tag `math-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-Math.zip` and `sha256.txt`. `BL-IdeaTek-Math.zip` has a folder for each target.
In `BL-IdeaTek-Math`, each folder holds the static archive `math.a`, and its headers in `include`.
Each folder of `BL-IdeaTek-Math.zip` also holds this readme and the licence.

The part needs no other part and no library beside it. It includes only headers of the C library.
On x64, the archive is built for a processor with AVX2, FMA and BMI2: the level `x86_64_v3`.
A project names the zip of `BL-IdeaTek-Math` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module or a library in C includes the headers that it uses, and links `math.a`.
A module, a library and a tool can each link the part: it needs nothing of Bridge and nothing of a contract part.

### The headers

| Header | Gives |
| --- | --- |
| `math_scalar.h` | The functions of 1 number. |
| `math_vec3.h` | The vectors of 3. |
| `math_vec4.h` | The vectors of 4. |
| `math_quat.h` | The quaternions. |
| `math_mat3.h` | The matrices of 3 by 3. |
| `math_place.h` | The places: a rigid place and an affine place. |
| `math_mat4.h` | The matrices of 4 by 4, and the projections. |

### The types

A name that ends with `f` holds numbers of 32 bits. A name that ends with `d` holds numbers of 64 bits.

| Type | Content |
| --- | --- |
| `vec3f_t`, `vec3d_t` | A vector of 3: `x`, `y` and `z`. It takes the room of 4 numbers. |
| `vec4f_t` | A vector of 4: `x`, `y`, `z` and `w`. |
| `quatf_t`, `quatd_t` | A quaternion: `x`, `y`, `z` and the real part `w`. `{ 0, 0, 0, 1 }` turns nothing. |
| `mat3f_t`, `mat3d_t` | A matrix of 3 rows and 3 columns. |
| `mat4f_t` | A matrix of 4 rows and 4 columns. |
| `rigid_t` | A rigid place: a rotation `quatd_t` and an origin `vec3d_t`. |
| `affine_t` | An affine place: a basis `mat3f_t` and an origin `vec3d_t`. The basis can hold a scale and a shear. |

The vectors, the quaternions and the matrices are the vector and matrix types of clang.
`rigid_t` and `affine_t` are structures.
A quaternion and a vector of 4 of the same width are 1 type for the compiler. It does not refuse a quaternion in the place of a vector.

A matrix keeps its columns one after the other. `m[row][column]` is 1 number of the matrix.
A basis holds the X axis in column 0, the Y axis in column 1 and the Z axis in column 2.
The cross product and each rotation are right-handed. Each angle is in radians.

### The rules of a caller

1. Give a vector, a quaternion or a matrix to a function as a value.
2. Give a `rigid_t` or an `affine_t` through a pointer.
3. Use `+`, `-`, `*` and `/` between 2 vectors, or between a vector and a number. Each one works part by part.
4. Use `+` and `-` between 2 matrices, and `*` between a matrix and a number.
5. Do not use `*` between 2 matrices. No document fixes the order of its sums. Call `mat3d_get_product`.
6. Call a function of the part for each other product.
7. Read and write a part of a vector by its name, for example `v.x`. The compiler refuses the address of a part.
8. Write a sum of the parts of a vector in the order `x`, `y`, `z`, `w`.
9. Keep the rows of a component and the large arrays as plain numbers. Load them into a value to compute.

A vector of 3 takes 16 bytes in 32 bits and 32 bytes in 64 bits, and it is aligned to its size.
So rule 9 keeps a component at the size of its 3 numbers.
A block of `malloc` is aligned to 16 bytes, and a `vec3d_t` needs 32. A vector type in such a block ends the program.
So keep plain numbers in such a block, or get the block with the alignment of the type.

### The flags of a caller

| Flag | Why |
| --- | --- |
| `-fenable-matrix` | The compiler needs it for the matrix types. Each source that includes `math_mat3.h`, `math_place.h` or `math_mat4.h` takes it. |
| `-ffp-contract=off` | The compiler then joins no product and no sum into 1 operation. A caller that needs the same results on each platform takes it. |

The functions of the part join no product and no sum, also when the caller is built without the second flag.
Each header of the part ends with `#pragma STDC FP_CONTRACT DEFAULT`, which gives back the state of the flag.
So a caller sets its own state with the flag, and not with a pragma before a header of the part.

### The same results on each platform

Each function gives the same bits for the same values, on each run, each processor and each system.

- The part uses only the operations whose result the standard IEEE 754 fixes: sum, difference, product, quotient and square root.
- The part calls no function of the C library whose result differs between 2 systems. It has its own sine, cosine, arc tangent and logarithm.
- A caller can use `sqrt` of the C library: IEEE 754 fixes its result.
- No function reads the clock, a random number or an address.
- The promise holds for the default floating-point state of the thread.
  In that state each result rounds to the nearest number, and a small number is not changed to 0.
  A library on the same thread can change that state. The part `BL-IdeaTek-Processor` reads it.
- 1 thing is not the same on each processor: the sign and the payload of a NaN that an operation makes.
  A sum of 2 infinities of opposite signs gives a NaN with the sign bit on x64, and without it on arm64. So check a value with `f64_is_finite`.

### The scalars

`math_scalar.h` gives these functions. A function of `f64` works on a `double`, and a function of `f32` on a `float`.

| Function | Twin in 32 bits | Result |
| --- | --- | --- |
| `f64_clamp(value, low, high)` | `f32_clamp` | `value`, or the nearest of `low` and `high` if it is outside them. |
| `f64_blend(a, b, share)` | `f32_blend` | `a * (1 - share) + b * share`. A `share` of 0 gives `a` exactly, and 1 gives `b` exactly. |
| `f64_get_sin(angle)` | `f32_get_sin` | The sine of an angle in radians. |
| `f64_get_cos(angle)` | `f32_get_cos` | The cosine of an angle in radians. |
| `f64_get_atan2(y, x)` | `f32_get_atan2` | The angle of the point `x`, `y`, from -pi to pi. Both at 0 give 0. |
| `f64_get_log(value)` | None | The natural logarithm of a `value` above 0. |
| `f64_is_finite(value)` | `f32_is_finite` | `true` if the number is no infinity and no NaN. |
| `f64_split(value, high, low)` | None | Writes 2 numbers of 32 bits: `high` is `value` as a `float`, and `low` is the rest as a `float`. Their sum holds about 48 bits of `value`. |
| `f64_find_root(ask, root)` | None | Looks for a root of a function between 2 bounds by bisection: *The root of a function*. |

A NaN in a value gives a NaN in the result, and `f64_get_log` of 0 or less gives a NaN.
The sine and the cosine of an infinity are a NaN.
When `f64_get_sin`, `f64_get_cos`, `f64_get_atan2` or `f64_get_log` gives a NaN, it is the fixed NaN with the bits `7ff8000000000000`.
For an infinity, `f64_get_atan2` gives the angle of the limit: a quarter of pi for 2 positive infinities.
A `y` of -0 with a negative `x` gives minus pi, as the C standard tells.

### The root of a function

`f64_find_root` takes a `f64_root_t` through a pointer:

| Field | Content |
| --- | --- |
| `callback` | The function: it takes `context` and a `double`, and gives a `double`. |
| `context` | A pointer that the part gives to `callback` unread. |
| `low` | The low bound. |
| `high` | The high bound, above `low`. |
| `steps` | How many times the part halves the range: 1 or more. |

1. The part calls `callback` at `low` and at `high`.
2. If the 2 results have the same sign, it gives `false` and writes nothing.
3. Else it halves the range `steps` times, and keeps the half whose ends have different signs.
4. It writes the middle of the last range into `root`, and gives `true`.

A result of 0 at a bound is a root: the part writes that bound and gives `true`.
A result of 0 at a middle ends the search: the part writes that middle and gives `true`.
If the function gives a NaN at a middle, the part gives `false` and writes nothing.

### The vectors

`math_vec3.h` and `math_vec4.h` give these functions.

| Function | Twin in 32 bits | Result |
| --- | --- | --- |
| `vec3d_get_dot(a, b)` | `vec3f_get_dot` | The dot product. |
| `vec3d_get_cross(a, b)` | `vec3f_get_cross` | The cross product, right-handed. |
| `vec3d_get_square(a)` | `vec3f_get_square` | The square of the length. |
| `vec3d_get_length(a)` | `vec3f_get_length` | The length. |
| `vec3d_get_distance(a, b)` | `vec3f_get_distance` | The length of `b - a`. |
| `vec3d_get_unit(a)` | `vec3f_get_unit` | `a` at the length 1. A vector of length 0 stays 0. |
| `vec3d_get_blend(a, b, share)` | `vec3f_get_blend` | `f64_blend` of each part. |
| `vec3d_get_least(a, b)` | `vec3f_get_least` | The smaller number of `a` and `b` for each part. |
| `vec3d_get_most(a, b)` | `vec3f_get_most` | The larger number of `a` and `b` for each part. |
| `vec3d_is_finite(a)` | `vec3f_is_finite` | `true` if each part is finite. |
| `vec3d_load(numbers)` | `vec3f_load` | The vector of 3 plain numbers at a pointer. |
| `vec3d_store(a, numbers)` | `vec3f_store` | Writes the 3 parts as plain numbers at a pointer. |
| `vec3d_get_narrow(a)` | None | The vector as a `vec3f_t`. Each part is rounded to the nearest number of 32 bits. |
| `vec3f_get_wide(a)` | None | The vector as a `vec3d_t`, with no loss. |
| `vec3d_split(a, high, low)` | None | Writes 2 vectors of 32 bits: `f64_split` of each part. |
| None | `vec4f_get_dot(a, b)` | The dot product of 2 vectors of 4. |
| None | `vec4f_load(numbers)` | The vector of 4 plain numbers at a pointer. |
| None | `vec4f_store(a, numbers)` | Writes the 4 parts as plain numbers at a pointer. |

A length comes from the square of the length. In 64 bits, the square is 0 below a length of about 10^-162, and an infinity above about 10^154.
In 32 bits, these 2 lengths are about 10^-23 and 10^19.
Below the first length, `vec3d_get_length` gives 0 and `vec3d_get_unit` gives the vector with no change.

### The quaternions

`math_quat.h` gives these functions. A quaternion of a rotation has the length 1.

| Function | Twin in 32 bits | Result |
| --- | --- | --- |
| `quatd_get_dot(a, b)` | `quatf_get_dot` | The dot product of the 4 parts. |
| `quatd_get_length(q)` | `quatf_get_length` | The length of the 4 parts. |
| `quatd_get_product(a, b)` | `quatf_get_product` | The rotation `b`, then the rotation `a`. |
| `quatd_get_conjugate(q)` | `quatf_get_conjugate` | The opposite rotation of a quaternion of length 1. |
| `quatd_get_unit(q)` | `quatf_get_unit` | `q` at the length 1. A quaternion of length 0 gives `{ 0, 0, 0, 1 }`. |
| `quatd_turn(q, v)` | `quatf_turn` | The vector `v` turned by `q`. |
| `quatd_turn_back(q, v)` | `quatf_turn_back` | The vector `v` turned by the opposite of `q`. |
| `quatd_make_turn(axis, angle)` | `quatf_make_turn` | The rotation of `angle` radians about an axis of length 1, right-handed. |
| `quatd_get_angle(q)` | `quatf_get_angle` | The angle of the rotation, from 0 to pi. |
| `quatd_get_axis(q)` | `quatf_get_axis` | The axis of the rotation, at the length 1, for the angle from 0 to pi. A rotation of the angle 0 gives the X axis. |
| `quatd_get_blend(a, b, share)` | `quatf_get_blend` | The spherical blend from `a` to `b`, by the shorter way. A share of 0 gives `a` exactly. A share of 1 gives `b` or its opposite, which is the same rotation. |
| `quatd_step(q, speed, time)` | None | `q` after it turns at the angular speed `speed`, in radians for each second, for `time` seconds. |
| `quatd_is_finite(q)` | `quatf_is_finite` | `true` if each part is finite. |
| `quatd_is_unit(q, margin)` | `quatf_is_unit` | `true` if the length differs from 1 by `margin` at most. |
| `quatd_get_narrow(q)` | None | The quaternion as a `quatf_t`. |
| `quatf_get_wide(q)` | None | The quaternion as a `quatd_t`. |

`quatd_step` is exact for a constant speed: it turns by the angle `length of speed` times `time` about the axis of `speed`.
A speed of 0 gives `q`. The result has the length 1.
`speed` is in the space in which `q` turns a vector.

### The matrices of 3 by 3

`math_mat3.h` gives these functions.

| Function | Twin in 32 bits | Result |
| --- | --- | --- |
| `mat3d_turn(m, v)` | `mat3f_turn` | The product of the matrix and the vector. |
| `mat3d_get_product(a, b)` | `mat3f_get_product` | The product of 2 matrices: `b` first, then `a`. |
| `mat3d_get_column(m, column)` | `mat3f_get_column` | 1 column of the matrix, as a vector of 3. |
| `mat3d_make_columns(x, y, z)` | `mat3f_make_columns` | The matrix whose 3 columns are the 3 vectors. |
| `mat3d_get_transpose(m)` | `mat3f_get_transpose` | The rows as columns. |
| `mat3d_get_determinant(m)` | `mat3f_get_determinant` | The determinant. |
| `mat3d_get_inverse(m, inverse)` | `mat3f_get_inverse` | Writes the inverse and gives `true`. With a determinant of 0, it gives `false` and writes nothing. |
| `mat3d_make_rotation(q)` | `mat3f_make_rotation` | The matrix of a quaternion of length 1. |
| `mat3d_make_basis(rotation, scale)` | `mat3f_make_basis` | The basis of a rotation and a scale on each axis: each column of the rotation times its scale. |
| `mat3d_get_rotation(m)` | `mat3f_get_rotation` | The quaternion of a matrix whose columns have the length 1 and are at right angles. |
| `mat3d_get_frame(m)` | `mat3f_get_frame` | The basis with its 3 columns made orthonormal, in the order X, Y, Z. |
| `mat3d_split(m, rotation, scale)` | `mat3f_split` | Writes the 2 halves of a basis, its rotation and its scale: *The split of a basis*. |
| `mat3d_load(numbers)` | `mat3f_load` | The matrix of 9 plain numbers at a pointer, column by column. |
| `mat3d_store(m, numbers)` | `mat3f_store` | Writes the 9 numbers at a pointer, column by column. |

2 more functions are steps of the functions above, and a caller rarely needs them.
`mat3d_get_part` gives 1 part of the quaternion of `mat3d_get_rotation`, and `mat3d_fill_unit` gives 1 column of `mat3d_get_frame`.

### The split of a basis

`mat3d_split` and `mat3f_split` follow these 4 steps:

1. The scale on each axis is the length of the column of that axis.
2. The rotation is the 3 columns made orthonormal by Gram-Schmidt, in the order X, Y, Z.
3. If the determinant of the basis is less than 0, the scale of Z is negative.
4. A column of length 0 has a scale of 0. Its column in the rotation is the cross product of the 2 other columns.

A shear does not go through the split: the split gives the nearest place without a shear.

Each one writes a quaternion of length 1 into `rotation`, and a vector of the 3 scales into `scale`.
For a basis with no shear, `mat3d_make_basis` of these 2 values gives the basis again, to the rounding.

### The places

`math_place.h` gives these functions. Each takes its places through pointers.

| Function | Result |
| --- | --- |
| `rigid_compose(parent, local, out)` | Writes the place of `local` in the space where `parent` is. |
| `rigid_invert(place, out)` | Writes the opposite place. |
| `rigid_move(place, point)` | The point moved from the space of the place into the space where the place is. |
| `rigid_move_back(place, point)` | The point moved the other way. |
| `affine_compose(parent, local, out)` | Writes the place of `local` in the space where `parent` is: the product of the 2 bases, and the origin in 64 bits. |
| `affine_invert(place, out)` | Writes the opposite place and gives `true`. With a basis whose determinant is 0, it gives `false` and writes nothing. |
| `affine_move(place, point)` | The point moved from the space of the place into the space where the place is, in 64 bits. |
| `mat3f_get_wide(m)` | The matrix of 32 bits as a matrix of 64 bits, with no loss. |

The output of a function of a place can be 1 of its inputs.
`rigid_compose` does not bring the rotation back to the length 1. Call `quatd_get_unit` on a place that is composed many times.

### The matrices of 4 by 4

`math_mat4.h` gives these functions, for a renderer.

| Function | Result |
| --- | --- |
| `mat4f_turn(m, v)` | The product of the matrix and a vector of 4. |
| `mat4f_get_product(a, b)` | The product of 2 matrices: `b` first, then `a`. |
| `mat4f_get_transpose(m)` | The rows as columns. |
| `mat4f_get_inverse(m, inverse)` | Writes the inverse and gives `true`. With a determinant of 0, it gives `false` and writes nothing. |
| `mat4f_make_perspective(lens)` | The perspective projection of a `mat4f_lens_t`: 1 vertical angle. |
| `mat4f_make_frustum(frustum)` | The perspective projection of a `mat4f_frustum_t`: 4 angles. |
| `mat4f_make_orthographic(box)` | The orthographic projection of a `mat4f_box_t`. |
| `mat4f_make_view(camera, origin)` | The matrix that moves a point, given relative to `origin`, into the space of the camera. |
| `mat4f_make_model(place, origin)` | The matrix that moves a point of a place into the space whose origin is `origin`. |
| `mat4f_get_planes(m, planes)` | Writes the 6 planes of a view and projection matrix: *The planes*. |
| `mat4f_load(numbers)` | The matrix of 16 plain numbers at a pointer, column by column. |
| `mat4f_store(m, numbers)` | Writes the 16 numbers at a pointer, column by column. |

The camera of each projection looks along -Z, and its +Y is up. The depth of the clip space goes from 0 to 1.

| Field of `mat4f_lens_t` | Content |
| --- | --- |
| `fov` | The vertical field of view, in radians. |
| `aspect` | The width divided by the height. |
| `near_plane` | The distance of the near plane, above 0. |
| `far_plane` | The distance of the far plane, above `near_plane`. An infinity gives a projection with no far plane. |
| `reverse` | `true`: the depth is 1 at the near plane and 0 at the far plane. `false`: 0 at the near plane and 1 at the far plane. |
| `y_down` | `true`: +Y of the clip space points down. `false`: it points up. |

`mat4f_frustum_t` holds 4 angles in radians in place of `fov` and `aspect`: `left`, `right`, `up` and `down`.
`left` is negative on the left of the view axis, and `down` is negative below it. Its 4 other fields are those of `mat4f_lens_t`.
`mat4f_box_t` holds `left`, `right`, `bottom`, `top`, `near_plane` and `far_plane`, in meters, and the same `reverse` and `y_down`.
The names are not `near` and `far`, because the headers of Windows define these 2 words as macros.
Each projection takes its structure through a pointer, and computes in 64 bits before it rounds each number to 32 bits.

### The view and the model

`camera` of `mat4f_make_view` is the `rigid_t` of the camera, and `origin` is a `vec3d_t`.
`place` of `mat4f_make_model` is an `affine_t`, and its `origin` is a `vec3d_t` too.
Each function subtracts `origin` in 64 bits, then rounds the result to 32 bits.
So a renderer that gives the same `origin` to both keeps its precision far from the origin of the world.

A view has no scale. A caller that holds the basis of a camera splits it first, and gives the rotation.

### The planes

Each plane of `mat4f_get_planes` is a `vec4f_t`: a normal of length 1 in `x`, `y` and `z`, and a distance in `w`.
A point is inside a plane when the dot product of the plane and the point, with a `w` of 1, is 0 or more.
The function gets only the matrix, so the order of the planes is that of the clip space:

| Index | Side of the clip space | Plane of the view | With a flag |
| --- | --- | --- | --- |
| 0 | `x >= -w` | Left | |
| 1 | `x <= w` | Right | |
| 2 | `y >= -w` | Bottom | Top with `y_down`. |
| 3 | `y <= w` | Top | Bottom with `y_down`. |
| 4 | `z >= 0` | Near | Far with `reverse`. |
| 5 | `z <= w` | Far | Near with `reverse`. |

A projection with no far plane has no such plane to give. Its normal would have the length 0.
The function then writes `{ 0, 0, 0, 1 }` for that plane, which each point is inside.

### The messages

The part writes no line. A function that cannot give its result tells it by its result, as each table says.
