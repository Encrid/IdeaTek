/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef STATES_CHAIN_H
    #define STATES_CHAIN_H

    #include <stdint.h>

constexpr uint32_t STATES_DEPTH = 8;

typedef struct {
    bool has_state;
    bool has_request;
    bool scoped;
    uint32_t state;
    uint32_t value;
} chain_link_t;

typedef enum : uint32_t {
    CHAIN_STAYS = 0,
    CHAIN_LEAVES = 1,
    CHAIN_DEEP = 2
} chain_verdict_t;

bool link_leaves(const chain_link_t *link);
chain_verdict_t chain_verdict(const chain_link_t *links, uint32_t count);

#endif
