/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "trig_inside.h"

static constexpr int32_t IPIO2[66] = {
    0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
    0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A,
    0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129,
    0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C, 0x7026B4, 0x5F7E41,
    0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8,
    0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A, 0x6D1F6D, 0x367ECF,
    0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5,
    0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB, 0x5FB11F, 0x8D5D08,
    0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3,
    0x91615E, 0xE61B08, 0x659985, 0x5F14A0, 0x68408D, 0xFFD880,
    0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B,
};

static double get_term(const trig_large_t *state, int32_t index)
{
    int32_t at = state->first_term - state->last_input + index;

    return (at < 0 ? 0.0 : (double)IPIO2[at]);
}

double trig_large_get_sum(const trig_large_t *state, int32_t index)
{
    int32_t last = state->last_input;
    double sum = 0.0;

    for (int32_t j = 0; j <= last; j++)
        sum += state->input[j] * get_term(state, last + index - j);
    return (sum);
}

static void split_input(trig_large_t *state, double x)
{
    uint64_t bits;
    double z;

    memcpy(&bits, &x, sizeof(bits));
    bits &= UINT64_MAX >> 12;
    bits |= (uint64_t)(0x3ff + 23) << 52;
    memcpy(&z, &bits, sizeof(z));
    for (int32_t i = 0; i < 2; i++) {
        state->input[i] = (double)(int32_t)z;
        z = (z - state->input[i]) * 16777216.0;
    }
    state->input[2] = z;
    state->last_input = 2;
    while (state->input[state->last_input] == 0.0)
        state->last_input--;
}

void trig_large_start(trig_large_t *state, double x)
{
    int32_t exponent = (int32_t)((trig_get_high(x) & 0x7fffffff) >> 20);
    int32_t e0 = exponent - (0x3ff + 23);

    split_input(state, x);
    state->first_term = (e0 - 3) / 24;
    state->scale = e0 - 24 * (state->first_term + 1);
    for (int32_t i = 0; i <= TRIG_TERMS; i++)
        state->q[i] = trig_large_get_sum(state, i);
    state->last = TRIG_TERMS;
}
