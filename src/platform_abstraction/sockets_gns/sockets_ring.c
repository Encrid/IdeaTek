/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_values.h"

static bool ring_fits(const sockets_ring_t *ring, uint32_t need,
    uint32_t *start)
{
    bool wraps = ring->head + need > ring->size;
    bool after = ring->head >= ring->tail;

    *start = wraps ? 0 : ring->head;
    if (need > ring->size)
        return (false);
    if (after && !wraps)
        return (true);
    if (after)
        return (need < ring->tail);
    return (ring->head + need < ring->tail);
}

bool ring_put(uint8_t *base, sockets_ring_t *ring, const void *bytes,
    uint32_t len)
{
    uint8_t *at = base + ring->at;
    uint32_t need = 4 + len;
    uint32_t start;

    if (!ring_fits(ring, need, &start))
        return (false);
    if (start == 0 && ring->head != 0 && ring->head + 4 <= ring->size)
        memset(at + ring->head, 0xff, 4);
    memcpy(at + start, &len, 4);
    memcpy(at + start + 4, bytes, len);
    ring->head = start + need;
    return (true);
}

uint32_t ring_take(const uint8_t *base, sockets_ring_t *ring,
    const uint8_t **bytes)
{
    const uint8_t *at = base + ring->at;
    uint32_t len = 0;

    if (ring->tail == ring->head)
        return (0);
    if (ring->tail + 4 > ring->size || at[ring->tail] == 0xff)
        ring->tail = 0;
    memcpy(&len, at + ring->tail, 4);
    *bytes = at + ring->tail + 4;
    ring->tail += 4 + len;
    return (len);
}
