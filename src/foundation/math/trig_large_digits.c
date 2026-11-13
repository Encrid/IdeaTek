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

static void distill(trig_large_t *state)
{
    double z = state->q[state->last];
    double carry;
    int32_t i = 0;

    for (int32_t j = state->last; j > 0; j--) {
        carry = (double)(int32_t)(DIGIT_INVERSE * z);
        state->digits[i] = (int32_t)(z - DIGIT * carry);
        z = state->q[j - 1] + carry;
        i++;
    }
    state->fraction = z;
}

static void take_count(trig_large_t *state)
{
    int32_t *top = &state->digits[state->last - 1];
    int32_t q0 = state->scale;
    double z = scalbn(state->fraction, q0);
    int32_t extra;

    z -= 8.0 * floor(z * 0.125);
    state->count = (int32_t)z;
    state->fraction = z - (double)state->count;
    state->half = 0;
    if (q0 > 0) {
        extra = *top >> (24 - q0);
        state->count += extra;
        *top -= extra << (24 - q0);
        state->half = *top >> (23 - q0);
    } else if (q0 == 0)
        state->half = *top >> 23;
    else if (state->fraction >= 0.5)
        state->half = 2;
}

static int32_t borrow_digits(trig_large_t *state)
{
    int32_t carry = 0;
    int32_t digit;

    for (int32_t i = 0; i < state->last; i++) {
        digit = state->digits[i];
        if (carry)
            state->digits[i] = 0xffffff - digit;
        else if (digit != 0) {
            carry = 1;
            state->digits[i] = 0x1000000 - digit;
        }
    }
    return (carry);
}

static void negate_fraction(trig_large_t *state)
{
    int32_t carry = borrow_digits(state);

    state->count += 1;
    if (state->scale == 1)
        state->digits[state->last - 1] &= 0x7fffff;
    if (state->scale == 2)
        state->digits[state->last - 1] &= 0x3fffff;
    if (state->half != 2)
        return;
    state->fraction = 1.0 - state->fraction;
    if (carry != 0)
        state->fraction -= scalbn(1.0, state->scale);
}

void trig_large_count(trig_large_t *state)
{
    distill(state);
    take_count(state);
    if (state->half > 0)
        negate_fraction(state);
}
