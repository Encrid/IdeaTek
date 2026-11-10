/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_pure.h"

const char *ktx2_read_block(ktx2_bytes_t file, ktx2_file_t *f)
{
    const uint8_t *dfd;

    if ((uint64_t)f->dfd_at + f->dfd_len > file.len)
        return ("the data format descriptor is outside the file");
    if (f->dfd_len < KTX2_DFD_MIN || ktx2_u32(file, f->dfd_at + 4) != 0)
        return ("the data format descriptor has no basic block");
    dfd = file.at + f->dfd_at;
    for (uint32_t k = 0; k < 3; k++)
        f->block[k] = (uint32_t)dfd[16 + k] + 1;
    f->block_bytes = dfd[20];
    if (f->block_bytes == 0)
        return ("bytesPlane0 is 0");
    return (nullptr);
}
