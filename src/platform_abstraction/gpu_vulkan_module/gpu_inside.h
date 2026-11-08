/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef GPU_INSIDE_H
    #define GPU_INSIDE_H

    #include <stddef.h>

    #include <volk.h>

    #include "bridge_native.h"
    #include "ideatek_gpu.h"
    #include "ideatek_gpu_vulkan.h"
    #include "ideatek_window.h"

    #define GPU_SETTINGS_FIELDS { \
        bridge_text_t loader; \
        uint32_t frames_in_flight; \
        uint32_t image_count; \
        bool validation; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(gpu_settings_t, GPU_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(gpu_settings_t, GPU_SETTINGS_FIELDS);

    #define GPU_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define GPU_F10(name) offsetof(gpu_chain_t, f10.features.name)
    #define GPU_F11(name) offsetof(gpu_chain_t, f11.name)
    #define GPU_F12(name) offsetof(gpu_chain_t, f12.name)
    #define GPU_F13(name) offsetof(gpu_chain_t, f13.name)

constexpr uint32_t GPU = 11;
constexpr uint32_t DEFAULT_GPU = 12;
constexpr uint32_t GPU_REQUEST = 13;
constexpr uint32_t GPU_DEVICE = 14;
constexpr uint32_t SWAPCHAIN_REQUEST = 15;
constexpr uint32_t SWAPCHAIN = 16;
constexpr uint32_t VULKAN_DEVICE = 17;
constexpr uint32_t VULKAN_SWAPCHAIN = 18;
constexpr uint32_t WINDOW_STATE = 19;
constexpr uint32_t SETTINGS = 20;

constexpr uint32_t GPU_GPUS = 16;
constexpr uint32_t GPU_IMAGES = 4;
constexpr uint32_t GPU_MAX_LOG = 256;
constexpr uint32_t GPU_FRAMES = 3;
constexpr size_t GPU_PATH = 4096;

typedef struct {
    VkPhysicalDevice handle;
    uint64_t entity;
    ideatek_gpu_feature_t features;
    bool usable;
} gpu_adapter_t;

typedef struct {
    bool started;
    bool broken;
    bool stale;
    bool lost;
    bool remake;
    VkInstance instance;
    VkDebugUtilsMessengerEXT messenger;
    gpu_adapter_t gpus[GPU_GPUS + 1];
    uint32_t gpu_count;
    uint32_t default_gpu;
    uint32_t chosen;
    VkPhysicalDevice physical;
    VkDevice device;
    uint64_t request;
    ideatek_gpu_request_t asked;
    uint32_t generation;
    VkQueue queues[3];
    uint32_t families[3];
    VkSemaphore timeline;
    VkSemaphore taken[GPU_FRAMES];
    VkSemaphore done[GPU_IMAGES];
    VkCommandPool pool;
    VkCommandBuffer buffers[GPU_FRAMES];
    uint32_t frames;
    uint32_t images;
    bool validation;
    uint64_t window;
    ideatek_surface_t surface_kind;
    uint64_t native_display;
    uint64_t native_window;
    uint32_t pixel_width;
    uint32_t pixel_height;
    bool drawable;
    bool window_hdr;
    bool no_present;
    bool swap_asked;
    ideatek_swapchain_request_t swap_request;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkImage swap_images[GPU_IMAGES];
    VkImageView views[GPU_IMAGES];
    uint32_t image_count;
    VkFormat format;
    VkColorSpaceKHR color_space;
    ideatek_swapchain_t swap_value;
    uint32_t swap_width;
    uint32_t swap_height;
    uint64_t frame;
    uint32_t image_index;
    bool got_image;
    uint32_t request_list;
    uint32_t swap_list;
    uint32_t window_list;
} gpu_module_t;

extern gpu_module_t gpu_module;

void keep_dxgi(void);
size_t module_file(char *out, size_t most);
PFN_vkGetInstanceProcAddr open_loader(const char *path);
bool load_vulkan(const bridge_api_t *api, bridge_text_t loader);
void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);

typedef struct {
    VkPhysicalDeviceFeatures2 f10;
    VkPhysicalDeviceVulkan11Features f11;
    VkPhysicalDeviceVulkan12Features f12;
    VkPhysicalDeviceVulkan13Features f13;
} gpu_chain_t;

typedef struct {
    VkStructureType sType;
    const void *pNext;
    uint32_t flags;
    uint64_t first;
    uint64_t second;
} gpu_surface_info_t;

typedef struct {
    VkImageLayout from;
    VkImageLayout to;
    VkSemaphore wait;
    VkSemaphore signal;
    uint64_t value;
} gpu_submit_t;

typedef VkResult (VKAPI_PTR *gpu_make_surface_t)(VkInstance, const void *,
    const VkAllocationCallbacks *, VkSurfaceKHR *);

bool start_gpus(const bridge_api_t *api);
bool make_instance(const bridge_api_t *api);
void list_gpus(const bridge_api_t *api);
void query_chain(VkPhysicalDevice gpu, gpu_chain_t *c);
uint32_t roadmap_lacks(const gpu_chain_t *c);
void find_queues(VkPhysicalDevice gpu);

bool make_device(const bridge_api_t *api, ideatek_gpu_feature_t asked,
    uint64_t asked_gpu);
bool settle(const bridge_api_t *api);
void drop_device(const bridge_api_t *api);
void note_result(const bridge_api_t *api, VkResult result);
void attach_device(const bridge_api_t *api, ideatek_gpu_status_t status,
    ideatek_gpu_feature_t on);

bool make_surface(const bridge_api_t *api);
void drop_surface(const bridge_api_t *api);

bool make_swapchain(const bridge_api_t *api);
void drop_swapchain(void);
void take_images(void);

void attach_round(const bridge_api_t *api, ideatek_swapchain_status_t status);

#endif
