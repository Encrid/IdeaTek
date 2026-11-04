/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_GPU_H
    #define BC_IDEATEK_GPU_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_GPU_KIND_VALUES { \
        IDEATEK_GPU_KIND_INTEGRATED = 0, \
        IDEATEK_GPU_KIND_DISCRETE = 1, \
        IDEATEK_GPU_KIND_VIRTUAL = 2, \
        IDEATEK_GPU_KIND_CPU = 3, \
        IDEATEK_GPU_KIND_OTHER = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gpu_kind_t, uint8_t, IDEATEK_GPU_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gpu_kind_t, uint8_t, IDEATEK_GPU_KIND_VALUES);

    #define IDEATEK_GPU_FEATURE_VALUES { \
        IDEATEK_GPU_FEATURE_RAY_QUERY = 0x01, \
        IDEATEK_GPU_FEATURE_RAY_TRACING_PIPELINE = 0x02, \
        IDEATEK_GPU_FEATURE_MESH_SHADER = 0x04, \
        IDEATEK_GPU_FEATURE_DRAW_INDIRECT_COUNT = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_gpu_feature_t, uint32_t,
    IDEATEK_GPU_FEATURE_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_gpu_feature_t, uint32_t,
    IDEATEK_GPU_FEATURE_VALUES);

    #define IDEATEK_GPU_API_VALUES { \
        IDEATEK_GPU_API_VULKAN = 0 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gpu_api_t, uint8_t, IDEATEK_GPU_API_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gpu_api_t, uint8_t, IDEATEK_GPU_API_VALUES);

    #define IDEATEK_GPU_STATUS_VALUES { \
        IDEATEK_GPU_STATUS_NONE = 0, \
        IDEATEK_GPU_STATUS_READY = 1, \
        IDEATEK_GPU_STATUS_LOST = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gpu_status_t, uint8_t,
    IDEATEK_GPU_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gpu_status_t, uint8_t,
    IDEATEK_GPU_STATUS_VALUES);

    #define IDEATEK_PRESENT_MODE_VALUES { \
        IDEATEK_PRESENT_MODE_VSYNC = 0, \
        IDEATEK_PRESENT_MODE_ADAPTIVE = 1, \
        IDEATEK_PRESENT_MODE_MAILBOX = 2, \
        IDEATEK_PRESENT_MODE_IMMEDIATE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_present_mode_t, uint8_t,
    IDEATEK_PRESENT_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_present_mode_t, uint8_t,
    IDEATEK_PRESENT_MODE_VALUES);

    #define IDEATEK_IMAGE_FORMAT_VALUES { \
        IDEATEK_IMAGE_FORMAT_BGRA8_SRGB = 0, \
        IDEATEK_IMAGE_FORMAT_RGBA8_SRGB = 1, \
        IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM = 2, \
        IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM = 3, \
        IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_image_format_t, uint8_t,
    IDEATEK_IMAGE_FORMAT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_image_format_t, uint8_t,
    IDEATEK_IMAGE_FORMAT_VALUES);

    #define IDEATEK_COLOR_SPACE_VALUES { \
        IDEATEK_COLOR_SPACE_SRGB = 0, \
        IDEATEK_COLOR_SPACE_HDR10 = 1, \
        IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_color_space_t, uint8_t,
    IDEATEK_COLOR_SPACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_color_space_t, uint8_t,
    IDEATEK_COLOR_SPACE_VALUES);

    #define IDEATEK_SWAPCHAIN_STATUS_VALUES { \
        IDEATEK_SWAPCHAIN_STATUS_READY = 0, \
        IDEATEK_SWAPCHAIN_STATUS_SKIPPED = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_swapchain_status_t, uint8_t,
    IDEATEK_SWAPCHAIN_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_swapchain_status_t, uint8_t,
    IDEATEK_SWAPCHAIN_STATUS_VALUES);

    #define IDEATEK_GPU_FIELDS { \
        char name[256]; \
        uint64_t memory_bytes; \
        uint32_t vendor_id; \
        uint32_t device_id; \
        ideatek_gpu_feature_t features; \
        ideatek_gpu_kind_t kind; \
        bool usable; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gpu_t, IDEATEK_GPU_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gpu_t, IDEATEK_GPU_FIELDS);

    #define IDEATEK_GPU_REQUEST_FIELDS { \
        ideatek_entity_t gpu; \
        ideatek_gpu_feature_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gpu_request_t, IDEATEK_GPU_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gpu_request_t, IDEATEK_GPU_REQUEST_FIELDS);

    #define IDEATEK_GPU_DEVICE_FIELDS { \
        ideatek_entity_t gpu; \
        uint32_t generation; \
        ideatek_gpu_feature_t features; \
        ideatek_gpu_api_t api; \
        ideatek_gpu_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gpu_device_t, IDEATEK_GPU_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gpu_device_t, IDEATEK_GPU_DEVICE_FIELDS);

    #define IDEATEK_SWAPCHAIN_REQUEST_FIELDS { \
        ideatek_present_mode_t present_mode; \
        bool hdr; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_swapchain_request_t,
    IDEATEK_SWAPCHAIN_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_swapchain_request_t,
    IDEATEK_SWAPCHAIN_REQUEST_FIELDS);

    #define IDEATEK_SWAPCHAIN_FIELDS { \
        uint64_t frame; \
        uint32_t width; \
        uint32_t height; \
        uint32_t slot; \
        ideatek_image_format_t format; \
        ideatek_color_space_t color_space; \
        ideatek_present_mode_t present_mode; \
        ideatek_swapchain_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_swapchain_t, IDEATEK_SWAPCHAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_swapchain_t, IDEATEK_SWAPCHAIN_FIELDS);

#endif
