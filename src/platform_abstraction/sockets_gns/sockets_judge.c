/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

void attach_problem(const bridge_api_t *api, uint64_t entity,
    const char *text)
{
    ideatek_connection_state_t state = {};

    state.status = IDEATEK_CONNECTION_STATUS_PROBLEM;
    strncpy(state.end_text, text, sizeof(state.end_text) - 1);
    BRIDGE_NATIVE_ATTACH(api, entity, CONNECTION_STATE, state);
}

static bool judge_rates(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r)
{
    char field[64];
    char range[64];
    bool min_in = (r->send_rate_min >= 1024) & (r->send_rate_min <= 268435456);
    bool max_ok = (r->send_rate_max >= 1024)
        & (r->send_rate_max <= 268435456);
    bool ordered = r->send_rate_min <= r->send_rate_max;

    if (!max_ok)
        snprintf(field, sizeof(field), "send_rate_max %u", r->send_rate_max);
    else
        snprintf(field, sizeof(field), "send_rate_min %u", r->send_rate_min);
    snprintf(range, sizeof(range), "1024 to %u", max_ok && min_in
        ? r->send_rate_max : 268435456);
    if (!max_ok || !min_in || !ordered)
        say_request(api, entity, field, range);
    return (max_ok & min_in & ordered);
}

static bool judge_close(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r)
{
    uint32_t at = find_entity(entity);
    bool symmetric = at != sockets_module.connection_max
        && sockets_module.connections[at].symmetric;
    bool reason_ok = (r->reason >= GNS_END_APP_MIN)
        & (r->reason <= GNS_END_APP_MAX);
    char field[64];

    if (!reason_ok) {
        snprintf(field, sizeof(field), "reason %d", r->reason);
        say_request(api, entity, field, "1000 to 2999");
        return (false);
    }
    if (r->linger && symmetric) {
        say_request(api, entity, "linger 1", "a symmetric connection");
        return (false);
    }
    return (true);
}

bool judge(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r)
{
    char field[64];

    if (r->action > IDEATEK_CONNECTION_ACTION_PAIR) {
        snprintf(field, sizeof(field), "action %u", r->action);
        say_request(api, entity, field, "0 to 4");
        return (false);
    }
    if (r->action == IDEATEK_CONNECTION_ACTION_CLOSE)
        return (judge_close(api, entity, r));
    return (judge_rates(api, entity, r));
}
