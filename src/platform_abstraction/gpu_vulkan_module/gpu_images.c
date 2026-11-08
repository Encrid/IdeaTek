/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

void take_images(void)
{
    VkImageViewCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = gpu_module.format,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };
    uint32_t count = GPU_IMAGES;
    VkResult result;

    result = vkGetSwapchainImagesKHR(gpu_module.device, gpu_module.swapchain,
        &count, gpu_module.swap_images);
    count *= result >= 0;
    gpu_module.image_count = count;
    for (uint32_t i = 0; i < count; i++) {
        info.image = gpu_module.swap_images[i];
        vkCreateImageView(gpu_module.device, &info, nullptr,
            &gpu_module.views[i]);
    }
    gpu_module.swap_value.width = gpu_module.pixel_width;
    gpu_module.swap_value.height = gpu_module.pixel_height;
}

void drop_swapchain(void)
{
    if (!gpu_module.device)
        return;
    vkDeviceWaitIdle(gpu_module.device);
    for (uint32_t i = 0; i < gpu_module.image_count; i++)
        vkDestroyImageView(gpu_module.device, gpu_module.views[i], nullptr);
    if (gpu_module.swapchain)
        vkDestroySwapchainKHR(gpu_module.device, gpu_module.swapchain,
            nullptr);
    gpu_module.swapchain = VK_NULL_HANDLE;
    gpu_module.image_count = 0;
}
