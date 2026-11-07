/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "probe.h"

typedef struct flag {
    const char *name;
    size_t at;
} flag_t;

typedef struct extension {
    const char *name;
    const char *flag;
    VkStructureType type;
} extension_t;

typedef struct ext_flag {
    VkStructureType sType;
    void *pNext;
    VkBool32 on;
} ext_flag_t;

#define FLAG(type, field) { #field, offsetof(type, field) }
#define F10(field) FLAG(VkPhysicalDeviceFeatures2, features.field)
#define F11(field) FLAG(VkPhysicalDeviceVulkan11Features, field)
#define F12(field) FLAG(VkPhysicalDeviceVulkan12Features, field)
#define F13(field) FLAG(VkPhysicalDeviceVulkan13Features, field)
#define F14(field) FLAG(VkPhysicalDeviceVulkan14Features, field)
#define ST(x) VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_##x##_FEATURES_KHR
#define COUNT(table) (sizeof(table) / sizeof((table)[0]))

static const flag_t FLAGS_10[] = {
    F10(fullDrawIndexUint32), F10(imageCubeArray), F10(independentBlend),
    F10(sampleRateShading), F10(drawIndirectFirstInstance), F10(depthClamp),
    F10(depthBiasClamp), F10(samplerAnisotropy), F10(occlusionQueryPrecise),
    F10(fragmentStoresAndAtomics), F10(shaderStorageImageExtendedFormats),
    F10(shaderUniformBufferArrayDynamicIndexing),
    F10(shaderSampledImageArrayDynamicIndexing),
    F10(shaderStorageBufferArrayDynamicIndexing),
    F10(shaderStorageImageArrayDynamicIndexing), F10(multiDrawIndirect),
    F10(shaderInt16), F10(shaderImageGatherExtended),
};

static const flag_t FLAGS_11[] = {
    F11(samplerYcbcrConversion), F11(shaderDrawParameters),
    F11(storageBuffer16BitAccess),
};

static const flag_t FLAGS_12[] = {
    F12(descriptorIndexing), F12(shaderUniformTexelBufferArrayDynamicIndexing),
    F12(shaderStorageTexelBufferArrayDynamicIndexing),
    F12(shaderUniformBufferArrayNonUniformIndexing),
    F12(shaderSampledImageArrayNonUniformIndexing),
    F12(shaderStorageBufferArrayNonUniformIndexing),
    F12(shaderStorageImageArrayNonUniformIndexing),
    F12(shaderUniformTexelBufferArrayNonUniformIndexing),
    F12(shaderStorageTexelBufferArrayNonUniformIndexing),
    F12(descriptorBindingSampledImageUpdateAfterBind),
    F12(descriptorBindingStorageImageUpdateAfterBind),
    F12(descriptorBindingStorageBufferUpdateAfterBind),
    F12(descriptorBindingUniformTexelBufferUpdateAfterBind),
    F12(descriptorBindingStorageTexelBufferUpdateAfterBind),
    F12(descriptorBindingUpdateUnusedWhilePending),
    F12(descriptorBindingPartiallyBound),
    F12(descriptorBindingVariableDescriptorCount), F12(runtimeDescriptorArray),
    F12(scalarBlockLayout), F12(shaderInt8), F12(shaderFloat16),
    F12(storageBuffer8BitAccess), F12(drawIndirectCount),
};

static const flag_t FLAGS_13[] = {
    F13(descriptorBindingInlineUniformBlockUpdateAfterBind),
};

static const flag_t FLAGS_14[] = {
    F14(globalPriorityQuery), F14(shaderSubgroupRotate),
    F14(shaderFloatControls2), F14(shaderExpectAssume),
    F14(vertexAttributeInstanceRateDivisor), F14(indexTypeUint8),
    F14(dynamicRenderingLocalRead), F14(maintenance5), F14(pushDescriptor),
};

static const flag_t LINES[] = {
    F14(rectangularLines), F14(bresenhamLines), F14(smoothLines),
    F14(stippledRectangularLines), F14(stippledBresenhamLines),
    F14(stippledSmoothLines),
};

static const extension_t EXTENSIONS[] = {
    { "VK_KHR_shader_quad_control", "shaderQuadControl", ST(QUAD_CONTROL) },
    { "VK_KHR_shader_maximal_reconvergence", "shaderMaximalReconvergence",
        ST(MAXIMAL_RECONVERGENCE) },
    { "VK_KHR_shader_subgroup_uniform_control_flow",
        "shaderSubgroupUniformControlFlow", ST(SUBGROUP_UNIFORM_CONTROL_FLOW) },
};

static bool is_on(const void *base, const flag_t *flag)
{
    return (*(const VkBool32 *)((const char *)base + flag->at) != 0);
}

static void print_off(const void *base, const flag_t *flags, size_t count)
{
    for (size_t i = 0; i < count; i++)
        if (!is_on(base, &flags[i]))
            printf(" %s", flags[i].name);
}

static bool any_on(const void *base, const flag_t *flags, size_t count)
{
    for (size_t i = 0; i < count; i++)
        if (is_on(base, &flags[i]))
            return (true);
    return (false);
}

typedef struct features {
    VkPhysicalDeviceFeatures2 f10;
    VkPhysicalDeviceVulkan11Features f11;
    VkPhysicalDeviceVulkan12Features f12;
    VkPhysicalDeviceVulkan13Features f13;
    VkPhysicalDeviceVulkan14Features f14;
    ext_flag_t exts[COUNT(EXTENSIONS)];
} features_t;

static void query(VkPhysicalDevice gpu, const VkExtensionProperties *list,
    uint32_t count, features_t *f)
{
    void *next = nullptr;

    for (size_t i = 0; i < COUNT(EXTENSIONS); i++) {
        f->exts[i].sType = EXTENSIONS[i].type;
        f->exts[i].pNext = next;
        f->exts[i].on = has_name(list, count, EXTENSIONS[i].name);
        if (f->exts[i].on)
            next = &f->exts[i];
    }
    f->f14.pNext = next;
    f->f13.pNext = &f->f14;
    f->f12.pNext = &f->f13;
    f->f11.pNext = &f->f12;
    f->f10.pNext = &f->f11;
    vkGetPhysicalDeviceFeatures2(gpu, &f->f10);
}

void print_lacks(VkPhysicalDevice gpu, const VkExtensionProperties *list,
    uint32_t count)
{
    features_t f = {
        .f10.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .f11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .f12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .f13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .f14.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES };

    query(gpu, list, count, &f);
    printf("  lacks:");
    print_off(&f.f10, FLAGS_10, COUNT(FLAGS_10));
    print_off(&f.f11, FLAGS_11, COUNT(FLAGS_11));
    print_off(&f.f12, FLAGS_12, COUNT(FLAGS_12));
    print_off(&f.f13, FLAGS_13, COUNT(FLAGS_13));
    print_off(&f.f14, FLAGS_14, COUNT(FLAGS_14));
    for (size_t i = 0; i < COUNT(EXTENSIONS); i++)
        if (!f.exts[i].on)
            printf(" %s", EXTENSIONS[i].flag);
    printf("%s\n", any_on(&f.f14, LINES, COUNT(LINES)) ? "" : " lines");
}
