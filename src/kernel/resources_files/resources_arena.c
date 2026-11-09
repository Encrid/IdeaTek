/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_arena.h"

static void insert(res_blocks_t *list, uint32_t i, const res_block_t *block)
{
    memmove(list->blocks + i + 1, list->blocks + i,
        (size_t)(list->used - i) * sizeof(*block));
    list->blocks[i] = *block;
    list->used++;
}

bool blocks_take(res_blocks_t *list, uint32_t size, uint32_t resource,
    uint32_t *start)
{
    res_block_t block = { .size = size, .resource = resource };
    uint32_t at = 0;
    uint32_t i = 0;
    bool fits;

    if (list->used == list->count)
        return (false);
    for (; i < list->used; i++) {
        if (list->blocks[i].start - at >= size)
            break;
        at = list->blocks[i].start + list->blocks[i].size;
    }
    fits = (i < list->used) | (list->size - at >= size);
    block.start = at;
    *start = at;
    if (fits)
        insert(list, i, &block);
    return (fits);
}

void blocks_free(res_blocks_t *list, uint32_t resource)
{
    uint32_t kept = 0;
    bool hit;

    for (uint32_t i = 0; i < list->used; i++) {
        hit = list->blocks[i].resource == resource;
        list->blocks[kept] = list->blocks[i];
        kept += !hit;
    }
    list->used = kept;
}

uint32_t blocks_room(const res_blocks_t *list)
{
    uint32_t at = 0;
    uint32_t room = 0;
    uint32_t gap;

    for (uint32_t i = 0; i < list->used; i++) {
        gap = list->blocks[i].start - at;
        room = gap > room ? gap : room;
        at = list->blocks[i].start + list->blocks[i].size;
    }
    gap = list->size - at;
    return (gap > room ? gap : room);
}
