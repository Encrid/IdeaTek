/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef GPU_FORMATS_H
    #define GPU_FORMATS_H

    #include <stdint.h>

    #ifndef VK_NO_PROTOTYPES
        #define VK_NO_PROTOTYPES
    #endif
    #include <vulkan/vulkan_core.h>

    #include "ideatek_gpu.h"

typedef struct {
    VkFormat format;
    VkColorSpaceKHR space;
    ideatek_image_format_t out_format;
    ideatek_color_space_t out_space;
} gpu_format_t;

gpu_format_t pick_format(const VkSurfaceFormatKHR *list, uint32_t count,
    bool hdr);
ideatek_present_mode_t pick_present(const VkPresentModeKHR *list,
    uint32_t count, ideatek_present_mode_t wanted);
VkPresentModeKHR vulkan_present(ideatek_present_mode_t mode);
uint32_t clamp_count(uint32_t value, uint32_t least, uint32_t most);

#endif
