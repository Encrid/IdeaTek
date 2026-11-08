/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gpu_inside.h"

static const char *const WANTED[] = {
    "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_metal_surface",
    "VK_KHR_xlib_surface", "VK_KHR_wayland_surface",
    "VK_KHR_portability_enumeration", "VK_EXT_debug_utils",
};

static const char *const LAYER = "VK_LAYER_KHRONOS_validation";

static const VkApplicationInfo APP = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .apiVersion = VK_API_VERSION_1_4,
};

static VkBool32 VKAPI_CALL on_message(
    [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *data, void *user)
{
    bridge_native_say(user, BRIDGE_LOG_WARN, "%.*s", (int)GPU_MAX_LOG,
        data->pMessage);
    return (VK_FALSE);
}

static const VkDebugUtilsMessengerCreateInfoEXT DEBUG = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
    .pfnUserCallback = on_message,
};

static uint32_t available(const char **out)
{
    VkExtensionProperties list[128];
    uint32_t total = 128;
    uint32_t kept = 0;
    VkResult result;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &total, list);
    total *= result >= 0;
    for (uint32_t w = 0; w < sizeof(WANTED) / sizeof(WANTED[0]); w++) {
        for (uint32_t i = 0; i < total; i++) {
            out[kept] = WANTED[w];
            kept += !strcmp(list[i].extensionName, WANTED[w]);
        }
    }
    return (kept);
}

static bool open_instance(const bridge_api_t *api, VkInstanceCreateInfo *info)
{
    VkResult result = vkCreateInstance(info, nullptr, &gpu_module.instance);

    if (result == VK_ERROR_LAYER_NOT_PRESENT) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no validation layer");
        info->enabledLayerCount = 0;
        result = vkCreateInstance(info, nullptr, &gpu_module.instance);
    }
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no instance: %d", result);
        gpu_module.instance = VK_NULL_HANDLE;
        return (false);
    }
    return (true);
}

bool make_instance(const bridge_api_t *api)
{
    const char *extensions[8];
    VkDebugUtilsMessengerCreateInfoEXT debug = DEBUG;
    VkInstanceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &APP, .ppEnabledExtensionNames = extensions,
        .enabledLayerCount = gpu_module.validation,
        .ppEnabledLayerNames = &LAYER };

    debug.pUserData = (void *)api;
    info.enabledExtensionCount = available(extensions);
    info.pNext = gpu_module.validation ? &debug : nullptr;
    if (!open_instance(api, &info))
        return (false);
    volkLoadInstance(gpu_module.instance);
    if (gpu_module.validation)
        vkCreateDebugUtilsMessengerEXT(gpu_module.instance, &debug, nullptr,
            &gpu_module.messenger);
    return (true);
}
