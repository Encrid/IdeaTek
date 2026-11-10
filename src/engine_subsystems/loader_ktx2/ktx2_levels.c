/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdckdint.h>
#include <string.h>

#include "ktx2_pure.h"

static uint64_t count_blocks(uint32_t size, uint32_t level, uint32_t block)
{
    uint64_t side = size >> level;

    side = side ? side : 1;
    return ((side + block - 1) / block);
}

bool ktx2_level_bytes(const ktx2_file_t *f, uint32_t level, uint64_t *bytes)
{
    uint64_t parts = (uint64_t)(f->layers ? f->layers : 1) * f->faces;
    bool over = ckd_mul(bytes, count_blocks(f->width, level, f->block[0]),
        count_blocks(f->height, level, f->block[1]));

    over |= ckd_mul(bytes, *bytes, count_blocks(f->depth, level,
        f->block[2]));
    over |= ckd_mul(bytes, *bytes, (uint64_t)f->block_bytes);
    over |= ckd_mul(bytes, *bytes, parts);
    return (!over);
}

static const char *check_level(ktx2_bytes_t file, ktx2_file_t *f,
    uint32_t level)
{
    uint64_t entry = KTX2_HEAD + (uint64_t)KTX2_LEVEL * level;
    ktx2_range_t *r = &f->level[level];
    uint64_t end;
    uint64_t bytes;

    r->at = ktx2_u64(file, entry);
    r->len = ktx2_u64(file, entry + 8);
    if (ckd_add(&end, r->at, r->len) || end > file.len)
        return ("a level is outside the file");
    if (ktx2_u64(file, entry + 16) != r->len)
        return ("uncompressedByteLength is not byteLength");
    if (!ktx2_level_bytes(f, level, &bytes) || bytes != r->len)
        return ("a level has not the bytes of its size");
    f->texel_bytes += r->len;
    return (nullptr);
}

const char *ktx2_check_levels(ktx2_bytes_t file, ktx2_file_t *f)
{
    const char *reason = nullptr;

    if (KTX2_HEAD + (uint64_t)KTX2_LEVEL * f->levels > file.len)
        return ("the level index is outside the file");
    f->texel_bytes = 0;
    for (uint32_t k = 0; k < f->levels && !reason; k++)
        reason = check_level(file, f, k);
    return (reason);
}

void ktx2_copy(ktx2_bytes_t file, const ktx2_file_t *f, uint8_t *to)
{
    uint64_t at = 0;

    for (uint32_t k = 0; k < f->levels; k++) {
        memcpy(to + at, file.at + f->level[k].at, f->level[k].len);
        at += f->level[k].len;
    }
}
