/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

typedef struct {
    VkStructureType sType;
    const void *pNext;
    uint32_t flags;
    uint64_t first;
    uint64_t second;
} gpu_surface_info_t;

typedef VkResult (VKAPI_PTR *gpu_make_surface_t)(VkInstance, const void *,
    const VkAllocationCallbacks *, VkSurfaceKHR *);

static const struct {
    ideatek_surface_t kind;
    const char *call;
    VkStructureType type;
    bool display_first;
} MAKERS[] = {
    { IDEATEK_SURFACE_WINDOWS, "vkCreateWin32SurfaceKHR",
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, true },
    { IDEATEK_SURFACE_METAL, "vkCreateMetalSurfaceEXT",
        VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT, false },
    { IDEATEK_SURFACE_XLIB, "vkCreateXlibSurfaceKHR",
        VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, true },
    { IDEATEK_SURFACE_WAYLAND, "vkCreateWaylandSurfaceKHR",
        VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, true },
};

void drop_surface(const bridge_api_t *api)
{
    (void)api;
    drop_swapchain();
    if (gpu_module.surface)
        vkDestroySurfaceKHR(gpu_module.instance, gpu_module.surface, nullptr);
    gpu_module.surface = VK_NULL_HANDLE;
}

bool make_surface(const bridge_api_t *api)
{
    gpu_surface_info_t info = { .first = gpu_module.native_display,
        .second = gpu_module.native_window };
    gpu_make_surface_t make = nullptr;
    VkResult result;

    for (size_t i = 0; i < sizeof(MAKERS) / sizeof(MAKERS[0]); i++) {
        if (MAKERS[i].kind != gpu_module.surface_kind)
            continue;
        info.sType = MAKERS[i].type;
        if (!MAKERS[i].display_first)
            info.first = gpu_module.native_window;
        make = (gpu_make_surface_t)vkGetInstanceProcAddr(gpu_module.instance,
            MAKERS[i].call);
    }
    if (!make) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "no surface: the kind %d is not there", gpu_module.surface_kind);
        return (false);
    }
    result = make(gpu_module.instance, &info, nullptr, &gpu_module.surface);
    if (result != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no surface: %d", result);
        gpu_module.surface = VK_NULL_HANDLE;
        return (false);
    }
    return (true);
}

static void take_state(const bridge_api_t *api, uint64_t id, uint32_t at)
{
    const ideatek_window_state_t *w = (const void *)(api->shared_base + at);
    bool handles_changed = w->native_window != gpu_module.native_window
        || w->surface != gpu_module.surface_kind;

    if (handles_changed || id != gpu_module.window)
        drop_surface(api);
    gpu_module.window = id;
    gpu_module.surface_kind = w->surface;
    gpu_module.native_display = w->native_display;
    gpu_module.native_window = w->native_window;
    gpu_module.minimized = w->minimized;
    if (w->pixel_width != gpu_module.pixel_width
        || w->pixel_height != gpu_module.pixel_height)
        drop_swapchain();
    gpu_module.pixel_width = w->pixel_width;
    gpu_module.pixel_height = w->pixel_height;
}

BRIDGE_NATIVE(window)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        gpu_module.window_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    uint32_t at;

    for (uint32_t i = 0; i < list->count; i++) {
        at = bridge_native_find_value(api, t[i].id, WINDOW_STATE);
        if (t[i].events & BRIDGE_ON_LEAVE || !at) {
            drop_surface(api);
            gpu_module.window = 0;
            continue;
        }
        take_state(api, t[i].id, at);
    }
    return (0);
}
