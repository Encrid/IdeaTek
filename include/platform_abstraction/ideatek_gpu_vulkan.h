/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_GPU_VULKAN_H
    #define BC_IDEATEK_GPU_VULKAN_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_VULKAN_DEVICE_FIELDS { \
        uint64_t instance; \
        uint64_t physical_device; \
        uint64_t device; \
        uint64_t queue; \
        uint64_t timeline; \
        uint32_t queue_family; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vulkan_device_t,
    IDEATEK_VULKAN_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vulkan_device_t,
    IDEATEK_VULKAN_DEVICE_FIELDS);

    #define IDEATEK_VULKAN_SWAPCHAIN_FIELDS { \
        uint64_t image; \
        uint64_t view; \
        int32_t format; \
        int32_t color_space; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vulkan_swapchain_t,
    IDEATEK_VULKAN_SWAPCHAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vulkan_swapchain_t,
    IDEATEK_VULKAN_SWAPCHAIN_FIELDS);

#endif
