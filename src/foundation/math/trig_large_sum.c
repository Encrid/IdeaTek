/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "trig_inside.h"

static constexpr double DIGIT_INVERSE = 5.9604644775390625e-08;
static constexpr double PIO2[5] = {
    1.57079625129699707031e+00, 7.54978941586159635335e-08,
    5.39030252995776476554e-15, 3.28200341580791294123e-22,
    1.27065575308067607349e-29
};

static void scale_digits(trig_large_t *state)
{
    double unit = scalbn(1.0, state->scale);

    for (int32_t i = state->last; i >= 0; i--) {
        state->q[i] = unit * (double)state->digits[i];
        unit *= DIGIT_INVERSE;
    }
}

static void sum_parts(const trig_large_t *state, double *parts)
{
    double sum;

    for (int32_t i = state->last; i >= 0; i--) {
        sum = 0.0;
        for (int32_t k = 0; k <= TRIG_TERMS && k <= state->last - i; k++)
            sum += PIO2[k] * state->q[i + k];
        parts[state->last - i] = sum;
    }
}

void trig_large_get_rest(trig_large_t *state, double *rest)
{
    double parts[TRIG_DIGITS];
    double high = 0.0;
    double low;

    scale_digits(state);
    sum_parts(state, parts);
    for (int32_t i = state->last; i >= 0; i--)
        high += parts[i];
    low = parts[0] - high;
    for (int32_t i = 1; i <= state->last; i++)
        low += parts[i];
    rest[0] = state->half == 0 ? high : -high;
    rest[1] = state->half == 0 ? low : -low;
}
