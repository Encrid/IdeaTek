/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

VkResult first_result(VkResult kept, VkResult result)
{
    return (kept != VK_SUCCESS ? kept : result);
}

static const VkSemaphoreTypeCreateInfo TIMELINE = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
    .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
};

static VkResult make_semaphores(void)
{
    VkSemaphoreCreateInfo sem = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = &TIMELINE };
    VkSemaphoreCreateInfo binary = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VkResult result;

    gpu_module.timeline = VK_NULL_HANDLE;
    result = vkCreateSemaphore(gpu_module.device, &sem, nullptr,
        &gpu_module.timeline);
    for (uint32_t f = 0; f < gpu_module.frames; f++) {
        gpu_module.taken[f] = VK_NULL_HANDLE;
        result = first_result(result, vkCreateSemaphore(gpu_module.device,
            &binary, nullptr, &gpu_module.taken[f]));
    }
    return (result);
}

static VkResult make_pool(void)
{
    VkCommandPoolCreateInfo pool = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = gpu_module.families[0] };
    VkCommandBufferAllocateInfo buffers = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = gpu_module.frames };
    VkResult result;

    gpu_module.pool = VK_NULL_HANDLE;
    result = vkCreateCommandPool(gpu_module.device, &pool, nullptr,
        &gpu_module.pool);
    buffers.commandPool = gpu_module.pool;
    if (result == VK_SUCCESS)
        result = vkAllocateCommandBuffers(gpu_module.device, &buffers,
            gpu_module.buffers);
    return (result);
}

bool make_sync(const bridge_api_t *api)
{
    VkResult result;

    for (uint32_t f = 0; f < 3; f++)
        vkGetDeviceQueue(gpu_module.device, gpu_module.families[f], 0,
            &gpu_module.queues[f]);
    result = make_semaphores();
    result = first_result(result, make_pool());
    gpu_module.frame = 0;
    if (result != VK_SUCCESS)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no device: %d", result);
    return (result == VK_SUCCESS);
}
