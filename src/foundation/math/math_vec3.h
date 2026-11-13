/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_VEC3_H
    #define BL_IDEATEK_MATH_VEC3_H

    #include "math_scalar.h"

    #pragma STDC FP_CONTRACT OFF

typedef float [[clang::ext_vector_type(3)]] vec3f_t;
typedef double [[clang::ext_vector_type(3)]] vec3d_t;

    #define real_t float
    #define real_sqrt sqrtf
    #define vec3_t vec3f_t
    #define vec3_get_dot vec3f_get_dot
    #define vec3_get_cross vec3f_get_cross
    #define vec3_get_square vec3f_get_square
    #define vec3_get_length vec3f_get_length
    #define vec3_get_distance vec3f_get_distance
    #define vec3_get_unit vec3f_get_unit
    #define vec3_get_blend vec3f_get_blend
    #define vec3_get_least vec3f_get_least
    #define vec3_get_most vec3f_get_most
    #define vec3_is_finite vec3f_is_finite
    #define vec3_load vec3f_load
    #define vec3_store vec3f_store
    #include "generic_vec3_products.h"
    #include "generic_vec3_shapes.h"
    #include "generic_vec3_numbers.h"
    #undef real_t
    #undef real_sqrt
    #undef vec3_t
    #undef vec3_get_dot
    #undef vec3_get_cross
    #undef vec3_get_square
    #undef vec3_get_length
    #undef vec3_get_distance
    #undef vec3_get_unit
    #undef vec3_get_blend
    #undef vec3_get_least
    #undef vec3_get_most
    #undef vec3_is_finite
    #undef vec3_load
    #undef vec3_store

    #define real_t double
    #define real_sqrt sqrt
    #define vec3_t vec3d_t
    #define vec3_get_dot vec3d_get_dot
    #define vec3_get_cross vec3d_get_cross
    #define vec3_get_square vec3d_get_square
    #define vec3_get_length vec3d_get_length
    #define vec3_get_distance vec3d_get_distance
    #define vec3_get_unit vec3d_get_unit
    #define vec3_get_blend vec3d_get_blend
    #define vec3_get_least vec3d_get_least
    #define vec3_get_most vec3d_get_most
    #define vec3_is_finite vec3d_is_finite
    #define vec3_load vec3d_load
    #define vec3_store vec3d_store
    #include "generic_vec3_products.h"
    #include "generic_vec3_shapes.h"
    #include "generic_vec3_numbers.h"
    #undef real_t
    #undef real_sqrt
    #undef vec3_t
    #undef vec3_get_dot
    #undef vec3_get_cross
    #undef vec3_get_square
    #undef vec3_get_length
    #undef vec3_get_distance
    #undef vec3_get_unit
    #undef vec3_get_blend
    #undef vec3_get_least
    #undef vec3_get_most
    #undef vec3_is_finite
    #undef vec3_load
    #undef vec3_store

static inline vec3f_t vec3d_get_narrow(vec3d_t a)
{
    return (__builtin_convertvector(a, vec3f_t));
}

static inline vec3d_t vec3f_get_wide(vec3f_t a)
{
    return (__builtin_convertvector(a, vec3d_t));
}

static inline void vec3d_split(vec3d_t a, vec3f_t *high, vec3f_t *low)
{
    *high = vec3d_get_narrow(a);
    *low = vec3d_get_narrow(a - vec3f_get_wide(*high));
}

    #pragma STDC FP_CONTRACT DEFAULT

#endif
