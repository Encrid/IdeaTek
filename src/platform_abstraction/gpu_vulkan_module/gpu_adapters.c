/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gpu_inside.h"

static ideatek_gpu_feature_t features_of(VkPhysicalDevice gpu,
    const gpu_chain_t *c)
{
    VkExtensionProperties list[512];
    uint32_t count = 0;
    ideatek_gpu_feature_t bits = 0;

    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, nullptr);
    count = count < 512 ? count : 512;
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, list);
    for (uint32_t i = 0; i < count; i++) {
        bits |= !strcmp(list[i].extensionName, "VK_KHR_ray_query")
            * IDEATEK_GPU_FEATURE_RAY_QUERY;
        bits |= !strcmp(list[i].extensionName, "VK_KHR_ray_tracing_pipeline")
            * IDEATEK_GPU_FEATURE_RAY_TRACING_PIPELINE;
        bits |= !strcmp(list[i].extensionName, "VK_EXT_mesh_shader")
            * IDEATEK_GPU_FEATURE_MESH_SHADER;
    }
    bits |= c->f12.drawIndirectCount * IDEATEK_GPU_FEATURE_DRAW_INDIRECT_COUNT;
    return (bits);
}

static ideatek_gpu_t read_gpu(VkPhysicalDevice gpu, bool *usable)
{
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memory;
    gpu_chain_t chain = {};
    ideatek_gpu_t value = {};

    vkGetPhysicalDeviceProperties(gpu, &props);
    vkGetPhysicalDeviceMemoryProperties(gpu, &memory);
    query_chain(gpu, &chain);
    memcpy(value.name, props.deviceName, sizeof(value.name) - 1);
    value.vendor_id = props.vendorID;
    value.device_id = props.deviceID;
    value.kind = (ideatek_gpu_kind_t)(props.deviceType
        ? props.deviceType - 1 : IDEATEK_GPU_KIND_OTHER);
    for (uint32_t i = 0; i < memory.memoryHeapCount; i++)
        value.memory_bytes += memory.memoryHeaps[i].flags
            & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ? memory.memoryHeaps[i].size : 0;
    value.features = features_of(gpu, &chain);
    *usable = props.apiVersion >= VK_API_VERSION_1_4 && roadmap_2022(&chain);
    value.usable = *usable;
    return (value);
}

static bool beats(const ideatek_gpu_t *a, const ideatek_gpu_t *b)
{
    bool a_discrete = a->kind == IDEATEK_GPU_KIND_DISCRETE;
    bool b_discrete = b->kind == IDEATEK_GPU_KIND_DISCRETE;

    if (a_discrete != b_discrete)
        return (a_discrete);
    return (a->memory_bytes > b->memory_bytes);
}

void list_gpus(const bridge_api_t *api)
{
    VkPhysicalDevice handles[GPU_GPUS];
    uint32_t count = 0;
    ideatek_gpu_t best = {};
    ideatek_gpu_t value;

    vkEnumeratePhysicalDevices(gpu_module.instance, &count, nullptr);
    count = count < GPU_GPUS ? count : GPU_GPUS;
    vkEnumeratePhysicalDevices(gpu_module.instance, &count, handles);
    gpu_module.gpu_count = count;
    gpu_module.default_gpu = GPU_GPUS;
    for (uint32_t i = 0; i < count; i++) {
        gpu_adapter_t *a = &gpu_module.gpus[i];
        a->handle = handles[i];
        value = read_gpu(handles[i], &a->usable);
        a->features = value.features;
        a->entity = api->spawn(api);
        BRIDGE_NATIVE_ATTACH(api, a->entity, GPU, value);
        if (a->usable && (gpu_module.default_gpu == GPU_GPUS
            || beats(&value, &best))) {
            best = value;
            gpu_module.default_gpu = i;
        }
    }
    if (gpu_module.default_gpu < GPU_GPUS)
        api->attach(api, gpu_module.gpus[gpu_module.default_gpu].entity,
            DEFAULT_GPU, BRIDGE_ZEROED);
}
