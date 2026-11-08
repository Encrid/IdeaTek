/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gpu_formats.h"

static const VkSurfaceFormatKHR SURFACES[][3] = {
    { { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR } },
    { { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
        { VK_FORMAT_A2B10G10R10_UNORM_PACK32,
            VK_COLOR_SPACE_HDR10_ST2084_EXT } },
    { { VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
        { VK_FORMAT_R16G16B16A16_SFLOAT,
            VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT } },
};

static const VkPresentModeKHR PRESENTS[] = { VK_PRESENT_MODE_FIFO_KHR,
    VK_PRESENT_MODE_MAILBOX_KHR };

int main(int argc, char **argv)
{
    gpu_format_t f;

    if (argc < 4)
        return (2);
    if (!strcmp(argv[1], "format")) {
        f = pick_format(SURFACES[atoi(argv[2])], 3, atoi(argv[3]));
        printf("format %d space %d\n", f.out_format, f.out_space);
    }
    if (!strcmp(argv[1], "present"))
        printf("present %d\n", pick_present(PRESENTS, (uint32_t)atoi(argv[2]),
            (ideatek_present_mode_t)atoi(argv[3])));
    if (!strcmp(argv[1], "clamp"))
        printf("count %u\n", clamp_count((uint32_t)atoi(argv[2]), 2,
            (uint32_t)atoi(argv[3])));
    return (0);
}
