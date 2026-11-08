/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const VkImageMemoryBarrier2 BARRIER = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
    .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
    .dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 },
};

static void record_barrier(VkCommandBuffer cmd, VkImageLayout from,
    VkImageLayout to)
{
    VkCommandBufferBeginInfo begin = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    VkImageMemoryBarrier2 barrier = BARRIER;
    VkDependencyInfo dep = { .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &barrier };

    barrier.oldLayout = from;
    barrier.newLayout = to;
    barrier.image = gpu_module.swap_images[gpu_module.image_index];
    vkBeginCommandBuffer(cmd, &begin);
    vkCmdPipelineBarrier2(cmd, &dep);
    vkEndCommandBuffer(cmd);
}

static void submit_barrier(VkImageLayout from, VkImageLayout to,
    uint64_t signal)
{
    VkCommandBuffer cmd = gpu_module.buffers[gpu_module.frame
        % gpu_module.frames];
    VkSemaphoreSubmitInfo sig = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = gpu_module.timeline, .value = signal,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT };
    VkCommandBufferSubmitInfo cb = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = cmd };
    VkSubmitInfo2 submit = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .commandBufferInfoCount = 1, .pCommandBufferInfos = &cb,
        .signalSemaphoreInfoCount = signal != 0,
        .pSignalSemaphoreInfos = &sig };

    record_barrier(cmd, from, to);
    vkQueueSubmit2(gpu_module.queues[0], 1, &submit, VK_NULL_HANDLE);
}

static void take_image(const bridge_api_t *api)
{
    uint64_t wait = gpu_module.frame > gpu_module.frames
        ? gpu_module.frame - gpu_module.frames : 0;
    VkSemaphoreWaitInfo info = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .semaphoreCount = 1, .pSemaphores = &gpu_module.timeline,
        .pValues = &wait };
    VkResult result;

    gpu_module.frame++;
    vkWaitSemaphores(gpu_module.device, &info, UINT64_MAX);
    result = vkAcquireNextImageKHR(gpu_module.device, gpu_module.swapchain,
        UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &gpu_module.image_index);
    note_result(api, result);
    if (result < 0) {
        attach_round(api, IDEATEK_SWAPCHAIN_STATUS_SKIPPED);
        return;
    }
    submit_barrier(VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0);
    gpu_module.got_image = true;
    attach_round(api, IDEATEK_SWAPCHAIN_STATUS_READY);
}

BRIDGE_NATIVE(begin)
{
    if (!settle(api))
        return (1);
    gpu_module.got_image = false;
    if (gpu_module.swapchain && gpu_module.drawable)
        take_image(api);
    else if (gpu_module.window)
        attach_round(api, IDEATEK_SWAPCHAIN_STATUS_SKIPPED);
    return (0);
}

BRIDGE_NATIVE(present)
{
    VkPresentInfoKHR info = { .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount = 1, .pSwapchains = &gpu_module.swapchain,
        .pImageIndices = &gpu_module.image_index };

    if (!gpu_module.got_image)
        return (0);
    submit_barrier(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, gpu_module.frame);
    vkQueueWaitIdle(gpu_module.queues[0]);
    note_result(api, vkQueuePresentKHR(gpu_module.queues[0], &info));
    gpu_module.got_image = false;
    return (0);
}
