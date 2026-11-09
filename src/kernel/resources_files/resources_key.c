/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_hash.h"

constexpr uint64_t PRIME = 1099511628211ULL;

bool same_text(const res_text_t *a, const res_text_t *b)
{
    bool same = a->len == b->len;

    return (same && !memcmp(a->at, b->at, a->len));
}

uint64_t res_hash_more(uint64_t hash, const res_text_t *text)
{
    for (uint32_t i = 0; i < text->len; i++)
        hash = (hash ^ (uint8_t)text->at[i]) * PRIME;
    return (hash);
}

uint64_t res_hash_key(const res_text_t *path, const res_text_t *type)
{
    res_text_t zero = { "", 1 };
    uint64_t hash = res_hash_more(RES_HASH_START, path);

    hash = res_hash_more(hash, &zero);
    hash = res_hash_more(hash, type);
    return (hash + (hash == 0));
}
