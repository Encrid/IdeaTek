/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void take_moves(const bridge_api_t *api)
{
    const space_settings_t *s = bridge_native_get_single(api, SETTINGS);
    space_ring_t *r = &space_module.ring;

    r->started = true;
    r->broken = s->moves > MOST_MOVES;
    if (r->broken) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "moves: %u is outside 0 to %u",
            s->moves, MOST_MOVES);
        return;
    }
    r->capacity = s->moves;
    r->bytes.len = s->moves * (uint32_t)sizeof(uint64_t);
    if (r->capacity) {
        r->bytes.at = api->alloc(api, r->bytes.len);
        r->ids = (uint64_t *)(api->shared_base + r->bytes.at);
    }
    r->written = true;
    publish_moves(api);
}

int run_started(const bridge_api_t *api, space_work_t work)
{
    if (!space_module.ring.started)
        take_moves(api);
    if (space_module.ring.broken)
        return (1);
    work(api);
    return (0);
}

void write_move(uint64_t id)
{
    space_ring_t *r = &space_module.ring;

    if (r->capacity) {
        r->ids[r->at] = id;
        r->at += 1;
        r->at *= r->at != r->capacity;
    }
    r->total += 1;
    r->written = true;
}

void publish_moves(const bridge_api_t *api)
{
    space_ring_t *r = &space_module.ring;
    ideatek_world_moves_t moves = { .moves = r->bytes, .total = r->total,
        .capacity = r->capacity };

    if (!r->written)
        return;
    BRIDGE_NATIVE_ATTACH(api, 0, MOVES, moves);
    r->written = false;
}
