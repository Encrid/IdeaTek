/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "probe.h"

#define CANARY "shaderSubgroupUniformControlFlow"
#define MISSING "VK_KHR_shader_subgroup_uniform_control_flow"
#define GPUS 16

/* A gpu of the kind CPU is the software driver of a runner. The contract of
   the gpu part makes a gpu usable with Roadmap 2022 only, because lavapipe
   lacks 1 feature of Roadmap 2024. When a runner gains that feature, this
   canary fails, and the rule tightens back to Roadmap 2024. */
static bool lacks_2024(VkPhysicalDevice gpu)
{
    uint32_t count = 0;
    VkExtensionProperties *list;
    bool lacks;

    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, nullptr);
    list = calloc(count, sizeof(*list));
    if (!list)
        return (true);
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, list);
    lacks = !has_name(list, count, MISSING);
    free(list);
    return (lacks);
}

static int check(VkInstance instance)
{
    VkPhysicalDevice gpus[GPUS];
    uint32_t count = 0;
    bool quiet = true;

    if (!instance || vkEnumeratePhysicalDevices(instance, &count, nullptr)
        != VK_SUCCESS || !count)
        return (0);
    if (count > GPUS)
        count = GPUS;
    vkEnumeratePhysicalDevices(instance, &count, gpus);
    for (uint32_t i = 0; i < count; i++) {
        VkPhysicalDeviceProperties props = {};

        vkGetPhysicalDeviceProperties(gpus[i], &props);
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU
            && !lacks_2024(gpus[i])) {
            printf("canary 2024: [%s] of the kind CPU has " CANARY "."
                " Tighten the rule of a usable gpu back to Roadmap 2024,"
                " and drop this canary.\n", props.deviceName);
            quiet = false;
        }
    }
    if (quiet)
        printf("canary 2024 quiet\n");
    return (quiet ? 0 : 1);
}

int canary_2024(int argc, char **argv, VkInstance instance)
{
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "--canary-2024") == 0)
            return (check(instance));
    return (-1);
}
