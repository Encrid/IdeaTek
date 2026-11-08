/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "frame_values.h"

static const struct {
    double time;
    double range;
} REFRESH[] = {
    { 1.0 / 60, 0.001 }, { 1.0 / 72, 0.00025 }, { 1.0 / 75, 0.00025 },
    { 1.0 / 85, 0.00025 }, { 1.0 / 90, 0.00025 }, { 1.0 / 100, 0.0005 },
    { 1.0 / 120, 0.0005 }, { 1.0 / 144, 0.0005 }, { 1.0 / 240, 0.001 },
};

void start_round(frame_clock_t *clock, const frame_rule_t *rule, double real,
    frame_values_t *out)
{
    double step = 1.0 / rule->rate;
    double delta = real * rule->scale * !rule->paused;
    double pending = clock->pending + delta;
    double wanted = floor(pending / step);
    bool over = wanted > rule->most_steps;
    uint32_t steps = (uint32_t)(over * rule->most_steps + !over * wanted);

    delta -= over * (pending - rule->most_steps * step);
    pending = over * (steps * step) + !over * pending;
    pending -= steps * step;
    clock->index++;
    clock->real_elapsed += real;
    clock->elapsed += delta;
    clock->pending = pending;
    out->frame = (ideatek_frame_t){ .delta = delta, .elapsed = clock->elapsed,
        .real_delta = real, .real_elapsed = clock->real_elapsed,
        .scale = rule->scale, .index = clock->index, .paused = rule->paused };
    out->tick.step = step;
    out->tick.steps = steps;
    out->tick.fraction = (float)(pending / step);
}

double smoothed(double real)
{
    double out = real;
    bool hit;

    for (uint32_t i = 0; i < sizeof(REFRESH) / sizeof(REFRESH[0]); i++) {
        hit = fabs(real - REFRESH[i].time) <= REFRESH[i].range;
        out = hit * REFRESH[i].time + !hit * out;
    }
    return (out);
}
