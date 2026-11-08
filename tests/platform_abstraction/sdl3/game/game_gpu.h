/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_GPU_H
    #define TEST_SDL3_GAME_GPU_H

    #include "game.h"

    #if defined(GAME_GPU)

    #include "ideatek_gpu.h"
    #include "ideatek_gpu_vulkan.h"

    typedef struct {
        uint32_t gpu;
        uint32_t default_gpu;
        uint32_t request;
        uint32_t device;
        uint32_t vulkan_device;
        uint32_t swapchain_request;
        uint32_t swapchain;
        uint32_t vulkan_swapchain;
    } game_gpu_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_gpu_slots_t;

    #endif

    bool game_gpu_step(const bridge_api_t *api, const char *verb, char *args);
    void game_declare_gpu(const bridge_api_t *api);
    void game_watch_gpu(const bridge_api_t *api);
#endif
