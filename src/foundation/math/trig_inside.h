/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BL_IDEATEK_MATH_TRIG_INSIDE_H
    #define BL_IDEATEK_MATH_TRIG_INSIDE_H

    #include <stdint.h>

constexpr int32_t TRIG_TERMS = 4;
constexpr int32_t TRIG_DIGITS = 20;

typedef struct trig_large {
    double input[3];
    double q[TRIG_DIGITS];
    double fraction;
    int32_t digits[TRIG_DIGITS];
    int32_t last_input;
    int32_t first_term;
    int32_t scale;
    int32_t last;
    int32_t count;
    int32_t half;
} trig_large_t;

uint32_t trig_get_high(double value);
double trig_get_sine(double x, double tail, bool has_tail);
double trig_get_cosine(double x, double tail);
double trig_get_arc_tangent(double x);
int trig_reduce(double x, double *rest);
int trig_reduce_medium(double x, double *rest);
int trig_reduce_large(double x, double *rest);
void trig_large_start(trig_large_t *state, double x);
double trig_large_get_sum(const trig_large_t *state, int32_t index);
void trig_large_count(trig_large_t *state);
void trig_large_get_rest(trig_large_t *state, double *rest);

#endif
