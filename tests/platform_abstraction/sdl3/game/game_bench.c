/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_bench.h"

#if defined(GAME_BENCH)

#include "bench_sum.h"

constexpr uint32_t MOST = 100000;
constexpr uint32_t SPAWNS = 1000;

static uint64_t nodes[MOST];
static ideatek_transform_t locals[MOST];
static uint32_t count;
static uint32_t target;
static uint32_t moved;
static uint32_t waves;

static void grow(const bridge_api_t *api)
{
    ideatek_parent_t parent = {};
    uint32_t end = count + SPAWNS < target ? count + SPAWNS : target;

    for (; count < end; count++) {
        nodes[count] = api->spawn(api);
        locals[count] = (ideatek_transform_t){ .rotation = { .w = 1.0f },
            .scale = { 1.0f, 1.0f, 1.0f } };
        BRIDGE_NATIVE_ATTACH(api, nodes[count], game_space_slots.transform,
            locals[count]);
        if (count == 0)
            continue;
        parent.entity.id = nodes[bench_parent(count)];
        BRIDGE_NATIVE_ATTACH(api, nodes[count], game_space_slots.parent,
            parent);
    }
}

static void wave(const bridge_api_t *api)
{
    uint32_t i;

    waves++;
    for (uint32_t k = 0; k < moved; k++) {
        i = count - 1 - k;
        locals[i].position.x = bench_x(waves, k);
        BRIDGE_NATIVE_ATTACH(api, nodes[i], game_space_slots.transform,
            locals[i]);
    }
}

bool game_bench_step(const bridge_api_t *api, const char *verb, char *args)
{
    uint32_t n = (uint32_t)atoi(args);

    (void)api;
    if (!strcmp(verb, "swarm"))
        target = n < MOST ? n : MOST;
    else if (!strcmp(verb, "wave"))
        moved = n;
    else if (!strcmp(verb, "still"))
        moved = 0;
    else
        return (false);
    return (true);
}

void game_bench_play(const bridge_api_t *api)
{
    if (count < target)
        grow(api);
    else if (moved && moved <= count)
        wave(api);
}

void game_bench_end(const bridge_api_t *api)
{
    uint64_t sum = BENCH_SUM_START;
    const ideatek_world_transform_t *w;

    for (uint32_t i = 0; i < count; i++) {
        w = game_cell(api, nodes[i], game_space_slots.world);
        if (w)
            sum = bench_sum(sum, w);
    }
    bridge_native_say(api, BRIDGE_LOG_INFO, "waves %u sum %016llx", waves,
        (unsigned long long)sum);
}

#endif
