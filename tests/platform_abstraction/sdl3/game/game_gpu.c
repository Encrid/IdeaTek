/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_gpu.h"

#if defined(GAME_GPU)

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T,     BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T,     BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T,     BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define SWEPT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T,     BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP,     BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_gpu_slots_t game_gpu_slots;

uint32_t game_gpu_queries[4];

static const char *const KINDS[] = { "integrated", "discrete", "virtual",
    "cpu", "other" };
static const char *const APIS[] = { "vulkan" };
static const char *const STATUS[] = { "none", "ready", "lost" };
static const char *const FORMATS[] = { "bgra8", "rgba8", "rgb10a2", "bgr10a2",
    "rgba16" };
static const char *const SPACES[] = { "srgb", "hdr10", "scrgb" };
static const char *const PRESENT[] = { "vsync", "adaptive", "mailbox",
    "immediate" };
static const char *const SWAP_STATUS[] = { "ready", "skipped" };

static void show_gpu(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_gpu_t *g = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u gpu %llu [%s] %s mem %llu vendor %u model %u features 0x%x %s"
        " usable %d", game.round, (unsigned long long)id, g->name,
        KINDS[g->kind], (unsigned long long)g->memory_bytes, g->vendor_id,
        g->device_id, g->features, g->usable ? "yes" : "no", g->usable);
}

static void show_default(const bridge_api_t *api, uint64_t id, const void *cell)
{
    (void)cell;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u default gpu %llu", game.round,
        (unsigned long long)id);
}

static void show_device(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_gpu_device_t *d = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u device %llu gpu %llu gen %u features 0x%x %s %s", game.round,
        (unsigned long long)id, (unsigned long long)d->gpu.id, d->generation,
        d->features, APIS[d->api], STATUS[d->status]);
}

static void show_vulkan_device(const bridge_api_t *api, uint64_t id,
    const void *cell)
{
    const ideatek_vulkan_device_t *v = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u vkdevice %llu inst %llx phy %llx dev %llx q %llx,%llx,%llx"
        " fam %u,%u,%u sem %llx", game.round, (unsigned long long)id,
        v->instance, v->physical_device, v->device, v->queue,
        v->compute_queue, v->transfer_queue, v->queue_family,
        v->compute_family, v->transfer_family, v->timeline);
}

static void show_swapchain(const bridge_api_t *api, uint64_t id,
    const void *cell)
{
    const ideatek_swapchain_t *s = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u swap %llu frame %llu %ux%u slot %u %s %s %s %s", game.round,
        (unsigned long long)id, (unsigned long long)s->frame, s->width,
        s->height, s->slot, FORMATS[s->format], SPACES[s->color_space],
        PRESENT[s->present_mode], SWAP_STATUS[s->status]);
}

static void show_vulkan_swapchain(const bridge_api_t *api, uint64_t id,
    const void *cell)
{
    const ideatek_vulkan_swapchain_t *v = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO,
        "r%u vkswap %llu img %llx view %llx fmt %d space %d", game.round,
        (unsigned long long)id, v->image, v->view, v->format, v->color_space);
}

static void ask_gpu(const bridge_api_t *api, char *args)
{
    ideatek_gpu_request_t r = {};
    uint64_t id = !strcmp(args, "other") && game.second
        ? game.second : game.window;

    if (strcmp(args, "none") && strcmp(args, "other"))
        r.gpu.id = (uint32_t)strtoul(args, nullptr, 0);
    if (!id)
        id = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, id, game_gpu_slots.request, r);
}

static void ask_swap(const bridge_api_t *api, char *args)
{
    ideatek_swapchain_request_t r = {};
    char *left = nullptr;
    const char *mode = strtok_r(args, " ", &left);

    for (uint32_t i = 0; i < 4; i++)
        if (mode && !strcmp(mode, PRESENT[i]))
            r.present_mode = (ideatek_present_mode_t)i;
    r.hdr = strstr(mode ? mode : "", "hdr") != nullptr;
    BRIDGE_NATIVE_ATTACH(api, game.window, game_gpu_slots.swapchain_request, r);
}

static void ask_swap_other(const bridge_api_t *api)
{
    ideatek_swapchain_request_t r = {};
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, game_gpu_slots.swapchain_request, r);
    api->destroy(api, id);
}

