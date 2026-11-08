/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <string.h>

#include "frame_inside.h"

static const char *const NAMES[4] = { "scale", "rate", "max_rate", "paused" };

static void refuse(const bridge_api_t *api, const char *name, double value)
{
    bridge_native_say(api, BRIDGE_LOG_WARN, "%s %g is refused", name, value);
}

static void copy_fields(const ideatek_time_request_t *r, const bool ok[4])
{
    frame_rule_t *rule = &frame_module.rule;

    memcpy(&rule->scale, &r->scale, ok[0] * sizeof(rule->scale));
    memcpy(&rule->rate, &r->rate, ok[1] * sizeof(rule->rate));
    memcpy(&frame_module.max_rate, &r->max_rate,
        ok[2] * sizeof(frame_module.max_rate));
    memcpy(&rule->paused, &r->paused, ok[3] * sizeof(rule->paused));
}

static void refuse_fields(const bridge_api_t *api,
    const ideatek_time_request_t *r, const bool ok[4])
{
    const double values[4] = { r->scale, r->rate, r->max_rate, r->paused };

    for (int i = 0; i < 4; i++)
        if (!ok[i])
            refuse(api, NAMES[i], values[i]);
}

void apply_request(const bridge_api_t *api)
{
    const ideatek_time_request_t *r = bridge_native_get_single(api,
        TIME_REQUEST);
    bool ok[4];

    if (!memcmp(r, &frame_module.seen, sizeof(*r)))
        return;
    frame_module.seen = *r;
    ok[0] = !!isfinite(r->scale) & (r->scale >= 0);
    ok[1] = (r->rate >= 1) & (r->rate <= 1000);
    ok[2] = r->max_rate <= 1000;
    ok[3] = r->paused <= 1;
    copy_fields(r, ok);
    refuse_fields(api, r, ok);
}
