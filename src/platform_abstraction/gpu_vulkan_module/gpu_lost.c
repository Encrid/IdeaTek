/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

void drop_device(const bridge_api_t *api)
{
    (void)api;
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
    gpu_module.lost = false;
}

void lose_device(const bridge_api_t *api, VkResult result)
{
    ideatek_gpu_device_t d = {
        .gpu.id = gpu_module.gpus[gpu_module.chosen].entity,
        .generation = gpu_module.generation,
        .api = IDEATEK_GPU_API_VULKAN, .status = IDEATEK_GPU_STATUS_LOST };

    if (result != VK_ERROR_DEVICE_LOST || gpu_module.lost)
        return;
    bridge_native_say(api, BRIDGE_LOG_WARN, "the device is lost");
    gpu_module.lost = true;
    gpu_module.remake = true;
    BRIDGE_NATIVE_ATTACH(api, gpu_module.request, GPU_DEVICE, d);
}
