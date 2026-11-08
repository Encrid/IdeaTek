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

static VkBool32 VKAPI_CALL on_message(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *data, void *user)
{
    int level = severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
        ? BRIDGE_LOG_ERR : BRIDGE_LOG_WARN;

    (void)type;
    bridge_native_say(user, level, "%.*s", (int)GPU_MAX_LOG, data->pMessage);
    return (VK_FALSE);
}

static bool has_layer(void)
{
    uint32_t count = 0;
    VkLayerProperties list[64];
    bool found = false;

    vkEnumerateInstanceLayerProperties(&count, nullptr);
    count = count < 64 ? count : 64;
    vkEnumerateInstanceLayerProperties(&count, list);
    for (uint32_t i = 0; i < count; i++)
        found |= strcmp(list[i].layerName, LAYER) == 0;
    return (found);
}

static uint32_t available(const char **out)
{
    uint32_t total = 0;
    VkExtensionProperties list[128];
    uint32_t kept = 0;

    vkEnumerateInstanceExtensionProperties(nullptr, &total, nullptr);
    total = total < 128 ? total : 128;
    vkEnumerateInstanceExtensionProperties(nullptr, &total, list);
    for (uint32_t w = 0; w < 7; w++)
        for (uint32_t i = 0; i < total; i++)
            if (strcmp(list[i].extensionName, WANTED[w]) == 0)
                out[kept++] = WANTED[w];
    return (kept);
}

static bool make_instance(const bridge_api_t *api)
{
    const char *extensions[8];
    VkApplicationInfo app = { .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion = VK_API_VERSION_1_4 };
    VkInstanceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app, .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = &LAYER };
    VkDebugUtilsMessengerCreateInfoEXT debug = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = on_message, .pUserData = (void *)api };
    VkResult result;

    if (volkInitialize() != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no loader: %s",
            "the machine has no library of Vulkan");
        return (false);
    }
    info.enabledExtensionCount = available(extensions);
    info.enabledLayerCount = gpu_module.validation && has_layer();
    if (gpu_module.validation && !info.enabledLayerCount)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no validation layer");
    result = vkCreateInstance(&info, nullptr, &gpu_module.instance);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no instance: %d", result);
        gpu_module.instance = VK_NULL_HANDLE;
        return (false);
    }
    volkLoadInstance(gpu_module.instance);
    if (info.enabledLayerCount)
        vkCreateDebugUtilsMessengerEXT(gpu_module.instance, &debug, nullptr,
            &gpu_module.messenger);
    return (true);
}

bool start_gpus(const bridge_api_t *api)
{
    const gpu_settings_t *s = bridge_native_get_single(api, SETTINGS);

    gpu_module.started = true;
    if (s->frames_in_flight < 2 || s->frames_in_flight > GPU_FRAMES) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "frames_in_flight: %u is outside 2 to 3", s->frames_in_flight);
        return (false);
    }
    if (s->image_count < 2 || s->image_count > GPU_IMAGES) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "image_count: %u is outside 2 to 4", s->image_count);
        return (false);
    }
    gpu_module.frames = s->frames_in_flight;
    gpu_module.images = s->image_count;
    gpu_module.validation = s->validation;
    if (make_instance(api))
        list_gpus(api);
    return (true);
}
