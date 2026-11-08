/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

void attach_device(const bridge_api_t *api,
    ideatek_gpu_status_t status, ideatek_gpu_feature_t on)
{
    gpu_adapter_t *a = &gpu_module.gpus[gpu_module.chosen];
    ideatek_gpu_device_t d = { .gpu.id = a->entity,
        .generation = gpu_module.generation, .features = on,
        .api = IDEATEK_GPU_API_VULKAN, .status = status };
    ideatek_vulkan_device_t v = {
        .instance = (uint64_t)gpu_module.instance,
        .physical_device = (uint64_t)a->handle,
        .device = (uint64_t)gpu_module.device,
        .queue = (uint64_t)gpu_module.queues[0],
        .compute_queue = (uint64_t)gpu_module.queues[1],
        .transfer_queue = (uint64_t)gpu_module.queues[2],
        .timeline = (uint64_t)gpu_module.timeline,
        .queue_family = gpu_module.families[0],
        .compute_family = gpu_module.families[1],
        .transfer_family = gpu_module.families[2] };

    BRIDGE_NATIVE_ATTACH(api, gpu_module.request, GPU_DEVICE, d);
    if (status == IDEATEK_GPU_STATUS_READY)
        BRIDGE_NATIVE_ATTACH(api, gpu_module.request, VULKAN_DEVICE, v);
}

void drop_device([[maybe_unused]] const bridge_api_t *api)
{
    if (!gpu_module.device)
        return;
    vkDeviceWaitIdle(gpu_module.device);
    drop_swapchain();
    vkDestroyCommandPool(gpu_module.device, gpu_module.pool, nullptr);
    vkDestroySemaphore(gpu_module.device, gpu_module.timeline, nullptr);
    vkDestroyDevice(gpu_module.device, nullptr);
    gpu_module.device = VK_NULL_HANDLE;
    gpu_module.pool = VK_NULL_HANDLE;
    gpu_module.timeline = VK_NULL_HANDLE;
}

void note_result(const bridge_api_t *api, VkResult result)
{
    gpu_module.stale |= (result == VK_ERROR_OUT_OF_DATE_KHR)
        | (result == VK_SUBOPTIMAL_KHR);
    if (result != VK_ERROR_DEVICE_LOST)
        return;
    bridge_native_say(api, BRIDGE_LOG_WARN, "the device is lost");
    gpu_module.lost = true;
    attach_device(api, IDEATEK_GPU_STATUS_LOST, 0);
}

BRIDGE_NATIVE(shutdown)
{
    drop_device(api);
    if (gpu_module.messenger)
        vkDestroyDebugUtilsMessengerEXT(gpu_module.instance,
            gpu_module.messenger, nullptr);
    if (gpu_module.instance)
        vkDestroyInstance(gpu_module.instance, nullptr);
    return (0);
}
