/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static bool in_range(const bridge_api_t *api, const char *name,
    uint32_t value, uint32_t most)
{
    if (value >= 2 && value <= most)
        return (true);
    bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside 2 to %u", name,
        value, most);
    return (false);
}

bool start_gpus(const bridge_api_t *api)
{
    const gpu_settings_t *s = bridge_native_get_single(api, SETTINGS);
    bool frames_ok = in_range(api, "frames_in_flight", s->frames_in_flight,
        GPU_FRAMES);
    bool images_ok = in_range(api, "image_count", s->image_count, GPU_IMAGES);

    gpu_module.started = true;
    gpu_module.broken = !(frames_ok & images_ok);
    if (gpu_module.broken)
        return (false);
    gpu_module.frames = s->frames_in_flight;
    gpu_module.images = s->image_count;
    gpu_module.validation = s->validation;
    if (load_vulkan(api, s->loader) && make_instance(api))
        list_gpus(api);
    return (true);
}
