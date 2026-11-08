/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gpu_inside.h"

static const ideatek_gpu_kind_t KINDS[] = { IDEATEK_GPU_KIND_OTHER,
    IDEATEK_GPU_KIND_INTEGRATED, IDEATEK_GPU_KIND_DISCRETE,
    IDEATEK_GPU_KIND_VIRTUAL, IDEATEK_GPU_KIND_CPU };

static ideatek_gpu_feature_t features_of(VkPhysicalDevice gpu,
    const gpu_chain_t *c)
{
    VkExtensionProperties list[512];
    uint32_t count = 512;
    ideatek_gpu_feature_t bits = 0;
    VkResult result;

    result = vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, list);
    count *= result >= 0;
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

static uint64_t local_memory(VkPhysicalDevice gpu)
{
    VkPhysicalDeviceMemoryProperties memory;
    uint64_t bytes = 0;
    const VkMemoryHeap *heap;

    vkGetPhysicalDeviceMemoryProperties(gpu, &memory);
    for (uint32_t i = 0; i < memory.memoryHeapCount; i++) {
        heap = &memory.memoryHeaps[i];
        bytes += heap->size * !!(heap->flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT);
    }
    return (bytes);
}

static ideatek_gpu_t read_gpu(VkPhysicalDevice gpu)
{
    VkPhysicalDeviceProperties props;
    gpu_chain_t chain = {};
    ideatek_gpu_t value = {};

    vkGetPhysicalDeviceProperties(gpu, &props);
    query_chain(gpu, &chain);
    memcpy(value.name, props.deviceName, sizeof(value.name) - 1);
    value.vendor_id = props.vendorID;
    value.device_id = props.deviceID;
    value.kind = KINDS[props.deviceType % 5];
    value.memory_bytes = local_memory(gpu);
    value.features = features_of(gpu, &chain);
    value.usable = (props.apiVersion >= VK_API_VERSION_1_4)
        & (roadmap_lacks(&chain) == 0);
    return (value);
}

static void add_gpu(const bridge_api_t *api, uint32_t i,
    VkPhysicalDevice handle, uint64_t *best)
{
    gpu_adapter_t *a = &gpu_module.gpus[i];
    ideatek_gpu_t value = read_gpu(handle);
    uint64_t discrete = value.kind == IDEATEK_GPU_KIND_DISCRETE;
    uint64_t score = (uint64_t)value.usable * ((discrete << 62)
        + (value.memory_bytes >> 2) + 1);
    uint64_t better = score > *best;

    a->handle = handle;
    a->usable = value.usable;
    a->features = value.features;
    a->entity = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, a->entity, GPU, value);
    gpu_module.default_gpu = (uint32_t)(better * i
        + !better * gpu_module.default_gpu);
    *best = better * score + !better * *best;
}

void list_gpus(const bridge_api_t *api)
{
    VkPhysicalDevice handles[GPU_GPUS];
    uint32_t count = GPU_GPUS;
    uint64_t best = 0;
    VkResult result;

    result = vkEnumeratePhysicalDevices(gpu_module.instance, &count, handles);
    count *= result >= 0;
    gpu_module.gpu_count = count;
    gpu_module.default_gpu = GPU_GPUS;
    for (uint32_t i = 0; i < count; i++)
        add_gpu(api, i, handles[i], &best);
    if (gpu_module.default_gpu < GPU_GPUS)
        api->attach(api, gpu_module.gpus[gpu_module.default_gpu].entity,
            DEFAULT_GPU, BRIDGE_ZEROED);
}
