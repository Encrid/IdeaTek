/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "ktx2_pure.h"

static const uint8_t IDENTIFIER[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32,
    0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

const char *ktx2_read_head(ktx2_bytes_t file, ktx2_file_t *f)
{
    uint32_t count;

    if (file.len < KTX2_HEAD)
        return ("the file is shorter than its header");
    if (memcmp(file.at, IDENTIFIER, sizeof(IDENTIFIER)) != 0)
        return ("the identifier is not the one of KTX 2.0");
    f->format = ktx2_u32(file, 12);
    f->width = ktx2_u32(file, 20);
    f->height = ktx2_u32(file, 24);
    f->depth = ktx2_u32(file, 28);
    f->layers = ktx2_u32(file, 32);
    f->faces = ktx2_u32(file, 36);
    count = ktx2_u32(file, 40);
    f->levels = count ? count : 1;
    f->scheme = ktx2_u32(file, 44);
    f->dfd_at = ktx2_u32(file, 48);
    f->dfd_len = ktx2_u32(file, 52);
    return (nullptr);
}

const char *ktx2_read(ktx2_bytes_t file, ktx2_file_t *f)
{
    const char *reason = ktx2_read_head(file, f);

    reason = reason ? reason : ktx2_check_shape(f);
    reason = reason ? reason : ktx2_read_block(file, f);
    reason = reason ? reason : ktx2_check_levels(file, f);
    return (reason);
}

ideatek_dimension_t ktx2_dimension(const ktx2_file_t *f)
{
    if (f->faces == 6)
        return (IDEATEK_DIMENSION_CUBE);
    if (f->depth)
        return (IDEATEK_DIMENSION_3D);
    if (f->layers)
        return (IDEATEK_DIMENSION_2D_ARRAY);
    return (IDEATEK_DIMENSION_2D);
}
