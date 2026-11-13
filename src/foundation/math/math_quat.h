/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_QUAT_H
    #define BL_IDEATEK_MATH_QUAT_H

    #include "math_vec3.h"

    #pragma STDC FP_CONTRACT OFF

typedef float [[clang::ext_vector_type(4)]] quatf_t;
typedef double [[clang::ext_vector_type(4)]] quatd_t;

    #define real_t float
    #define real_sqrt sqrtf
    #define real_abs fabsf
    #define real_get_sin f32_get_sin
    #define real_get_cos f32_get_cos
    #define real_get_atan2 f32_get_atan2
    #define vec3_t vec3f_t
    #define vec3_get_cross vec3f_get_cross
    #define vec3_get_length vec3f_get_length
    #define quat_t quatf_t
    #define quat_get_dot quatf_get_dot
    #define quat_get_length quatf_get_length
    #define quat_get_product quatf_get_product
    #define quat_get_conjugate quatf_get_conjugate
    #define quat_get_unit quatf_get_unit
    #define quat_turn quatf_turn
    #define quat_turn_back quatf_turn_back
    #define quat_make_turn quatf_make_turn
    #define quat_get_angle quatf_get_angle
    #define quat_get_axis quatf_get_axis
    #define quat_is_finite quatf_is_finite
    #define quat_is_unit quatf_is_unit
    #define quat_get_blend quatf_get_blend
    #include "generic_quat_products.h"
    #include "generic_quat_turns.h"
    #include "generic_quat_checks.h"
    #undef real_t
    #undef real_sqrt
    #undef real_abs
    #undef real_get_sin
    #undef real_get_cos
    #undef real_get_atan2
    #undef vec3_t
    #undef vec3_get_cross
    #undef vec3_get_length
    #undef quat_t
    #undef quat_get_dot
    #undef quat_get_length
    #undef quat_get_product
    #undef quat_get_conjugate
    #undef quat_get_unit
    #undef quat_turn
    #undef quat_turn_back
    #undef quat_make_turn
    #undef quat_get_angle
    #undef quat_get_axis
    #undef quat_is_finite
    #undef quat_is_unit
    #undef quat_get_blend

    #define real_t double
    #define real_sqrt sqrt
    #define real_abs fabs
    #define real_get_sin f64_get_sin
    #define real_get_cos f64_get_cos
    #define real_get_atan2 f64_get_atan2
    #define vec3_t vec3d_t
    #define vec3_get_cross vec3d_get_cross
    #define vec3_get_length vec3d_get_length
    #define quat_t quatd_t
    #define quat_get_dot quatd_get_dot
    #define quat_get_length quatd_get_length
    #define quat_get_product quatd_get_product
    #define quat_get_conjugate quatd_get_conjugate
    #define quat_get_unit quatd_get_unit
    #define quat_turn quatd_turn
    #define quat_turn_back quatd_turn_back
    #define quat_make_turn quatd_make_turn
    #define quat_get_angle quatd_get_angle
    #define quat_get_axis quatd_get_axis
    #define quat_is_finite quatd_is_finite
    #define quat_is_unit quatd_is_unit
    #define quat_get_blend quatd_get_blend
    #include "generic_quat_products.h"
    #include "generic_quat_turns.h"
    #include "generic_quat_checks.h"
    #undef real_t
    #undef real_sqrt
    #undef real_abs
    #undef real_get_sin
    #undef real_get_cos
    #undef real_get_atan2
    #undef vec3_t
    #undef vec3_get_cross
    #undef vec3_get_length
    #undef quat_t
    #undef quat_get_dot
    #undef quat_get_length
    #undef quat_get_product
    #undef quat_get_conjugate
    #undef quat_get_unit
    #undef quat_turn
    #undef quat_turn_back
    #undef quat_make_turn
    #undef quat_get_angle
    #undef quat_get_axis
    #undef quat_is_finite
    #undef quat_is_unit
    #undef quat_get_blend

static inline quatf_t quatd_get_narrow(quatd_t q)
{
    return (__builtin_convertvector(q, quatf_t));
}

static inline quatd_t quatf_get_wide(quatf_t q)
{
    return (__builtin_convertvector(q, quatd_t));
}

quatd_t quatd_step(quatd_t q, vec3d_t speed, double time);

    #pragma STDC FP_CONTRACT DEFAULT

#endif
