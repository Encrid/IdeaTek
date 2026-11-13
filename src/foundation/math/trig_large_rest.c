/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "trig_inside.h"

static constexpr double DIGIT = 16777216.0;
static constexpr double DIGIT_INVERSE = 5.9604644775390625e-08;

static bool extend_terms(trig_large_t *state)
{
    int32_t found = 0;
    int32_t more = 1;

    if (state->fraction != 0.0)
        return (false);
    for (int32_t i = state->last - 1; i >= TRIG_TERMS; i--)
        found |= state->digits[i];
    if (found != 0)
        return (false);
    while (state->digits[TRIG_TERMS - more] == 0)
        more++;
    for (int32_t i = state->last + 1; i <= state->last + more; i++)
        state->q[i] = trig_large_get_sum(state, i);
    state->last += more;
    return (true);
}

static void chop_digits(trig_large_t *state)
{
    state->last -= 1;
    state->scale -= 24;
    while (state->digits[state->last] == 0) {
        state->last--;
        state->scale -= 24;
    }
}

static void break_fraction(trig_large_t *state)
{
    double z = scalbn(state->fraction, -state->scale);
    double carry;

    if (z < DIGIT) {
        state->digits[state->last] = (int32_t)z;
        return;
    }
    carry = (double)(int32_t)(DIGIT_INVERSE * z);
    state->digits[state->last] = (int32_t)(z - DIGIT * carry);
    state->last += 1;
    state->scale += 24;
    state->digits[state->last] = (int32_t)carry;
}

int trig_reduce_large(double x, double *rest)
{
    trig_large_t state;
    int count;

    trig_large_start(&state, x);
    do {
        trig_large_count(&state);
    } while (extend_terms(&state));
    if (state.fraction == 0.0)
        chop_digits(&state);
    else
        break_fraction(&state);
    trig_large_get_rest(&state, rest);
    count = (int)(state.count & 7);
    if (!signbit(x))
        return (count);
    rest[0] = -rest[0];
    rest[1] = -rest[1];
    return (-count);
}
