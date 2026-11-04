/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_gpu.h"
#include "ideatek_vulkan.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T,     BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T,     BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef struct {
    uint32_t gpu;
    uint32_t default_gpu;
    uint32_t request;
    uint32_t device;
    uint32_t swapchain_request;
    uint32_t swapchain;
    uint32_t vulkan_device;
    uint32_t vulkan_swapchain;
} slots_t;

static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_gpu_kind_t", ideatek_gpu_kind_t);
    ENUM(api, "ideatek:ideatek_gpu_api_t", ideatek_gpu_api_t);
    ENUM(api, "ideatek:ideatek_gpu_status_t", ideatek_gpu_status_t);
    ENUM(api, "ideatek:ideatek_present_mode_t", ideatek_present_mode_t);
    ENUM(api, "ideatek:ideatek_image_format_t", ideatek_image_format_t);
    ENUM(api, "ideatek:ideatek_color_space_t", ideatek_color_space_t);
    ENUM(api, "ideatek:ideatek_swapchain_status_t",
        ideatek_swapchain_status_t);
}

static slots_t declare_structs(const bridge_api_t *api)
{
    return ((slots_t){
        .gpu = STRUCT(api, "ideatek:Gpu", ideatek_gpu_t),
        .default_gpu = BRIDGE_NATIVE_DECLARE_TAG(api, "ideatek:DefaultGpu",
            BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE),
        .request = STRUCT(api, "ideatek:GpuRequest", ideatek_gpu_request_t),
        .device = STRUCT(api, "ideatek:GpuDevice", ideatek_gpu_device_t),
        .swapchain_request = STRUCT(api, "ideatek:SwapchainRequest",
            ideatek_swapchain_request_t),
        .swapchain = STRUCT(api, "ideatek:Swapchain", ideatek_swapchain_t),
        .vulkan_device = STRUCT(api, "ideatek:VulkanDevice",
            ideatek_vulkan_device_t),
        .vulkan_swapchain = STRUCT(api, "ideatek:VulkanSwapchain",
            ideatek_vulkan_swapchain_t)
    });
}

[[maybe_unused]] static void ask(const bridge_api_t *api, slots_t slots)
{
    ideatek_gpu_request_t request = { 0 };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.request, request);
}

[[maybe_unused]] static void report(const bridge_api_t *api, slots_t slots)
{
    ideatek_gpu_device_t device = { .api = IDEATEK_GPU_API_VULKAN,
        .status = IDEATEK_GPU_STATUS_NONE };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slots.device, device);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_structs(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    declare_fields(api);
    ask(api, declare_structs(api));
#elif defined(ROLE_GPU)
    declare_fields(api);
    report(api, declare_structs(api));
#else
    declare_fields(api);
    declare_structs(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
