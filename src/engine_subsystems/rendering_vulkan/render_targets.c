/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "render_inside.h"

static const VkImageCreateInfo IMAGE = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .imageType = VK_IMAGE_TYPE_2D, .format = VK_FORMAT_R8G8B8A8_SRGB,
    .mipLevels = 1, .arrayLayers = 1, .samples = VK_SAMPLE_COUNT_1_BIT,
    .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT };
static const VkImageViewCreateInfo VIEW = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = VK_FORMAT_R8G8B8A8_SRGB,
    .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };

VkRect2D get_area(const ideatek_rect_t *r)
{
    const render_target_t *t = &render_module.target;
    float x = fminf(fmaxf(r->x, 0.0f), 1.0f);
    float y = fminf(fmaxf(r->y, 0.0f), 1.0f);
    float w = r->width > 0.0f ? fminf(r->width, 1.0f - x) : 1.0f - x;
    float h = r->height > 0.0f ? fminf(r->height, 1.0f - y) : 1.0f - y;

    return ((VkRect2D){ { (int32_t)(x * t->width), (int32_t)(y * t->height) },
        { (uint32_t)(w * t->width), (uint32_t)(h * t->height) } });
}

static bool make_image(VkDevice dev, render_target_t *t)
{
    VkImageCreateInfo image = IMAGE;
    VkImageViewCreateInfo view = VIEW;
    VkMemoryRequirements need = {};
    VkMemoryAllocateInfo alloc = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    bool ok;

    image.extent = (VkExtent3D){ t->width, t->height, 1 };
    ok = vkCreateImage(dev, &image, nullptr, &t->image) == VK_SUCCESS;
    if (ok)
        vkGetImageMemoryRequirements(dev, t->image, &need);
    alloc.allocationSize = need.size;
    alloc.memoryTypeIndex = find_memory(need.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    ok = ok && vkAllocateMemory(dev, &alloc, nullptr, &t->memory)
        == VK_SUCCESS;
    ok = ok && vkBindImageMemory(dev, t->image, t->memory, 0) == VK_SUCCESS;
    view.image = t->image;
    return (ok && vkCreateImageView(dev, &view, nullptr, &t->view)
        == VK_SUCCESS);
}

bool fit_target(const ideatek_swapchain_t *swap)
{
    render_target_t *t = &render_module.target;
    bool ok;

    if (has_size(swap))
        return (t->view != VK_NULL_HANDLE);
    vkDeviceWaitIdle(render_module.device.device);
    free_target();
    t->width = swap->width;
    t->height = swap->height;
    ok = make_image(render_module.device.device, t);
    if (!ok)
        free_target();
    t->width = swap->width;
    t->height = swap->height;
    return (ok);
}

void free_target(void)
{
    render_target_t *t = &render_module.target;
    VkDevice dev = render_module.device.device;

    if (dev) {
        vkDestroyImageView(dev, t->view, nullptr);
        vkDestroyImage(dev, t->image, nullptr);
        vkFreeMemory(dev, t->memory, nullptr);
    }
    *t = (render_target_t){};
}
