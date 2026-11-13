/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "mat4_inside.h"

mat4f_t mat4_make_zero(void)
{
    mat4f_t out;

    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            out[row][column] = 0.0f;
    return (out);
}

static mat4f_t make_place(mat3f_t basis, vec3d_t origin)
{
    mat4f_t out = mat4_make_zero();

    for (int row = 0; row < 3; row++) {
        out[row][0] = basis[row][0];
        out[row][1] = basis[row][1];
        out[row][2] = basis[row][2];
        out[row][3] = (float)origin[row];
    }
    out[3][3] = 1.0f;
    return (out);
}

mat4f_t mat4f_make_orthographic(const mat4f_box_t *box)
{
    double width = (double)box->right - (double)box->left;
    double height = (double)box->top - (double)box->bottom;
    double range = (double)box->far_plane - (double)box->near_plane;
    double flip = box->y_down ? -1.0 : 1.0;
    double depth = box->reverse ? 1.0 : -1.0;
    double start = box->reverse ? box->far_plane : -box->near_plane;
    mat4f_t out = mat4_make_zero();

    out[0][0] = (float)(2.0 / width);
    out[0][3] = (float)(-((double)box->right + (double)box->left) / width);
    out[1][1] = (float)(flip * 2.0 / height);
    out[1][3] = (float)(-flip * ((double)box->top + box->bottom) / height);
    out[2][2] = (float)(depth / range);
    out[2][3] = (float)(start / range);
    out[3][3] = 1.0f;
    return (out);
}

mat4f_t mat4f_make_view(const rigid_t *camera, vec3d_t origin)
{
    quatd_t back = quatd_get_conjugate(camera->rotation);
    mat3d_t turn = mat3d_make_rotation(back);
    vec3d_t from = -quatd_turn(back, camera->origin - origin);
    mat3f_t basis;

    for (int column = 0; column < 3; column++)
        for (int row = 0; row < 3; row++)
            basis[row][column] = (float)turn[row][column];
    return (make_place(basis, from));
}

mat4f_t mat4f_make_model(const affine_t *place, vec3d_t origin)
{
    return (make_place(place->basis, place->origin - origin));
}
