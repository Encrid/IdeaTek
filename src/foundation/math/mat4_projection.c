/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "mat4_inside.h"

static double get_tangent(double angle)
{
    return (f64_get_sin(angle) / f64_get_cos(angle));
}

static void fill_depth(const mat4_shape_t *shape, mat4f_t *out)
{
    double near_plane = shape->near_plane;
    double far_plane = shape->far_plane;
    double range = far_plane - near_plane;
    double scale = shape->reverse ? near_plane : -far_plane;
    double other = shape->reverse ? far_plane : near_plane;

    if (isinf(far_plane)) {
        (*out)[2][2] = shape->reverse ? 0.0f : -1.0f;
        (*out)[2][3] = (float)(shape->reverse ? near_plane : -near_plane);
        return;
    }
    (*out)[2][2] = (float)(scale / range);
    (*out)[2][3] = (float)(scale * other / range);
}

static mat4f_t make_shape(const mat4_shape_t *shape)
{
    double width = shape->right - shape->left;
    double height = shape->up - shape->down;
    double flip = shape->y_down ? -1.0 : 1.0;
    mat4f_t out = mat4_make_zero();

    out[0][0] = (float)(2.0 / width);
    out[0][2] = (float)((shape->right + shape->left) / width);
    out[1][1] = (float)(flip * 2.0 / height);
    out[1][2] = (float)(flip * (shape->up + shape->down) / height);
    out[3][2] = -1.0f;
    fill_depth(shape, &out);
    return (out);
}

mat4f_t mat4f_make_perspective(const mat4f_lens_t *lens)
{
    double up = get_tangent((double)lens->fov * 0.5);
    double right = up * (double)lens->aspect;
    mat4_shape_t shape = { .left = -right, .right = right, .down = -up,
        .up = up, .near_plane = (double)lens->near_plane,
        .far_plane = (double)lens->far_plane, .reverse = lens->reverse,
        .y_down = lens->y_down };

    return (make_shape(&shape));
}

mat4f_t mat4f_make_frustum(const mat4f_frustum_t *frustum)
{
    mat4_shape_t shape = { .left = get_tangent((double)frustum->left),
        .right = get_tangent((double)frustum->right),
        .down = get_tangent((double)frustum->down),
        .up = get_tangent((double)frustum->up),
        .near_plane = (double)frustum->near_plane,
        .far_plane = (double)frustum->far_plane,
        .reverse = frustum->reverse, .y_down = frustum->y_down };

    return (make_shape(&shape));
}
