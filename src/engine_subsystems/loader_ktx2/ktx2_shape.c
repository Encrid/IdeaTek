/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_pure.h"

static uint32_t count_levels(const ktx2_file_t *f)
{
    uint32_t side = f->width > f->height ? f->width : f->height;
    uint32_t count = 0;

    side = side > f->depth ? side : f->depth;
    while (side) {
        count++;
        side >>= 1;
    }
    return (count);
}

static const char *check_kind(const ktx2_file_t *f)
{
    if (f->format == 0)
        return ("vkFormat is 0");
    if (f->scheme != 0)
        return ("supercompressionScheme is not 0");
    if (f->width == 0 || f->height == 0)
        return ("pixelWidth or pixelHeight is 0");
    return (nullptr);
}

static const char *check_faces(const ktx2_file_t *f)
{
    if (f->faces != 1 && f->faces != 6)
        return ("faceCount is not 1 or 6");
    if (f->faces == 6 && (f->width != f->height || f->depth))
        return ("a cube is not square or has a pixelDepth");
    if (f->layers && (f->faces == 6 || f->depth))
        return ("a cube or a volume has a layerCount");
    return (nullptr);
}

const char *ktx2_check_shape(const ktx2_file_t *f)
{
    const char *reason = check_kind(f);

    reason = reason ? reason : check_faces(f);
    if (!reason && f->levels > count_levels(f))
        reason = "levelCount is above the levels of the largest side";
    return (reason);
}
