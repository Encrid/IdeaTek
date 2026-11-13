# src/foundation/math/place_rigid.c

This file composes and inverts a rigid place of `src/foundation/math/math_place.h`.

## The compose

`rigid_compose` chains 2 places. `local` is given inside `parent`, and the result is given outside `parent`:

- the rotation is the product of the rotation of `parent` and the rotation of `local`;
- the origin is the origin of `local`, moved by `parent` with `rigid_move`.

The function computes the origin before it writes the rotation. Thus `out` can be `parent`.
The product is not divided by its length. A chain of places is as long as a tree is deep, so the error stays near the rounding.
A caller that composes the same place again and again calls `quatd_get_unit`.

## The inverse

`rigid_invert` gives the conjugate as the rotation. The origin is the origin of the place turned by that conjugate, with the opposite sign.

## Functions

| Function | Result |
| --- | --- |
| `rigid_compose` | Writes the place of `local` in the outer space of `parent`. |
| `rigid_invert` | Writes the place that undoes `place`. |
