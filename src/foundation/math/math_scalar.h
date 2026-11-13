/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_SCALAR_H
    #define BL_IDEATEK_MATH_SCALAR_H

    #include <math.h>
    #include <stdint.h>

    #pragma STDC FP_CONTRACT OFF

typedef double (*f64_function_t)(void *context, double value);

typedef struct f64_root {
    f64_function_t callback;
    void *context;
    double low;
    double high;
    uint32_t steps;
} f64_root_t;

    #define real_t float
    #define real_clamp f32_clamp
    #define real_blend f32_blend
    #define real_is_finite f32_is_finite
    #include "generic_scalar_range.h"
    #undef real_t
    #undef real_clamp
    #undef real_blend
    #undef real_is_finite

    #define real_t double
    #define real_clamp f64_clamp
    #define real_blend f64_blend
    #define real_is_finite f64_is_finite
    #include "generic_scalar_range.h"
    #undef real_t
    #undef real_clamp
    #undef real_blend
    #undef real_is_finite

static inline void f64_split(double value, float *high, float *low)
{
    *high = (float)value;
    *low = (float)(value - (double)*high);
}

double f64_get_sin(double angle);
double f64_get_cos(double angle);
double f64_get_atan2(double y, double x);
double f64_get_log(double value);
bool f64_find_root(const f64_root_t *ask, double *root);

static inline float f32_get_sin(float angle)
{
    return ((float)f64_get_sin((double)angle));
}

static inline float f32_get_cos(float angle)
{
    return ((float)f64_get_cos((double)angle));
}

static inline float f32_get_atan2(float y, float x)
{
    return ((float)f64_get_atan2((double)y, (double)x));
}

    #pragma STDC FP_CONTRACT DEFAULT

#endif
