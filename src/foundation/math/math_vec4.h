/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_VEC4_H
    #define BL_IDEATEK_MATH_VEC4_H

    #pragma STDC FP_CONTRACT OFF

typedef float [[clang::ext_vector_type(4)]] vec4f_t;

static inline float vec4f_get_dot(vec4f_t a, vec4f_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

static inline vec4f_t vec4f_load(const float *numbers)
{
    vec4f_t out;

    out.x = numbers[0];
    out.y = numbers[1];
    out.z = numbers[2];
    out.w = numbers[3];
    return (out);
}

static inline void vec4f_store(vec4f_t a, float *numbers)
{
    numbers[0] = a.x;
    numbers[1] = a.y;
    numbers[2] = a.z;
    numbers[3] = a.w;
}

    #pragma STDC FP_CONTRACT DEFAULT

#endif
