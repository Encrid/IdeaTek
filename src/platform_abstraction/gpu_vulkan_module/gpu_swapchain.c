/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_formats.h"
#include "gpu_inside.h"

static const VkSwapchainCreateInfoKHR SWAPCHAIN_INFO = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .clipped = VK_TRUE,
};

static void choose_format(const bridge_api_t *api,
    VkSwapchainCreateInfoKHR *info)
{
    VkSurfaceFormatKHR list[64];
    uint32_t count = 64;
    gpu_format_t chosen;
    bool lost;
    VkResult result;

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_module.physical,
        gpu_module.surface, &count, list);
    count *= result >= 0;
    chosen = pick_format(list, count, gpu_module.swap_request.hdr);
    gpu_module.format = chosen.format;
    gpu_module.color_space = chosen.space;
    gpu_module.swap_value.format = chosen.out_format;
    gpu_module.swap_value.color_space = chosen.out_space;
    info->imageFormat = chosen.format;
    info->imageColorSpace = chosen.space;
    lost = gpu_module.swap_request.hdr
        & (chosen.out_space == IDEATEK_COLOR_SPACE_SRGB);
    if (lost)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no hdr on the window: srgb");
}

static void choose_present(const bridge_api_t *api,
    VkSwapchainCreateInfoKHR *info)
{
    VkPresentModeKHR list[8];
    uint32_t count = 8;
    ideatek_present_mode_t wanted = gpu_module.swap_request.present_mode;
    VkResult result;

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu_module.physical,
        gpu_module.surface, &count, list);
    count *= result >= 0;
    gpu_module.swap_value.present_mode = pick_present(list, count, wanted);
    if (gpu_module.swap_value.present_mode != wanted)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the present mode %u is not there: vsync", wanted);
    info->presentMode = vulkan_present(gpu_module.swap_value.present_mode);
}

static void apply_caps(VkSwapchainCreateInfoKHR *info)
{
    VkSurfaceCapabilitiesKHR caps = {};
    uint32_t most;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu_module.physical,
        gpu_module.surface, &caps);
    most = caps.maxImageCount + !caps.maxImageCount * GPU_IMAGES;
    info->minImageCount = clamp_count(gpu_module.images, caps.minImageCount,
        most);
    info->preTransform = caps.currentTransform;
    info->surface = gpu_module.surface;
    info->imageExtent = (VkExtent2D){ gpu_module.pixel_width,
        gpu_module.pixel_height };
}

bool make_swapchain(const bridge_api_t *api)
{
    VkSwapchainCreateInfoKHR info = SWAPCHAIN_INFO;
    VkResult result;

    apply_caps(&info);
    choose_format(api, &info);
    choose_present(api, &info);
    result = vkCreateSwapchainKHR(gpu_module.device, &info, nullptr,
        &gpu_module.swapchain);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no swapchain: %d", result);
        gpu_module.swapchain = VK_NULL_HANDLE;
        return (false);
    }
    take_images();
    return (true);
}
