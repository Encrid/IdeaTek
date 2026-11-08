/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gpu_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

gpu_module_t gpu_module;

BRIDGE_NATIVE(init)
{
    declare_types(api);
    add_runs(api);
    return (0);
}

static void leave_request(const bridge_api_t *api, uint64_t id)
{
    drop_device(api);
    api->detach(api, id, GPU_DEVICE);
    api->detach(api, id, VULKAN_DEVICE);
    gpu_module.request = 0;
    gpu_module.remake = false;
}

static void take_request(const bridge_api_t *api, uint64_t id)
{
    ideatek_gpu_request_t r = *(const ideatek_gpu_request_t *)(
        api->shared_base + bridge_native_find_value(api, id, GPU_REQUEST));
    bool changed = memcmp(&r, &gpu_module.asked, sizeof(r)) != 0;

    if (gpu_module.request && gpu_module.request != id) {
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "a second request %llu gets no device", (unsigned long long)id);
        return;
    }
    gpu_module.remake |= changed | !gpu_module.request;
    gpu_module.request = id;
    gpu_module.asked = r;
}

BRIDGE_NATIVE(request)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        gpu_module.request_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE)
            leave_request(api, t[i].id);
        else
            take_request(api, t[i].id);
    }
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
