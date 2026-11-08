/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static const size_t ROADMAP_2022[] = {
    GPU_F10(fullDrawIndexUint32), GPU_F10(imageCubeArray),
    GPU_F10(independentBlend), GPU_F10(sampleRateShading),
    GPU_F10(drawIndirectFirstInstance), GPU_F10(depthClamp),
    GPU_F10(depthBiasClamp), GPU_F10(samplerAnisotropy),
    GPU_F10(occlusionQueryPrecise), GPU_F10(fragmentStoresAndAtomics),
    GPU_F10(shaderStorageImageExtendedFormats),
    GPU_F10(shaderUniformBufferArrayDynamicIndexing),
    GPU_F10(shaderSampledImageArrayDynamicIndexing),
    GPU_F10(shaderStorageBufferArrayDynamicIndexing),
    GPU_F10(shaderStorageImageArrayDynamicIndexing),
    GPU_F11(samplerYcbcrConversion), GPU_F12(descriptorIndexing),
    GPU_F12(shaderUniformTexelBufferArrayDynamicIndexing),
    GPU_F12(shaderStorageTexelBufferArrayDynamicIndexing),
    GPU_F12(shaderUniformBufferArrayNonUniformIndexing),
    GPU_F12(shaderSampledImageArrayNonUniformIndexing),
    GPU_F12(shaderStorageBufferArrayNonUniformIndexing),
    GPU_F12(shaderStorageImageArrayNonUniformIndexing),
    GPU_F12(shaderUniformTexelBufferArrayNonUniformIndexing),
    GPU_F12(shaderStorageTexelBufferArrayNonUniformIndexing),
    GPU_F12(descriptorBindingSampledImageUpdateAfterBind),
    GPU_F12(descriptorBindingStorageImageUpdateAfterBind),
    GPU_F12(descriptorBindingStorageBufferUpdateAfterBind),
    GPU_F12(descriptorBindingUniformTexelBufferUpdateAfterBind),
    GPU_F12(descriptorBindingStorageTexelBufferUpdateAfterBind),
    GPU_F12(descriptorBindingUpdateUnusedWhilePending),
    GPU_F12(descriptorBindingPartiallyBound),
    GPU_F12(descriptorBindingVariableDescriptorCount),
    GPU_F12(runtimeDescriptorArray), GPU_F12(scalarBlockLayout),
    GPU_F13(descriptorBindingInlineUniformBlockUpdateAfterBind),
};

void query_chain(VkPhysicalDevice gpu, gpu_chain_t *c)
{
    c->f10.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    c->f11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    c->f12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    c->f13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    c->f10.pNext = &c->f11;
    c->f11.pNext = &c->f12;
    c->f12.pNext = &c->f13;
    vkGetPhysicalDeviceFeatures2(gpu, &c->f10);
}

uint32_t roadmap_lacks(const gpu_chain_t *c)
{
    uint32_t off = 0;

    for (size_t i = 0; i < sizeof(ROADMAP_2022) / sizeof(ROADMAP_2022[0]); i++)
        off += !*(const VkBool32 *)((const char *)c + ROADMAP_2022[i]);
    return (off);
}

static uint32_t pick_family(const VkQueueFamilyProperties *list,
    uint32_t count, uint32_t mask, uint32_t want)
{
    uint32_t chosen = count;
    uint32_t hit;

    for (uint32_t i = count; i-- > 0;) {
        hit = (list[i].queueFlags & mask) == want;
        chosen = hit * i + !hit * chosen;
    }
    return (chosen);
}

void find_queues(VkPhysicalDevice gpu)
{
    VkQueueFamilyProperties list[32];
    uint32_t count = 32;
    uint32_t work = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
    uint32_t *f = gpu_module.families;
    uint32_t hit;

    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, list);
    f[0] = pick_family(list, count, VK_QUEUE_GRAPHICS_BIT,
        VK_QUEUE_GRAPHICS_BIT);
    f[1] = pick_family(list, count, work, VK_QUEUE_COMPUTE_BIT);
    f[2] = pick_family(list, count, work | VK_QUEUE_TRANSFER_BIT,
        VK_QUEUE_TRANSFER_BIT);
    for (uint32_t q = 1; q < 3; q++) {
        hit = f[q] < count;
        f[q] = hit * f[q] + !hit * f[0];
    }
}
