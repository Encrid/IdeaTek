/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "probe.h"

static const char *const SURFACES[] = {
    "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_metal_surface",
    "VK_KHR_xlib_surface", "VK_KHR_xcb_surface", "VK_KHR_wayland_surface",
    "VK_EXT_headless_surface", "VK_KHR_portability_enumeration",
};

static PFN_vkGetInstanceProcAddr open_loader(const char *path)
{
#ifdef _WIN32
    HMODULE lib = LoadLibraryA(path);

    return (lib ? (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(
        lib, "vkGetInstanceProcAddr") : nullptr);
#else
    void *lib = dlopen(path, RTLD_NOW | RTLD_LOCAL);

    return (lib ? (PFN_vkGetInstanceProcAddr)dlsym(lib,
        "vkGetInstanceProcAddr") : nullptr);
#endif
}

bool has_name(const VkExtensionProperties *list, uint32_t count,
    const char *name)
{
    for (uint32_t i = 0; i < count; i++)
        if (!strcmp(list[i].extensionName, name))
            return (true);
    return (false);
}

static void print_instance_extensions(void)
{
    uint32_t count = 0;
    VkExtensionProperties *list;

    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    list = calloc(count, sizeof(*list));
    if (!list)
        return;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, list);
    printf("instance extensions:");
    for (size_t i = 0; i < sizeof(SURFACES) / sizeof(SURFACES[0]); i++)
        if (has_name(list, count, SURFACES[i]))
            printf(" %s", SURFACES[i]);
    printf("\n");
    free(list);
}

static bool start(void)
{
    const char *path = getenv("VULKAN_LOADER");
    PFN_vkGetInstanceProcAddr get = path ? open_loader(path) : nullptr;
    uint32_t version = 0;

    if (path && !get) {
        printf("no loader at %s\n", path);
        return (false);
    }
    if (path)
        volkInitializeCustom(get);
    else if (volkInitialize() != VK_SUCCESS) {
        printf("no loader\n");
        return (false);
    }
    vkEnumerateInstanceVersion(&version);
    printf("loader %u.%u.%u\n", VK_API_VERSION_MAJOR(version),
        VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version));
    return (true);
}

int main(void)
{
    VkApplicationInfo app = { .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion = VK_API_VERSION_1_4 };
    VkInstanceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app };
    VkInstance instance;
    VkResult result;

    if (!start())
        return (1);
    print_instance_extensions();
    result = vkCreateInstance(&info, nullptr, &instance);
    if (result != VK_SUCCESS) {
        printf("no instance: %d\n", (int)result);
        return (1);
    }
    volkLoadInstance(instance);
    print_gpus(instance);
    vkDestroyInstance(instance, nullptr);
    return (0);
}
