/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_formats.h"

static const VkPresentModeKHR MODES[] = { VK_PRESENT_MODE_FIFO_KHR,
    VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_MAILBOX_KHR,
    VK_PRESENT_MODE_IMMEDIATE_KHR };

static const gpu_format_t HDR[] = {
    { VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT,
        IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM, IDEATEK_COLOR_SPACE_HDR10 },
    { VK_FORMAT_A2R10G10B10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT,
        IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM, IDEATEK_COLOR_SPACE_HDR10 },
    { VK_FORMAT_R16G16B16A16_SFLOAT, VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
        IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT,
        IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR },
};

static const gpu_format_t SRGB = { VK_FORMAT_B8G8R8A8_SRGB,
    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, IDEATEK_IMAGE_FORMAT_BGRA8_SRGB,
    IDEATEK_COLOR_SPACE_SRGB };

gpu_format_t pick_format(const VkSurfaceFormatKHR *list, uint32_t count,
    bool hdr)
{
    gpu_format_t chosen = SRGB;

    for (size_t h = sizeof(HDR) / sizeof(HDR[0]); h-- > 0;)
        for (uint32_t i = 0; i < count; i++)
            chosen = hdr && list[i].format == HDR[h].format
                && list[i].colorSpace == HDR[h].space ? HDR[h] : chosen;
    return (chosen);
}

ideatek_present_mode_t pick_present(const VkPresentModeKHR *list,
    uint32_t count, ideatek_present_mode_t wanted)
{
    bool found = false;

    for (uint32_t i = 0; i < count; i++)
        found |= list[i] == MODES[wanted & 3];
    found &= wanted < 4;
    return (found ? wanted : IDEATEK_PRESENT_MODE_VSYNC);
}

VkPresentModeKHR vulkan_present(ideatek_present_mode_t mode)
{
    return (MODES[mode & 3]);
}

uint32_t clamp_count(uint32_t value, uint32_t least, uint32_t most)
{
    uint32_t up = value < least;
    uint32_t down;

    value = up * least + !up * value;
    down = value > most;
    return (down * most + !down * value);
}
