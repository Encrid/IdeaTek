/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "probe.h"

static const char *const DEVICE_EXTENSIONS[] = {
    "VK_KHR_swapchain", "VK_KHR_acceleration_structure", "VK_KHR_ray_query",
    "VK_KHR_ray_tracing_pipeline", "VK_KHR_deferred_host_operations",
    "VK_EXT_mesh_shader", "VK_KHR_shader_quad_control",
    "VK_KHR_shader_maximal_reconvergence",
    "VK_KHR_shader_subgroup_uniform_control_flow", "VK_KHR_portability_subset",
};

static uint64_t local_memory(VkPhysicalDevice gpu)
{
    VkPhysicalDeviceMemoryProperties memory;
    uint64_t bytes = 0;

    vkGetPhysicalDeviceMemoryProperties(gpu, &memory);
    for (uint32_t i = 0; i < memory.memoryHeapCount; i++)
        if (memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            bytes += memory.memoryHeaps[i].size;
    return (bytes);
}

static void print_properties(uint32_t index, VkPhysicalDevice gpu)
{
    VkPhysicalDeviceDriverProperties driver = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES };
    VkPhysicalDeviceProperties2 props = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = &driver };
    uint32_t api;

    vkGetPhysicalDeviceProperties2(gpu, &props);
    api = props.properties.apiVersion;
    printf("gpu %u [%s] kind %d api %u.%u.%u driver [%s] [%s] memory %llu\n",
        index, props.properties.deviceName, (int)props.properties.deviceType,
        VK_API_VERSION_MAJOR(api), VK_API_VERSION_MINOR(api),
        VK_API_VERSION_PATCH(api), driver.driverName, driver.driverInfo,
        (unsigned long long)local_memory(gpu));
}

static void print_device_extensions(const VkExtensionProperties *list,
    uint32_t count)
{
    printf("  extensions:");
    for (size_t i = 0; i < sizeof(DEVICE_EXTENSIONS)
        / sizeof(DEVICE_EXTENSIONS[0]); i++)
        if (has_name(list, count, DEVICE_EXTENSIONS[i]))
            printf(" %s", DEVICE_EXTENSIONS[i]);
    printf("\n");
}

static void print_gpu(uint32_t index, VkPhysicalDevice gpu)
{
    uint32_t count = 0;
    VkExtensionProperties *list;

    print_properties(index, gpu);
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, nullptr);
    list = calloc(count, sizeof(*list));
    if (!list)
        return;
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, list);
    print_device_extensions(list, count);
    print_lacks(gpu, list, count);
    free(list);
}

void print_gpus(VkInstance instance)
{
    uint32_t count = 0;
    VkPhysicalDevice *gpus;

    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    printf("gpus %u\n", count);
    gpus = calloc(count, sizeof(*gpus));
    if (!gpus)
        return;
    vkEnumeratePhysicalDevices(instance, &count, gpus);
    for (uint32_t i = 0; i < count; i++)
        print_gpu(i, gpus[i]);
    free(gpus);
}
