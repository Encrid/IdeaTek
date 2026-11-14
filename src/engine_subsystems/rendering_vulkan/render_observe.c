/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "render_inside.h"

static constexpr uint32_t EVENTS = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH
    | BRIDGE_ON_LEAVE;
static const char *const NAMES[LIST_COUNT] = { "device", "window", "cameras",
    "request" };
static constexpr uint32_t TERMS[LIST_COUNT] = { VULKAN_DEVICE,
    VULKAN_SWAPCHAIN, CAMERA, CAPTURE_REQUEST };

static uint32_t add_observer(const bridge_api_t *api, uint32_t k)
{
    bridge_term_t term[1] = {{ render_module.slots[TERMS[k]], BRIDGE_READ,
        BRIDGE_AND, 0 }};
    uint32_t len = (uint32_t)strlen(NAMES[k]);
    bridge_observer_statement_t s = {
        .name = bridge_native_put(api, NAMES[k], len), .name_len = len,
        .events = EVENTS,
        .terms = bridge_native_put(api, term, sizeof(term)),
        .term_count = 1 };

    return (api->observe(api, bridge_native_put(api, &s, sizeof(s))));
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[2] = { render_module.slots[CAPTURE],
        render_module.slots[STATS] };

    for (uint32_t k = 0; k < LIST_COUNT; k++)
        render_module.lists[k] = add_observer(api, k);
    BRIDGE_NATIVE_ADD_SYSTEM(api, "draw", "last", nullptr, 0, changes, 2,
        nullptr, 0);
}
