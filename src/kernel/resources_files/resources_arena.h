/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_ARENA_H
    #define RESOURCES_ARENA_H

    #include <stdint.h>

typedef struct {
    uint32_t start;
    uint32_t size;
    uint32_t resource;
    uint32_t pad;
} res_block_t;

typedef struct {
    res_block_t *blocks;
    uint32_t count;
    uint32_t used;
    uint32_t size;
} res_blocks_t;

bool blocks_take(res_blocks_t *list, uint32_t size, uint32_t resource,
    uint32_t *start);
void blocks_free(res_blocks_t *list, uint32_t resource);
uint32_t blocks_room(const res_blocks_t *list);

#endif
