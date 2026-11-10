/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static constexpr uint32_t GIVES[] = { MODEL, MESH, MATERIAL, CLEARCOAT,
    SPECULAR, TRANSMISSION, ANISOTROPY, DIFFUSE, UNLIT, BINDS, CLIP, LABEL,
    OUTCOME, LOAD, RELOAD };
static constexpr uint32_t GIVE_COUNT = sizeof(GIVES) / sizeof(GIVES[0]);

static void add_givers(const bridge_api_t *api)
{
    bridge_term_t bytes[1] = {{ BYTES, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t handle[1] = {{ HANDLE, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t raw[1] = {{ RAW, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t failed[1] = {{ FAILED, BRIDGE_READ, BRIDGE_AND, 0 }};
    uint32_t *lists = gltf_module.lists;

    lists[LIST_BYTES] = BRIDGE_NATIVE_OBSERVER(api, "bytes", BRIDGE_ON_ENTER
        | BRIDGE_ON_ATTACH, bytes, 1, GIVES, GIVE_COUNT, nullptr, 0);
    lists[LIST_HANDLES] = BRIDGE_NATIVE_OBSERVER(api, "handles",
        BRIDGE_ON_ENTER, handle, 1, GIVES, GIVE_COUNT, &lists[LIST_BYTES], 1);
    lists[LIST_RAW] = BRIDGE_NATIVE_OBSERVER(api, "raw", BRIDGE_ON_ENTER
        | BRIDGE_ON_ATTACH, raw, 1, GIVES, GIVE_COUNT, &lists[LIST_HANDLES],
        1);
    lists[LIST_FAILED] = BRIDGE_NATIVE_OBSERVER(api, "failed",
        BRIDGE_ON_ENTER, failed, 1, GIVES, GIVE_COUNT, &lists[LIST_RAW], 1);
}

void add_observers(const bridge_api_t *api)
{
    bridge_term_t model[1] = {{ MODEL, BRIDGE_READ, BRIDGE_AND, 0 }};

    add_givers(api);
    gltf_module.lists[LIST_SWAP] = BRIDGE_NATIVE_OBSERVER(api, "swap",
        BRIDGE_ON_ATTACH, model, 1, nullptr, 0, nullptr, 0);
    gltf_module.lists[LIST_GONE] = BRIDGE_NATIVE_OBSERVER(api, "gone",
        BRIDGE_ON_LEAVE, model, 1, nullptr, 0, nullptr, 0);
}

const bridge_trigger_t *get_triggers(const bridge_api_t *api,
    uint32_t list, uint32_t *count)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        gltf_module.lists[list]);

    *count = l->count;
    return ((const void *)(api->private_base + l->at));
}
