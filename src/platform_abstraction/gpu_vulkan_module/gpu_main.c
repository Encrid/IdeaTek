/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

gpu_module_t gpu_module;

static const bridge_trigger_t *triggers(const bridge_api_t *api,
    uint32_t list_index, uint32_t *count)
{
    const bridge_list_t *list = bridge_native_get_list(api, list_index);

    *count = list->count;
    return ((const bridge_trigger_t *)(api->private_base + list->at));
}

BRIDGE_NATIVE(init)
{
    declare_types(api);
    add_runs(api);
    return (0);
}

BRIDGE_NATIVE(request)
{
    uint32_t count;
    const bridge_trigger_t *t = triggers(api, gpu_module.request_list, &count);
    uint32_t at;

    for (uint32_t i = 0; i < count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE) {
            drop_device(api);
            gpu_module.request = 0;
            continue;
        }
        at = bridge_native_find_value(api, t[i].id, GPU_REQUEST);
        if (gpu_module.request && gpu_module.request != t[i].id) {
            bridge_native_say(api, BRIDGE_LOG_WARN,
                "a second request %llu gets no device",
                (unsigned long long)t[i].id);
            continue;
        }
        gpu_module.request = t[i].id;
        gpu_module.remake = at != 0;
    }
    return (0);
}

BRIDGE_NATIVE(swap)
{
    uint32_t count;
    const bridge_trigger_t *t = triggers(api, gpu_module.swap_list, &count);
    uint32_t at;

    for (uint32_t i = 0; i < count; i++) {
        at = bridge_native_find_value(api, t[i].id, SWAPCHAIN_REQUEST);
        if (t[i].id != gpu_module.window || !at)
            continue;
        gpu_module.swap_request = *(const ideatek_swapchain_request_t *)(
            api->shared_base + at);
        gpu_module.swap_asked = true;
        drop_swapchain();
    }
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    drop_device(api);
    if (gpu_module.messenger)
        vkDestroyDebugUtilsMessengerEXT(gpu_module.instance,
            gpu_module.messenger, nullptr);
    if (gpu_module.instance)
        vkDestroyInstance(gpu_module.instance, nullptr);
    volkFinalize();
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
