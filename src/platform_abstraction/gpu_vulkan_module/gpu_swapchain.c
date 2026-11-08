/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const VkPresentModeKHR MODES[] = { VK_PRESENT_MODE_FIFO_KHR,
    VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_MAILBOX_KHR,
    VK_PRESENT_MODE_IMMEDIATE_KHR };

static const struct {
    VkFormat format;
    VkColorSpaceKHR space;
    ideatek_image_format_t out_format;
    ideatek_color_space_t out_space;
} HDR[] = {
    { VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT,
        IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM, IDEATEK_COLOR_SPACE_HDR10 },
    { VK_FORMAT_A2R10G10B10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT,
        IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM, IDEATEK_COLOR_SPACE_HDR10 },
    { VK_FORMAT_R16G16B16A16_SFLOAT, VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
        IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT,
        IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR },
};

static void choose_format(const bridge_api_t *api)
{
    VkSurfaceFormatKHR list[64];
    uint32_t count = 0;

    gpu_module.format = VK_FORMAT_B8G8R8A8_SRGB;
    gpu_module.color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    gpu_module.swap_value.format = IDEATEK_IMAGE_FORMAT_BGRA8_SRGB;
    gpu_module.swap_value.color_space = IDEATEK_COLOR_SPACE_SRGB;
    if (!gpu_module.swap_request.hdr)
        return;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_module.physical,
        gpu_module.surface, &count, nullptr);
    count = count < 64 ? count : 64;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_module.physical,
        gpu_module.surface, &count, list);
    for (size_t h = 0; h < sizeof(HDR) / sizeof(HDR[0]); h++)
        for (uint32_t i = 0; i < count; i++)
            if (list[i].format == HDR[h].format
                && list[i].colorSpace == HDR[h].space
                && gpu_module.color_space == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                gpu_module.format = HDR[h].format;
                gpu_module.color_space = HDR[h].space;
                gpu_module.swap_value.format = HDR[h].out_format;
                gpu_module.swap_value.color_space = HDR[h].out_space;
            }
    if (gpu_module.color_space == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no hdr on the window: srgb");
}

static VkPresentModeKHR choose_present(const bridge_api_t *api)
{
    VkPresentModeKHR list[8];
    uint32_t count = 0;
    VkPresentModeKHR wanted = MODES[gpu_module.swap_request.present_mode & 3];

    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu_module.physical,
        gpu_module.surface, &count, nullptr);
    count = count < 8 ? count : 8;
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu_module.physical,
        gpu_module.surface, &count, list);
    gpu_module.swap_value.present_mode = gpu_module.swap_request.present_mode;
    for (uint32_t i = 0; i < count; i++)
        if (list[i] == wanted)
            return (wanted);
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the present mode %u is not there: vsync",
        gpu_module.swap_request.present_mode);
    gpu_module.swap_value.present_mode = IDEATEK_PRESENT_MODE_VSYNC;
    return (VK_PRESENT_MODE_FIFO_KHR);
}

static bool make_views(void)
{
    VkImageViewCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = gpu_module.format,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };
    uint32_t count = GPU_IMAGES;

    vkGetSwapchainImagesKHR(gpu_module.device, gpu_module.swapchain, &count,
        nullptr);
    count = count < GPU_IMAGES ? count : GPU_IMAGES;
    vkGetSwapchainImagesKHR(gpu_module.device, gpu_module.swapchain, &count,
        gpu_module.swap_images);
    gpu_module.image_count = count;
    for (uint32_t i = 0; i < count; i++) {
        info.image = gpu_module.swap_images[i];
        vkCreateImageView(gpu_module.device, &info, nullptr,
            &gpu_module.views[i]);
    }
    return (true);
}

bool make_swapchain(const bridge_api_t *api)
{
    VkSurfaceCapabilitiesKHR caps;
    VkSwapchainCreateInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = gpu_module.surface, .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, .clipped = VK_TRUE };
    VkResult result;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu_module.physical,
        gpu_module.surface, &caps);
    info.minImageCount = gpu_module.images < caps.minImageCount
        ? caps.minImageCount : gpu_module.images;
    if (caps.maxImageCount && info.minImageCount > caps.maxImageCount)
        info.minImageCount = caps.maxImageCount;
    choose_format(api);
    info.imageFormat = gpu_module.format;
    info.imageColorSpace = gpu_module.color_space;
    info.imageExtent = (VkExtent2D){ gpu_module.pixel_width,
        gpu_module.pixel_height };
    info.preTransform = caps.currentTransform;
    info.presentMode = choose_present(api);
    result = vkCreateSwapchainKHR(gpu_module.device, &info, nullptr,
        &gpu_module.swapchain);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no swapchain: %d", result);
        gpu_module.swapchain = VK_NULL_HANDLE;
        return (false);
    }
    gpu_module.swap_width = gpu_module.pixel_width;
    gpu_module.swap_height = gpu_module.pixel_height;
    gpu_module.swap_value.width = gpu_module.pixel_width;
    gpu_module.swap_value.height = gpu_module.pixel_height;
    return (make_views());
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
