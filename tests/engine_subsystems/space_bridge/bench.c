/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench_sum.h"
#include "space_pure.h"

typedef struct {
    ideatek_transform_t *local;
    ideatek_world_transform_t *world;
    uint32_t count;
} bench_tree_t;

static void place(bench_tree_t *t, uint32_t i)
{
    ideatek_world_transform_t local;

    get_matrix(&t->local[i], &local);
    if (i == 0)
        t->world[i] = local;
    else
        compose(&t->world[bench_parent(i)], &local, 0, &t->world[i]);
}

static void wave(bench_tree_t *t, uint32_t moved, uint32_t w)
{
    uint32_t first = t->count - moved;

    for (uint32_t k = 0; k < moved; k++)
        t->local[t->count - 1 - k].position.x = bench_x(w, k);
    for (uint32_t i = first; i < t->count; i++)
        place(t, i);
}

static double micros(const struct timespec *a, const struct timespec *b)
{
    return ((double)(b->tv_sec - a->tv_sec) * 1e6
        + (double)(b->tv_nsec - a->tv_nsec) / 1e3);
}

int main(int argc, char **argv)
{
    bench_tree_t t = { .count = argc > 1 ? (uint32_t)atoi(argv[1]) : 1000 };
    uint32_t moved = argc > 2 ? (uint32_t)atoi(argv[2]) : 10;
    uint32_t waves = argc > 3 ? (uint32_t)atoi(argv[3]) : 10;
    uint64_t sum = BENCH_SUM_START;
    struct timespec a;
    struct timespec b;

    t.local = calloc(t.count, sizeof(*t.local));
    t.world = calloc(t.count, sizeof(*t.world));
    if (!t.local || !t.world || !t.count || moved > t.count)
        return (1);
    for (uint32_t i = 0; i < t.count; i++) {
        t.local[i] = (ideatek_transform_t){ .rotation = { .w = 1.0f },
            .scale = { 1.0f, 1.0f, 1.0f } };
        place(&t, i);
    }
    timespec_get(&a, TIME_UTC);
    for (uint32_t w = 1; w <= waves; w++)
        wave(&t, moved, w);
    timespec_get(&b, TIME_UTC);
    for (uint32_t i = 0; i < t.count; i++)
        sum = bench_sum(sum, &t.world[i]);
    printf("waves %u sum %016llx\nper wave %.3f us\n", waves,
        (unsigned long long)sum, waves ? micros(&a, &b) / waves : 0.0);
    free(t.local);
    free(t.world);
    return (0);
}
