/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const char *const DEVICE_EXTENSIONS[] = { "VK_KHR_swapchain" };

static void find_queues(VkPhysicalDevice gpu)
{
    VkQueueFamilyProperties list[32];
    uint32_t count = 0;
    uint32_t graphics = VK_QUEUE_GRAPHICS_BIT;
    uint32_t compute = VK_QUEUE_COMPUTE_BIT;

    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, nullptr);
    count = count < 32 ? count : 32;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, list);
    gpu_module.families[0] = 0;
    for (uint32_t i = 0; i < count; i++)
        if (list[i].queueFlags & graphics)
            gpu_module.families[0] = i;
    gpu_module.families[1] = gpu_module.families[0];
    gpu_module.families[2] = gpu_module.families[0];
    for (uint32_t i = 0; i < count; i++) {
        if ((list[i].queueFlags & (graphics | compute)) == compute)
            gpu_module.families[1] = i;
        if ((list[i].queueFlags & (graphics | compute)) == 0
            && list[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            gpu_module.families[2] = i;
    }
}

static const float ONE = 1.0F;

static uint32_t queue_infos(VkDeviceQueueCreateInfo infos[3])
{
    uint32_t count = 0;

    for (uint32_t f = 0; f < 3; f++) {
        bool seen = false;
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

static void attach_device(const bridge_api_t *api,
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

static bool open_device(const bridge_api_t *api, gpu_adapter_t *a,
    const gpu_chain_t *chain)
{
    VkDeviceQueueCreateInfo queues[3];
    VkDeviceCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = queues, .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = DEVICE_EXTENSIONS, .pNext = &chain->f10 };
    VkResult result;

    find_queues(a->handle);
    info.queueCreateInfoCount = queue_infos(queues);
    result = vkCreateDevice(a->handle, &info, nullptr, &gpu_module.device);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no device: %d", result);
        gpu_module.device = VK_NULL_HANDLE;
        return (false);
    }
    volkLoadDevice(gpu_module.device);
    for (uint32_t f = 0; f < 3; f++)
        vkGetDeviceQueue(gpu_module.device, gpu_module.families[f], 0,
            &gpu_module.queues[f]);
    return (true);
}

bool make_device(const bridge_api_t *api, ideatek_gpu_feature_t asked)
{
    gpu_chain_t chain = {};
    VkSemaphoreTypeCreateInfo kind = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE };
    VkSemaphoreCreateInfo sem = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = &kind };
    VkCommandPoolCreateInfo pool = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = 0 };
    VkCommandBufferAllocateInfo buffers = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = gpu_module.frames };
    gpu_adapter_t *a = &gpu_module.gpus[gpu_module.chosen];
    ideatek_gpu_feature_t has = a->features;

    gpu_module.generation++;
    if (asked & ~has)
        bridge_native_say(api, BRIDGE_LOG_WARN, "the gpu lacks 0x%x",
            asked & ~has);
    query_chain(a->handle, &chain);
    if (!open_device(api, a, &chain)) {
        attach_device(api, IDEATEK_GPU_STATUS_NONE, 0);
        return (false);
    }
    vkCreateSemaphore(gpu_module.device, &sem, nullptr, &gpu_module.timeline);
    pool.queueFamilyIndex = gpu_module.families[0];
    vkCreateCommandPool(gpu_module.device, &pool, nullptr, &gpu_module.pool);
    buffers.commandPool = gpu_module.pool;
    vkAllocateCommandBuffers(gpu_module.device, &buffers, gpu_module.buffers);
    gpu_module.frame = 0;
    gpu_module.physical = a->handle;
    attach_device(api, IDEATEK_GPU_STATUS_READY, asked & has);
    return (true);
}
