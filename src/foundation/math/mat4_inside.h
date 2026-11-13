/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_MAT4_INSIDE_H
    #define BL_IDEATEK_MATH_MAT4_INSIDE_H

    #include "math_mat4.h"

typedef struct mat4_shape {
    double left;
    double right;
    double down;
    double up;
    double near_plane;
    double far_plane;
    bool reverse;
    bool y_down;
} mat4_shape_t;

mat4f_t mat4_make_zero(void);

#endif
