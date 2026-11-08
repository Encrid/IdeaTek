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

static const VkSemaphoreSubmitInfo SEMAPHORE = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
    .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
};

static const ideatek_swapchain_status_t ROUND[] = {
    IDEATEK_SWAPCHAIN_STATUS_READY, IDEATEK_SWAPCHAIN_STATUS_SKIPPED };

static VkCommandBuffer record_barrier(const gpu_submit_t *s)
{
    VkCommandBuffer cmd = gpu_module.buffers[gpu_module.frame
        % gpu_module.frames];
    VkCommandBufferBeginInfo begin = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    VkImageMemoryBarrier2 barrier = BARRIER;
    VkDependencyInfo dep = { .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &barrier };

    barrier.oldLayout = s->from;
    barrier.newLayout = s->to;
    barrier.image = gpu_module.swap_images[gpu_module.image_index];
    vkBeginCommandBuffer(cmd, &begin);
    vkCmdPipelineBarrier2(cmd, &dep);
    vkEndCommandBuffer(cmd);
    return (cmd);
}

static void submit_barrier(const bridge_api_t *api, const gpu_submit_t *s)
{
    VkSemaphoreSubmitInfo wait = SEMAPHORE;
    VkSemaphoreSubmitInfo signals[2] = { SEMAPHORE, SEMAPHORE };
    VkCommandBufferSubmitInfo cb = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = record_barrier(s) };
    VkSubmitInfo2 submit = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = s->wait != VK_NULL_HANDLE,
        .pWaitSemaphoreInfos = &wait, .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &cb,
        .signalSemaphoreInfoCount = 2 * (s->value != 0),
        .pSignalSemaphoreInfos = signals };

    wait.semaphore = s->wait;
    signals[0].semaphore = s->signal;
    signals[1].semaphore = gpu_module.timeline;
    signals[1].value = s->value;
    note_result(api, vkQueueSubmit2(gpu_module.queues[0], 1, &submit,
        VK_NULL_HANDLE));
}

static void take_image(const bridge_api_t *api)
{
    uint64_t wait = (gpu_module.frame > gpu_module.frames)
        * (gpu_module.frame - gpu_module.frames);
    VkSemaphoreWaitInfo info = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .semaphoreCount = 1, .pSemaphores = &gpu_module.timeline,
        .pValues = &wait };
    gpu_submit_t first = { .from = VK_IMAGE_LAYOUT_UNDEFINED,
        .to = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkResult result;

    gpu_module.frame++;
    first.wait = gpu_module.taken[gpu_module.frame % gpu_module.frames];
    vkWaitSemaphores(gpu_module.device, &info, UINT64_MAX);
    result = vkAcquireNextImageKHR(gpu_module.device, gpu_module.swapchain,
        UINT64_MAX, first.wait, VK_NULL_HANDLE, &gpu_module.image_index);
    note_result(api, result);
    if (result >= 0)
        submit_barrier(api, &first);
    gpu_module.got_image = result >= 0;
    attach_round(api, ROUND[result < 0]);
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
    gpu_submit_t last = { .from = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .to = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .signal = gpu_module.done[gpu_module.image_index],
        .value = gpu_module.frame };
    VkPresentInfoKHR info = { .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1, .pWaitSemaphores = &last.signal,
        .swapchainCount = 1, .pSwapchains = &gpu_module.swapchain,
        .pImageIndices = &gpu_module.image_index };

    if (!gpu_module.got_image)
        return (0);
    submit_barrier(api, &last);
    note_result(api, vkQueuePresentKHR(gpu_module.queues[0], &info));
    gpu_module.got_image = false;
    return (0);
}
