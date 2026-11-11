/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>

#include "bench_sum.h"

uint32_t bench_parent(uint32_t node)
{
    return ((node - 1) / BENCH_FANOUT);
}

double bench_x(uint32_t wave, uint32_t k)
{
    return ((double)wave * 0.001 + (double)k);
}

uint64_t bench_sum(uint64_t sum, const ideatek_world_transform_t *world)
{
    const uint8_t *b = (const uint8_t *)world;
    size_t end = offsetof(ideatek_world_transform_t, basis)
        + sizeof(world->basis);

    for (size_t k = 0; k < end; k++)
        sum = (sum ^ b[k]) * 0x100000001b3ull;
    return (sum);
}