bool game_gpu_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "gpureq"))
        ask_gpu(api, args);
    else if (!strcmp(verb, "swapreq"))
        ask_swap(api, args);
    else if (!strcmp(verb, "swapother"))
        ask_swap_other(api);
    else if (!strcmp(verb, "show_gpus"))
        game_rows(api, game_gpu_queries[0], game_gpu_slots.gpu, show_gpu);
    else if (!strcmp(verb, "show_default"))
        game_rows(api, game_gpu_queries[1], game_gpu_slots.default_gpu, show_default);
    else if (!strcmp(verb, "show_device"))
        game_rows(api, game_gpu_queries[2], game_gpu_slots.device, show_device);
    else if (!strcmp(verb, "show_vkdevice"))
        game_rows(api, game_gpu_queries[2], game_gpu_slots.vulkan_device,
            show_vulkan_device);
    else if (!strcmp(verb, "show_swap"))
        game_rows(api, game_gpu_queries[3], game_gpu_slots.swapchain, show_swapchain);
    else if (!strcmp(verb, "show_vkswap"))
        game_rows(api, game_gpu_queries[3], game_gpu_slots.vulkan_swapchain,
            show_vulkan_swapchain);
    else
        done = false;
    return (done);
}

void game_declare_gpu(const bridge_api_t *api)
{
    game_gpu_slots_t *s = &game_gpu_slots;

    ENUM(api, "ideatek:ideatek_gpu_kind_t", ideatek_gpu_kind_t);
    ENUM(api, "ideatek:ideatek_gpu_api_t", ideatek_gpu_api_t);
    ENUM(api, "ideatek:ideatek_gpu_status_t", ideatek_gpu_status_t);
    ENUM(api, "ideatek:ideatek_present_mode_t", ideatek_present_mode_t);
    ENUM(api, "ideatek:ideatek_image_format_t", ideatek_image_format_t);
    ENUM(api, "ideatek:ideatek_color_space_t", ideatek_color_space_t);
    ENUM(api, "ideatek:ideatek_swapchain_status_t", ideatek_swapchain_status_t);
    TYPED(api, BRIDGE_BITMASK, "ideatek:ideatek_gpu_feature_t", ideatek_gpu_feature_t);
    s->gpu = SWEPT(api, "ideatek:Gpu", ideatek_gpu_t);
    s->default_gpu = TAG(api, "ideatek:DefaultGpu");
    s->request = STRUCT(api, "ideatek:GpuRequest", ideatek_gpu_request_t);
    s->device = STRUCT(api, "ideatek:GpuDevice", ideatek_gpu_device_t);
    s->vulkan_device = STRUCT(api, "ideatek:VulkanDevice", ideatek_vulkan_device_t);
    s->swapchain_request = STRUCT(api, "ideatek:SwapchainRequest",
        ideatek_swapchain_request_t);
    s->swapchain = STRUCT(api, "ideatek:Swapchain", ideatek_swapchain_t);
    s->vulkan_swapchain = STRUCT(api, "ideatek:VulkanSwapchain",
        ideatek_vulkan_swapchain_t);
}

BRIDGE_NATIVE(show_gpus)
{
    game_rows(api, game_gpu_queries[0], game_gpu_slots.gpu, show_gpu);
    return (0);
}

BRIDGE_NATIVE(show_default)
{
    game_rows(api, game_gpu_queries[1], game_gpu_slots.default_gpu,
        show_default);
    return (0);
}

BRIDGE_NATIVE(show_device)
{
    game_rows(api, game_gpu_queries[2], game_gpu_slots.device, show_device);
    return (0);
}

BRIDGE_NATIVE(show_vkdevice)
{
    game_rows(api, game_gpu_queries[2], game_gpu_slots.vulkan_device,
        show_vulkan_device);
    return (0);
}

BRIDGE_NATIVE(show_swap)
{
    game_rows(api, game_gpu_queries[3], game_gpu_slots.swapchain,
        show_swapchain);
    return (0);
}

BRIDGE_NATIVE(show_vkswap)
{
    game_rows(api, game_gpu_queries[3], game_gpu_slots.vulkan_swapchain,
        show_vulkan_swapchain);
    return (0);
}

void game_watch_gpu(const bridge_api_t *api)
{
    bridge_term_t term[1] = {{ game_gpu_slots.gpu, BRIDGE_READ, BRIDGE_AND,
        0 }};

    game_gpu_queries[0] = BRIDGE_NATIVE_ADD_SYSTEM(api, "show_gpus", "check",
        term, 1, nullptr, 0, nullptr, 0);
    term[0].slot = game_gpu_slots.default_gpu;
    game_gpu_queries[1] = BRIDGE_NATIVE_ADD_SYSTEM(api, "show_default",
        "check", term, 1, nullptr, 0, nullptr, 0);
    term[0].slot = game_gpu_slots.device;
    game_gpu_queries[2] = BRIDGE_NATIVE_ADD_SYSTEM(api, "show_device",
        "check", term, 1, nullptr, 0, nullptr, 0);
    term[0].slot = game_gpu_slots.swapchain;
    game_gpu_queries[3] = BRIDGE_NATIVE_ADD_SYSTEM(api, "show_swap", "check",
        term, 1, nullptr, 0, nullptr, 0);
}

#endif
