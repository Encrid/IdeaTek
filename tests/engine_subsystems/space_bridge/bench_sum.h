/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SPACE_BENCH_SUM_H
    #define TEST_SPACE_BENCH_SUM_H

    #include <stdint.h>

    #include "ideatek_space.h"

    constexpr uint32_t BENCH_FANOUT = 10;
    constexpr uint64_t BENCH_SUM_START = 0xcbf29ce484222325ull;

    uint32_t bench_parent(uint32_t node);
    double bench_x(uint32_t wave, uint32_t k);
    uint64_t bench_sum(uint64_t sum, const ideatek_world_transform_t *world);
#endif
