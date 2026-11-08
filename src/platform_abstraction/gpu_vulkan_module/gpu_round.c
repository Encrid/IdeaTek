/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

void attach_round(const bridge_api_t *api, ideatek_swapchain_status_t status)
{
    ideatek_swapchain_t s = gpu_module.swap_value;
    ideatek_vulkan_swapchain_t v = {
        .image = (uint64_t)gpu_module.swap_images[gpu_module.image_index],
        .view = (uint64_t)gpu_module.views[gpu_module.image_index],
        .format = (int32_t)gpu_module.format,
        .color_space = (int32_t)gpu_module.color_space };

    s.frame = gpu_module.frame;
    s.slot = (uint32_t)(gpu_module.frame % gpu_module.frames);
    s.status = status;
    BRIDGE_NATIVE_ATTACH(api, gpu_module.window, SWAPCHAIN, s);
    if (status == IDEATEK_SWAPCHAIN_STATUS_READY)
        BRIDGE_NATIVE_ATTACH(api, gpu_module.window, VULKAN_SWAPCHAIN, v);
}

static void submit_barrier(VkImageLayout from, VkImageLayout to,
    uint64_t signal)
{
    VkCommandBuffer cmd = gpu_module.buffers[gpu_module.frame
        % gpu_module.frames];
    VkCommandBufferBeginInfo begin = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };
    VkImageMemoryBarrier2 barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT
            | VK_ACCESS_2_MEMORY_WRITE_BIT,
        .oldLayout = from, .newLayout = to,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = gpu_module.swap_images[gpu_module.image_index],
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };
    VkDependencyInfo dep = { .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &barrier };
    VkSemaphoreSubmitInfo sig = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = gpu_module.timeline, .value = signal,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT };
    VkCommandBufferSubmitInfo cb = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = cmd };
    VkSubmitInfo2 submit = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .commandBufferInfoCount = 1, .pCommandBufferInfos = &cb,
        .signalSemaphoreInfoCount = signal != 0, .pSignalSemaphoreInfos = &sig };

    vkBeginCommandBuffer(cmd, &begin);
    vkCmdPipelineBarrier2(cmd, &dep);
    vkEndCommandBuffer(cmd);
    vkQueueSubmit2(gpu_module.queues[0], 1, &submit, VK_NULL_HANDLE);
}

static void settle(const bridge_api_t *api)
{
    uint32_t at = bridge_native_find_value(api, gpu_module.request,
        GPU_REQUEST);
    const ideatek_gpu_request_t *r = (const void *)(api->shared_base + at);

    if (!gpu_module.started && !start_gpus(api))
        return;
    if (gpu_module.remake && at) {
        drop_device(api);
        gpu_module.remake = false;
        gpu_module.chosen = gpu_module.default_gpu;
        for (uint32_t i = 0; i < gpu_module.gpu_count; i++)
            if (gpu_module.gpus[i].entity == r->gpu.id)
                gpu_module.chosen = i;
        if (gpu_module.chosen >= GPU_GPUS || !gpu_module.gpus[gpu_module.chosen]
            .usable)
            bridge_native_say(api, BRIDGE_LOG_WARN,
                "the gpu %llu is not usable: %s",
                (unsigned long long)r->gpu.id, "no usable adapter");
        else
            make_device(api, r->features);
    }
    if (gpu_module.device && gpu_module.window && !gpu_module.surface
        && gpu_module.surface_kind != IDEATEK_SURFACE_NONE)
        make_surface(api);
    if (gpu_module.device && gpu_module.surface && gpu_module.swap_asked
        && !gpu_module.swapchain && gpu_module.pixel_width
        && gpu_module.pixel_height && !gpu_module.minimized)
        make_swapchain(api);
}

BRIDGE_NATIVE(begin)
{
    uint64_t wait = gpu_module.frame > gpu_module.frames
        ? gpu_module.frame - gpu_module.frames : 0;
    VkSemaphoreWaitInfo info = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .semaphoreCount = 1, .pSemaphores = &gpu_module.timeline,
        .pValues = &wait };
    VkResult result;

    settle(api);
    gpu_module.got_image = false;
    if (!gpu_module.swapchain || gpu_module.minimized) {
        if (gpu_module.window && gpu_module.device)
            attach_round(api, IDEATEK_SWAPCHAIN_STATUS_SKIPPED);
        return (0);
    }
    gpu_module.frame++;
    vkWaitSemaphores(gpu_module.device, &info, UINT64_MAX);
    result = vkAcquireNextImageKHR(gpu_module.device, gpu_module.swapchain,
        UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &gpu_module.image_index);
    lose_device(api, result);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
        drop_swapchain();
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        attach_round(api, IDEATEK_SWAPCHAIN_STATUS_SKIPPED);
        return (0);
    }
    submit_barrier(VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0);
    gpu_module.got_image = true;
    attach_round(api, IDEATEK_SWAPCHAIN_STATUS_READY);
    return (0);
}

BRIDGE_NATIVE(present)
{
    VkPresentInfoKHR info = { .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount = 1, .pSwapchains = &gpu_module.swapchain,
        .pImageIndices = &gpu_module.image_index };
    VkResult result;

    if (!gpu_module.got_image)
        return (0);
    submit_barrier(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, gpu_module.frame);
    vkQueueWaitIdle(gpu_module.queues[0]);
    result = vkQueuePresentKHR(gpu_module.queues[0], &info);
    lose_device(api, result);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        drop_swapchain();
    gpu_module.got_image = false;
    return (0);
}
