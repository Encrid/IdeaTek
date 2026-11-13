/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static const VkImageSubresourceRange COLOR = { VK_IMAGE_ASPECT_COLOR_BIT, 0,
    1, 0, 1 };
static const VkClearColorValue BLACK = { .float32 = { 0, 0, 0, 1 } };
static const VkCommandBufferBeginInfo BEGIN = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

static void clear_view(const bridge_api_t *api, VkCommandBuffer cmd,
    uint64_t id)
{
    const ideatek_camera_t *c = RENDER_FIND(api, id, CAMERA, ideatek_camera_t);
    VkRenderingAttachmentInfo color = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = render_module.target.view,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE, .clearValue.color = BLACK };
    VkRenderingInfo info = { .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = get_area(&c->viewport), .layerCount = 1,
        .colorAttachmentCount = 1, .pColorAttachments = &color };

    vkCmdBeginRendering(cmd, &info);
    vkCmdEndRendering(cmd);
}

static void record_round(const bridge_api_t *api, VkCommandBuffer cmd,
    const ideatek_swapchain_t *swap, const ideatek_vulkan_swapchain_t *vk)
{
    VkImage image = render_module.target.image;
    uint64_t cameras[RENDER_CAMERAS];
    uint32_t drawn = gather_cameras(api, cameras);

    vkBeginCommandBuffer(cmd, &BEGIN);
    move_image(cmd, image, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vkCmdClearColorImage(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        &BLACK, 1, &COLOR);
    move_image(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    for (uint32_t i = 0; i < drawn; i++)
        clear_view(api, cmd, cameras[i]);
    move_image(cmd, image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    record_captures(api, cmd, drawn, swap);
    copy_target(cmd, vk);
    vkEndCommandBuffer(cmd);
}

static void submit_round(VkCommandBuffer cmd)
{
    VkCommandBufferSubmitInfo buffer = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = cmd };
    VkSubmitInfo2 submit = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .commandBufferInfoCount = 1, .pCommandBufferInfos = &buffer };

    vkQueueSubmit2(render_module.device.queue, 1, &submit, VK_NULL_HANDLE);
}

BRIDGE_NATIVE(draw)
{
    const ideatek_frame_t *f = bridge_native_get_single(api,
        render_module.slots[FRAME]);
    const ideatek_swapchain_t *swap;
    const ideatek_vulkan_swapchain_t *vk;
    VkCommandBuffer cmd;

    if (!render_module.started)
        start_render(api);
    render_module.round = f ? f->index : 0;
    if (render_module.broken || !find_device(api))
        return (render_module.broken);
    if (!fit_window(api, &swap, &vk))
        return (0);
    finish_captures(api, swap);
    cmd = render_module.device.buffers[swap->slot];
    record_round(api, cmd, swap, vk);
    submit_round(cmd);
    return (0);
}
