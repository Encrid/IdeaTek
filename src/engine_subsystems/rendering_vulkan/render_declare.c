/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    RENDER_TYPE(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    RENDER_TYPE(api, "ideatek:ideatek_rect_t", ideatek_rect_t);
    RENDER_TYPE(api, "ideatek:ideatek_position_t", ideatek_position_t);
    RENDER_ENUM(api, "ideatek:ideatek_projection_kind_t",
        ideatek_projection_kind_t);
    RENDER_ENUM(api, "ideatek:ideatek_capture_status_t",
        ideatek_capture_status_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, "ideatek:ideatek_gpu_feature_t",
        ideatek_gpu_feature_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    RENDER_ENUM(api, "ideatek:ideatek_gpu_api_t", ideatek_gpu_api_t);
    RENDER_ENUM(api, "ideatek:ideatek_gpu_status_t", ideatek_gpu_status_t);
    RENDER_ENUM(api, "ideatek:ideatek_image_format_t", ideatek_image_format_t);
    RENDER_ENUM(api, "ideatek:ideatek_color_space_t", ideatek_color_space_t);
    RENDER_ENUM(api, "ideatek:ideatek_present_mode_t", ideatek_present_mode_t);
    RENDER_ENUM(api, "ideatek:ideatek_swapchain_status_t",
        ideatek_swapchain_status_t);
}

static void declare_reads(const bridge_api_t *api, uint32_t *s)
{
    s[CAMERA] = RENDER_TYPE(api, "ideatek:Camera", ideatek_camera_t);
    s[PROJECTION] = RENDER_TYPE(api, "ideatek:Projection",
        ideatek_projection_t);
    s[CAPTURE_REQUEST] = RENDER_TYPE(api, "ideatek:CaptureRequest",
        ideatek_capture_request_t);
    s[WORLD] = RENDER_TYPE(api, "ideatek:WorldTransform",
        ideatek_world_transform_t);
    s[GPU_DEVICE] = RENDER_TYPE(api, "ideatek:GpuDevice",
        ideatek_gpu_device_t);
    s[SWAPCHAIN] = RENDER_TYPE(api, "ideatek:Swapchain", ideatek_swapchain_t);
    s[VULKAN_DEVICE] = RENDER_TYPE(api, "ideatek:VulkanDevice",
        ideatek_vulkan_device_t);
    s[VULKAN_SWAPCHAIN] = RENDER_TYPE(api, "ideatek:VulkanSwapchain",
        ideatek_vulkan_swapchain_t);
    s[FRAME] = RENDER_SINGLE(api, "ideatek:Frame", ideatek_frame_t);
}

void declare_types(const bridge_api_t *api)
{
    uint32_t *s = render_module.slots;

    declare_fields(api);
    declare_reads(api, s);
    s[CAPTURE] = RENDER_TYPE(api, "ideatek:Capture", ideatek_capture_t);
    s[STATS] = RENDER_SINGLE(api, "ideatek:RenderStats",
        ideatek_render_stats_t);
    s[SETTINGS] = RENDER_SINGLE(api, "ideatek_rendering_vulkan:Settings",
        render_settings_t);
}

const void *render_find(const bridge_api_t *api, uint64_t id, uint32_t slot)
{
    uint32_t at = bridge_native_find_value(api, id,
        render_module.slots[slot]);

    return (at ? api->shared_base + at : nullptr);
}
