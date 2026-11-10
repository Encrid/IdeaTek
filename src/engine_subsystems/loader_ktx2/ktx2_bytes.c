/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "ktx2_pure.h"

uint32_t ktx2_u32(ktx2_bytes_t file, uint64_t at)
{
    const uint8_t *b = file.at + at;

    return ((uint32_t)b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16
        | (uint32_t)b[3] << 24);
}

uint64_t ktx2_u64(ktx2_bytes_t file, uint64_t at)
{
    return ((uint64_t)ktx2_u32(file, at)
        | (uint64_t)ktx2_u32(file, at + 4) << 32);
}

bool ktx2_same(ktx2_bytes_t text, const char *word)
{
    size_t len = strlen(word);

    return (text.len == len && memcmp(text.at, word, len) == 0);
}
