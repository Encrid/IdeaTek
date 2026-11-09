/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_HASH_H
    #define RESOURCES_HASH_H

    #include <stdint.h>

    #include "resources_paths.h"

constexpr uint64_t RES_HASH_START = 14695981039346656037ULL;

typedef struct {
    uint64_t key;
    uint32_t value;
    uint32_t pad;
} res_slot_t;

typedef struct {
    res_slot_t *slots;
    uint32_t count;
    uint32_t used;
} res_map_t;

uint64_t res_hash_more(uint64_t hash, const res_text_t *text);
uint64_t res_hash_key(const res_text_t *path, const res_text_t *type);
uint32_t map_find(const res_map_t *map, uint64_t key);
bool map_put(res_map_t *map, uint64_t key, uint32_t value);
void map_drop(res_map_t *map, uint64_t key);
void map_clear(res_map_t *map);

#endif
