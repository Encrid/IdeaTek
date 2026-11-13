/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static void take_handles(render_device_t *d,
    const ideatek_vulkan_device_t *vk)
{
    *d = (render_device_t){ .entity = d->entity,
        .instance = (VkInstance)vk->instance,
        .physical = (VkPhysicalDevice)vk->physical_device,
        .device = (VkDevice)vk->device, .queue = (VkQueue)vk->queue,
        .family = vk->queue_family };
    volkLoadInstanceOnly(d->instance);
    volkLoadDevice(d->device);
}

static bool make_objects(const ideatek_vulkan_device_t *vk)
{
    render_device_t *d = &render_module.device;
    VkCommandPoolCreateInfo pool = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vk->queue_family };
    VkCommandBufferAllocateInfo buffers = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = RENDER_SLOTS };
    bool ok;

    if (volkInitialize() != VK_SUCCESS)
        return (false);
    take_handles(d, vk);
    ok = vkCreateCommandPool(d->device, &pool, nullptr, &d->pool)
        == VK_SUCCESS;
    buffers.commandPool = d->pool;
    ok = ok && vkAllocateCommandBuffers(d->device, &buffers, d->buffers)
        == VK_SUCCESS;
    return (ok && make_reads(d, render_module.halves[0].len));
}

static void tell_absence(const bridge_api_t *api, bool live)
{
    render_device_t *d = &render_module.device;

    if (!live && !d->told)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no device: the gpu module "
            "has no device");
    d->told = !live;
}

bool find_device(const bridge_api_t *api)
{
    render_device_t *d = &render_module.device;
    const ideatek_gpu_device_t *gpu = RENDER_FIND(api, d->entity, GPU_DEVICE,
        ideatek_gpu_device_t);
    const ideatek_vulkan_device_t *vk = RENDER_FIND(api, d->entity,
        VULKAN_DEVICE, ideatek_vulkan_device_t);
    bool live = vk && gpu->status == IDEATEK_GPU_STATUS_READY;

    if (live && (d->ready | d->refused))
        return (d->ready);
    fail_captures(api);
    free_objects();
    tell_absence(api, live);
    d->ready = live && make_objects(vk);
    d->refused = live && !d->ready;
    if (d->refused)
        bridge_native_say(api, BRIDGE_LOG_ERR, "no device: vulkan refuses the "
            "objects of the module");
    return (d->ready);
}

void free_objects(void)
{
    render_device_t *d = &render_module.device;
    render_device_t kept = { .entity = d->entity, .told = d->told,
        .refused = d->refused };

    if (d->device)
        vkDeviceWaitIdle(d->device);
    free_target();
    for (uint32_t k = 0; d->device && k < RENDER_SLOTS; k++) {
        vkDestroyBuffer(d->device, d->reads[k], nullptr);
        vkFreeMemory(d->device, d->read_memory[k], nullptr);
    }
    if (d->device)
        vkDestroyCommandPool(d->device, d->pool, nullptr);
    *d = kept;
}
