/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_MAT3_H
    #define BL_IDEATEK_MATH_MAT3_H

    #include "math_quat.h"

    #pragma STDC FP_CONTRACT OFF

typedef float [[clang::matrix_type(3, 3)]] mat3f_t;
typedef double [[clang::matrix_type(3, 3)]] mat3d_t;

    #define real_t float
    #define real_sqrt sqrtf
    #define real_max fmaxf
    #define real_copysign copysignf
    #define vec3_t vec3f_t
    #define vec3_get_dot vec3f_get_dot
    #define vec3_get_cross vec3f_get_cross
    #define vec3_get_length vec3f_get_length
    #define vec3_get_unit vec3f_get_unit
    #define quat_t quatf_t
    #define quat_get_unit quatf_get_unit
    #define mat3_t mat3f_t
    #define mat3_get_column mat3f_get_column
    #define mat3_make_columns mat3f_make_columns
    #define mat3_get_transpose mat3f_get_transpose
    #define mat3_load mat3f_load
    #define mat3_store mat3f_store
    #define mat3_turn mat3f_turn
    #define mat3_get_product mat3f_get_product
    #define mat3_get_determinant mat3f_get_determinant
    #define mat3_get_inverse mat3f_get_inverse
    #define mat3_get_part mat3f_get_part
    #define mat3_get_rotation mat3f_get_rotation
    #define mat3_make_rotation mat3f_make_rotation
    #define mat3_make_basis mat3f_make_basis
    #define mat3_fill_unit mat3f_fill_unit
    #define mat3_get_frame mat3f_get_frame
    #define mat3_split mat3f_split
    #include "generic_mat3_parts.h"
    #include "generic_mat3_products.h"
    #include "generic_mat3_rotation.h"
    #include "generic_mat3_split.h"
    #undef real_t
    #undef real_sqrt
    #undef real_max
    #undef real_copysign
    #undef vec3_t
    #undef vec3_get_dot
    #undef vec3_get_cross
    #undef vec3_get_length
    #undef vec3_get_unit
    #undef quat_t
    #undef quat_get_unit
    #undef mat3_t
    #undef mat3_get_column
    #undef mat3_make_columns
    #undef mat3_get_transpose
    #undef mat3_load
    #undef mat3_store
    #undef mat3_turn
    #undef mat3_get_product
    #undef mat3_get_determinant
    #undef mat3_get_inverse
    #undef mat3_get_part
    #undef mat3_get_rotation
    #undef mat3_make_rotation
    #undef mat3_make_basis
    #undef mat3_fill_unit
    #undef mat3_get_frame
    #undef mat3_split

    #define real_t double
    #define real_sqrt sqrt
    #define real_max fmax
    #define real_copysign copysign
    #define vec3_t vec3d_t
    #define vec3_get_dot vec3d_get_dot
    #define vec3_get_cross vec3d_get_cross
    #define vec3_get_length vec3d_get_length
    #define vec3_get_unit vec3d_get_unit
    #define quat_t quatd_t
    #define quat_get_unit quatd_get_unit
    #define mat3_t mat3d_t
    #define mat3_get_column mat3d_get_column
    #define mat3_make_columns mat3d_make_columns
    #define mat3_get_transpose mat3d_get_transpose
    #define mat3_load mat3d_load
    #define mat3_store mat3d_store
    #define mat3_turn mat3d_turn
    #define mat3_get_product mat3d_get_product
    #define mat3_get_determinant mat3d_get_determinant
    #define mat3_get_inverse mat3d_get_inverse
    #define mat3_get_part mat3d_get_part
    #define mat3_get_rotation mat3d_get_rotation
    #define mat3_make_rotation mat3d_make_rotation
    #define mat3_make_basis mat3d_make_basis
    #define mat3_fill_unit mat3d_fill_unit
    #define mat3_get_frame mat3d_get_frame
    #define mat3_split mat3d_split
    #include "generic_mat3_parts.h"
    #include "generic_mat3_products.h"
    #include "generic_mat3_rotation.h"
    #include "generic_mat3_split.h"
    #undef real_t
    #undef real_sqrt
    #undef real_max
    #undef real_copysign
    #undef vec3_t
    #undef vec3_get_dot
    #undef vec3_get_cross
    #undef vec3_get_length
    #undef vec3_get_unit
    #undef quat_t
    #undef quat_get_unit
    #undef mat3_t
    #undef mat3_get_column
    #undef mat3_make_columns
    #undef mat3_get_transpose
    #undef mat3_load
    #undef mat3_store
    #undef mat3_turn
    #undef mat3_get_product
    #undef mat3_get_determinant
    #undef mat3_get_inverse
    #undef mat3_get_part
    #undef mat3_get_rotation
    #undef mat3_make_rotation
    #undef mat3_make_basis
    #undef mat3_fill_unit
    #undef mat3_get_frame
    #undef mat3_split

    #pragma STDC FP_CONTRACT DEFAULT

#endif
