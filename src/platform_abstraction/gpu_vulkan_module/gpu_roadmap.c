/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

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

bool roadmap_2022(const gpu_chain_t *c)
{
    const VkPhysicalDeviceFeatures *f = &c->f10.features;
    bool v10 = f->fullDrawIndexUint32 && f->imageCubeArray
        && f->independentBlend && f->sampleRateShading
        && f->drawIndirectFirstInstance && f->depthClamp && f->depthBiasClamp
        && f->samplerAnisotropy && f->occlusionQueryPrecise
        && f->fragmentStoresAndAtomics && f->shaderStorageImageExtendedFormats
        && f->shaderUniformBufferArrayDynamicIndexing
        && f->shaderSampledImageArrayDynamicIndexing
        && f->shaderStorageBufferArrayDynamicIndexing
        && f->shaderStorageImageArrayDynamicIndexing;
    bool v12 = c->f12.descriptorIndexing && c->f12.runtimeDescriptorArray
        && c->f12.descriptorBindingPartiallyBound
        && c->f12.descriptorBindingVariableDescriptorCount
        && c->f12.scalarBlockLayout
        && c->f12.shaderSampledImageArrayNonUniformIndexing
        && c->f12.shaderStorageBufferArrayNonUniformIndexing;

    return (v10 && c->f11.samplerYcbcrConversion && v12
        && c->f13.descriptorBindingInlineUniformBlockUpdateAfterBind);
}

