/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const struct {
    ideatek_surface_t kind;
    const char *call;
    VkStructureType type;
    uint32_t first;
} MAKERS[] = {
    { IDEATEK_SURFACE_WINDOWS, "vkCreateWin32SurfaceKHR",
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, 0 },
    { IDEATEK_SURFACE_METAL, "vkCreateMetalSurfaceEXT",
        VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT, 1 },
    { IDEATEK_SURFACE_XLIB, "vkCreateXlibSurfaceKHR",
        VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, 0 },
    { IDEATEK_SURFACE_WAYLAND, "vkCreateWaylandSurfaceKHR",
        VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, 0 },
};

static gpu_make_surface_t find_maker(gpu_surface_info_t *info)
{
    uint64_t handles[2] = { gpu_module.native_display,
        gpu_module.native_window };
    gpu_make_surface_t make = nullptr;

    for (size_t i = 0; i < sizeof(MAKERS) / sizeof(MAKERS[0]); i++) {
        if (MAKERS[i].kind != gpu_module.surface_kind)
            continue;
        info->sType = MAKERS[i].type;
        info->first = handles[MAKERS[i].first];
        make = (gpu_make_surface_t)vkGetInstanceProcAddr(gpu_module.instance,
            MAKERS[i].call);
    }
    return (make);
}

static bool can_present(const bridge_api_t *api)
{
    VkBool32 ok = VK_FALSE;
    VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(
        gpu_module.physical, gpu_module.families[0], gpu_module.surface,
        &ok);
    bool refused = (result < 0) | !ok;

    if (refused) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no present on the queue: %d", result);
        drop_surface(api);
    }
    gpu_module.no_present = refused;
    return (!refused);
}

bool make_surface(const bridge_api_t *api)
{
    gpu_surface_info_t info = { .second = gpu_module.native_window };
    gpu_make_surface_t make = find_maker(&info);
    VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;

    if (make)
        result = make(gpu_module.instance, &info, nullptr, &gpu_module.surface);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no surface: %d", result);
        gpu_module.surface = VK_NULL_HANDLE;
        return (false);
    }
    return (can_present(api));
}
