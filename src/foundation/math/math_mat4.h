/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_MAT4_H
    #define BL_IDEATEK_MATH_MAT4_H

    #include "math_place.h"
    #include "math_vec4.h"

    #pragma STDC FP_CONTRACT OFF

typedef float [[clang::matrix_type(4, 4)]] mat4f_t;

typedef struct mat4f_lens {
    float fov;
    float aspect;
    float near_plane;
    float far_plane;
    bool reverse;
    bool y_down;
} mat4f_lens_t;

typedef struct mat4f_frustum {
    float left;
    float right;
    float up;
    float down;
    float near_plane;
    float far_plane;
    bool reverse;
    bool y_down;
} mat4f_frustum_t;

typedef struct mat4f_box {
    float left;
    float right;
    float bottom;
    float top;
    float near_plane;
    float far_plane;
    bool reverse;
    bool y_down;
} mat4f_box_t;

static inline vec4f_t mat4f_turn(mat4f_t m, vec4f_t v)
{
    vec4f_t out;

    out.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    out.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    out.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    out.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
    return (out);
}

static inline mat4f_t mat4f_get_product(mat4f_t a, mat4f_t b)
{
    mat4f_t out;

    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            out[row][column] = a[row][0] * b[0][column]
                + a[row][1] * b[1][column] + a[row][2] * b[2][column]
                + a[row][3] * b[3][column];
    return (out);
}

static inline mat4f_t mat4f_get_transpose(mat4f_t m)
{
    mat4f_t out;

    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            out[row][column] = m[column][row];
    return (out);
}

static inline mat4f_t mat4f_load(const float *numbers)
{
    mat4f_t out;

    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            out[row][column] = numbers[column * 4 + row];
    return (out);
}

static inline void mat4f_store(mat4f_t m, float *numbers)
{
    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            numbers[column * 4 + row] = m[row][column];
}

mat4f_t mat4f_make_perspective(const mat4f_lens_t *lens);
mat4f_t mat4f_make_frustum(const mat4f_frustum_t *frustum);
mat4f_t mat4f_make_orthographic(const mat4f_box_t *box);
mat4f_t mat4f_make_view(const rigid_t *camera, vec3d_t origin);
mat4f_t mat4f_make_model(const affine_t *place, vec3d_t origin);
bool mat4f_get_inverse(mat4f_t m, mat4f_t *inverse);
void mat4f_get_planes(mat4f_t m, vec4f_t *planes);

    #pragma STDC FP_CONTRACT DEFAULT

#endif
