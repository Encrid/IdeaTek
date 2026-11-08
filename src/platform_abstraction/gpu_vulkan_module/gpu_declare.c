/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static void declare_enums(const bridge_api_t *api)
{
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_gpu_kind_t",
        ideatek_gpu_kind_t);
    GPU_TYPE(api, BRIDGE_BITMASK, "ideatek:ideatek_gpu_feature_t",
        ideatek_gpu_feature_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_gpu_api_t", ideatek_gpu_api_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_gpu_status_t",
        ideatek_gpu_status_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_present_mode_t",
        ideatek_present_mode_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_image_format_t",
        ideatek_image_format_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_color_space_t",
        ideatek_color_space_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_swapchain_status_t",
        ideatek_swapchain_status_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_window_mode_t",
        ideatek_window_mode_t);
    GPU_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_surface_t", ideatek_surface_t);
}

void declare_types(const bridge_api_t *api)
{
    declare_enums(api);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:Gpu", ideatek_gpu_t);
    BRIDGE_NATIVE_DECLARE_TAG(api, "ideatek:DefaultGpu", BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:GpuRequest", ideatek_gpu_request_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:GpuDevice", ideatek_gpu_device_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:SwapchainRequest",
        ideatek_swapchain_request_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:Swapchain", ideatek_swapchain_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:VulkanDevice",
        ideatek_vulkan_device_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:VulkanSwapchain",
        ideatek_vulkan_swapchain_t);
    GPU_TYPE(api, BRIDGE_STRUCT, "ideatek:WindowState",
        ideatek_window_state_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek_gpu_vulkan:Settings", gpu_settings_t,
        BRIDGE_STRIP, BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { GPU, DEFAULT_GPU, GPU_DEVICE, VULKAN_DEVICE,
        SWAPCHAIN, VULKAN_SWAPCHAIN };
    bridge_term_t term[1] = {{ GPU_REQUEST, BRIDGE_READ, BRIDGE_AND, 0 }};
    uint32_t both = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    BRIDGE_NATIVE_ADD_SYSTEM(api, "begin", "first", nullptr, 0, changes, 6,
        nullptr, 0);
    BRIDGE_NATIVE_ADD_SYSTEM(api, "present", "last", nullptr, 0, changes, 6,
        nullptr, 0);
    gpu_module.request_list = BRIDGE_NATIVE_OBSERVER(api, "request",
        both | BRIDGE_ON_LEAVE, term, 1, changes + 2, 2, nullptr, 0);
    term[0].slot = SWAPCHAIN_REQUEST;
    gpu_module.swap_list = BRIDGE_NATIVE_OBSERVER(api, "swap", both, term, 1,
        nullptr, 0, nullptr, 0);
    term[0].slot = WINDOW_STATE;
    gpu_module.window_list = BRIDGE_NATIVE_OBSERVER(api, "window",
        both | BRIDGE_ON_LEAVE, term, 1, nullptr, 0, nullptr, 0);
}
