/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_PLACE_H
    #define BL_IDEATEK_MATH_PLACE_H

    #include "math_mat3.h"

    #pragma STDC FP_CONTRACT OFF

typedef struct rigid {
    quatd_t rotation;
    vec3d_t origin;
} rigid_t;

typedef struct affine {
    mat3f_t basis;
    vec3d_t origin;
} affine_t;

static inline mat3d_t mat3f_get_wide(mat3f_t m)
{
    mat3d_t out;

    for (int column = 0; column < 3; column++)
        for (int row = 0; row < 3; row++)
            out[row][column] = (double)m[row][column];
    return (out);
}

static inline vec3d_t rigid_move(const rigid_t *place, vec3d_t point)
{
    return (place->origin + quatd_turn(place->rotation, point));
}

static inline vec3d_t rigid_move_back(const rigid_t *place, vec3d_t point)
{
    return (quatd_turn_back(place->rotation, point - place->origin));
}

static inline vec3d_t affine_move(const affine_t *place, vec3d_t point)
{
    return (place->origin + mat3d_turn(mat3f_get_wide(place->basis), point));
}

void rigid_compose(const rigid_t *parent, const rigid_t *local, rigid_t *out);
void rigid_invert(const rigid_t *place, rigid_t *out);
void affine_compose(const affine_t *parent, const affine_t *local,
    affine_t *out);
bool affine_invert(const affine_t *place, affine_t *out);

    #pragma STDC FP_CONTRACT DEFAULT

#endif
