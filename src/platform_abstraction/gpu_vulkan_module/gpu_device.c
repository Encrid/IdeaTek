/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const char *const DEVICE_EXTENSIONS[] = { "VK_KHR_swapchain" };

static const float ONE = 1.0F;

static uint32_t queue_infos(VkDeviceQueueCreateInfo infos[3])
{
    uint32_t count = 0;
    bool seen;

    for (uint32_t f = 0; f < 3; f++) {
        seen = false;
        for (uint32_t i = 0; i < count; i++)
            seen |= infos[i].queueFamilyIndex == gpu_module.families[f];
        if (seen)
            continue;
        infos[count] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = gpu_module.families[f], .queueCount = 1,
            .pQueuePriorities = &ONE };
        count++;
    }
    return (count);
}

static bool open_device(const bridge_api_t *api, gpu_adapter_t *a)
{
    VkDeviceQueueCreateInfo queues[3];
    gpu_chain_t chain = {};
    VkDeviceCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = queues, .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = DEVICE_EXTENSIONS, .pNext = &chain.f10 };
    VkResult result;

    find_queues(a->handle);
    query_chain(a->handle, &chain);
    info.queueCreateInfoCount = queue_infos(queues);
    result = vkCreateDevice(a->handle, &info, nullptr, &gpu_module.device);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no device: %d", result);
        gpu_module.device = VK_NULL_HANDLE;
        return (false);
    }
    gpu_module.physical = a->handle;
    volkLoadDevice(gpu_module.device);
    return (true);
}

static const VkSemaphoreTypeCreateInfo TIMELINE = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
    .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
};

static void make_semaphores(void)
{
    VkSemaphoreCreateInfo sem = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = &TIMELINE };
    VkSemaphoreCreateInfo binary = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    vkCreateSemaphore(gpu_module.device, &sem, nullptr, &gpu_module.timeline);
    for (uint32_t f = 0; f < gpu_module.frames; f++)
        vkCreateSemaphore(gpu_module.device, &binary, nullptr,
            &gpu_module.taken[f]);
}

static void make_sync(void)
{
    VkCommandPoolCreateInfo pool = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = gpu_module.families[0] };
    VkCommandBufferAllocateInfo buffers = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = gpu_module.frames };

    for (uint32_t f = 0; f < 3; f++)
        vkGetDeviceQueue(gpu_module.device, gpu_module.families[f], 0,
            &gpu_module.queues[f]);
    make_semaphores();
    vkCreateCommandPool(gpu_module.device, &pool, nullptr, &gpu_module.pool);
    buffers.commandPool = gpu_module.pool;
    vkAllocateCommandBuffers(gpu_module.device, &buffers, gpu_module.buffers);
    gpu_module.frame = 0;
}

bool make_device(const bridge_api_t *api, ideatek_gpu_feature_t asked,
    uint64_t asked_gpu)
{
    gpu_adapter_t *a = &gpu_module.gpus[gpu_module.chosen];

    gpu_module.generation++;
    if (!a->usable) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the gpu %llu is not usable: %s", (unsigned long long)asked_gpu,
            "it is no adapter with Vulkan 1.4 and Roadmap 2022");
        attach_device(api, IDEATEK_GPU_STATUS_NONE, 0);
        return (false);
    }
    if (asked & ~a->features)
        bridge_native_say(api, BRIDGE_LOG_WARN, "the gpu lacks 0x%x",
            asked & ~a->features);
    if (!open_device(api, a)) {
        attach_device(api, IDEATEK_GPU_STATUS_NONE, 0);
        return (false);
    }
    make_sync();
    attach_device(api, IDEATEK_GPU_STATUS_READY, asked & a->features);
    return (true);
}
