/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

void move_image(VkCommandBuffer cmd, VkImage image, VkImageLayout from,
    VkImageLayout to)
{
    VkImageMemoryBarrier2 b = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT
            | VK_ACCESS_2_MEMORY_WRITE_BIT,
        .oldLayout = from, .newLayout = to, .image = image,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };
    VkDependencyInfo dep = { .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &b };

    vkCmdPipelineBarrier2(cmd, &dep);
}

void copy_target(VkCommandBuffer cmd, const ideatek_vulkan_swapchain_t *vk)
{
    const render_target_t *t = &render_module.target;
    VkOffset3D end = { (int32_t)t->width, (int32_t)t->height, 1 };
    VkImageBlit region = {
        .srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
        .srcOffsets = { { 0, 0, 0 }, end },
        .dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
        .dstOffsets = { { 0, 0, 0 }, end } };

    move_image(cmd, (VkImage)vk->image,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vkCmdBlitImage(cmd, t->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        (VkImage)vk->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region,
        VK_FILTER_NEAREST);
    move_image(cmd, (VkImage)vk->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
}
