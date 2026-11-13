/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RENDER_INSIDE_H
    #define RENDER_INSIDE_H

    #include <stddef.h>

    #include <volk.h>

    #include "bridge_native.h"
    #include "ideatek_frame.h"
    #include "ideatek_gpu.h"
    #include "ideatek_gpu_vulkan.h"
    #include "ideatek_rendering.h"
    #include "ideatek_space.h"

    #define RENDER_SETTINGS_FIELDS { \
        double lod_threshold; \
        bool occlusion; \
        bridge_text_t diffuse; \
        bridge_text_t specular; \
        bridge_text_t shading; \
        bool stats; \
        bridge_text_t shader_folder; \
        bridge_text_t world_precision; \
        double origin_distance; \
        double max_instances; \
        double max_lights; \
        double cluster_x; \
        double cluster_y; \
        double cluster_z; \
        double lights_per_cell; \
        double geometry_bytes; \
        double texture_bytes; \
        double capture_bytes; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(render_settings_t, RENDER_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(render_settings_t, RENDER_SETTINGS_FIELDS);

    #define RENDER_TYPE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_TABLE, \
        BRIDGE_ON_DESTROY_REMOVE)
    #define RENDER_ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_ENUM, name, T, BRIDGE_STRIP, BRIDGE_TABLE, \
        BRIDGE_ON_DESTROY_REMOVE)
    #define RENDER_SINGLE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_SINGLETON, \
        BRIDGE_ON_DESTROY_REMOVE)
    #define RENDER_FIND(api, id, slot, T) ((const T *)render_find(api, id, \
        slot))
    #define RENDER_AT(field) offsetof(render_settings_t, field)

constexpr uint32_t RENDER_SLOTS = 3;
constexpr uint32_t RENDER_CAMERAS = 64;
constexpr uint32_t RENDER_CAPTURES = 16;

constexpr uint32_t CAMERA = 0;
constexpr uint32_t PROJECTION = 1;
constexpr uint32_t CAPTURE_REQUEST = 2;
constexpr uint32_t CAPTURE = 3;
constexpr uint32_t STATS = 4;
constexpr uint32_t WORLD = 5;
constexpr uint32_t GPU_DEVICE = 6;
constexpr uint32_t SWAPCHAIN = 7;
constexpr uint32_t VULKAN_DEVICE = 8;
constexpr uint32_t VULKAN_SWAPCHAIN = 9;
constexpr uint32_t FRAME = 10;
constexpr uint32_t SETTINGS = 11;
constexpr uint32_t SLOT_COUNT = 12;

constexpr uint32_t LIST_DEVICE = 0;
constexpr uint32_t LIST_WINDOW = 1;
constexpr uint32_t LIST_CAMERAS = 2;
constexpr uint32_t LIST_REQUESTS = 3;
constexpr uint32_t LIST_COUNT = 4;

constexpr uint32_t CAPTURE_FREE = 0;
constexpr uint32_t CAPTURE_WAITING = 1;
constexpr uint32_t CAPTURE_COPYING = 2;

typedef struct {
    const char *name;
    size_t at;
    double low;
    double high;
    bool open;
    bool even;
    const char *range;
} render_number_t;

typedef struct {
    const char *name;
    size_t at;
    const char *words[2];
} render_words_t;

typedef struct {
    VkInstance instance;
    VkPhysicalDevice physical;
    VkDevice device;
    VkQueue queue;
    VkCommandPool pool;
    VkCommandBuffer buffers[RENDER_SLOTS];
    VkBuffer reads[RENDER_SLOTS];
    VkDeviceMemory read_memory[RENDER_SLOTS];
    uint8_t *mapped[RENDER_SLOTS];
    uint64_t entity;
    uint32_t family;
    bool ready;
    bool refused;
    bool told;
} render_device_t;

typedef struct {
    VkImage image;
    VkImageView view;
    VkDeviceMemory memory;
    uint32_t width;
    uint32_t height;
} render_target_t;

typedef struct {
    uint64_t entity;
    uint64_t round;
    VkOffset2D at;
    VkExtent2D size;
    uint32_t slot;
    uint32_t state;
} render_capture_t;

typedef struct {
    render_settings_t settings;
    render_device_t device;
    render_target_t target;
    render_capture_t captures[RENDER_CAPTURES];
    uint64_t cameras[RENDER_CAMERAS];
    uint64_t window;
    uint64_t round;
    uint32_t slots[SLOT_COUNT];
    uint32_t lists[LIST_COUNT];
    uint32_t camera_count;
    uint32_t choices[4];
    bridge_text_t halves[2];
    uint32_t side;
    bool started;
    bool broken;
} render_module_t;

extern render_module_t render_module;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);
const void *render_find(const bridge_api_t *api, uint64_t id, uint32_t slot);
bool start_render(const bridge_api_t *api);
bool find_device(const bridge_api_t *api);
void free_objects(void);
uint32_t find_memory(uint32_t types, VkMemoryPropertyFlags flags);
bool fit_target(const ideatek_swapchain_t *swap);
bool make_reads(render_device_t *d, uint32_t size);
void move_image(VkCommandBuffer cmd, VkImage image, VkImageLayout from,
    VkImageLayout to);
void free_target(void);
void copy_target(VkCommandBuffer cmd, const ideatek_vulkan_swapchain_t *vk);
uint32_t gather_cameras(const bridge_api_t *api, uint64_t *out);
VkRect2D get_area(const ideatek_rect_t *r);
bool has_size(const ideatek_swapchain_t *swap);
bool fit_window(const bridge_api_t *api, const ideatek_swapchain_t **swap,
    const ideatek_vulkan_swapchain_t **vk);
void skip_captures(const bridge_api_t *api);
void record_captures(const bridge_api_t *api, VkCommandBuffer cmd,
    uint32_t drawn, const ideatek_swapchain_t *swap);
void finish_captures(const bridge_api_t *api, const ideatek_swapchain_t *swap);
void fail_captures(const bridge_api_t *api);
void answer_capture(const bridge_api_t *api, render_capture_t *c,
    ideatek_capture_status_t status);

#endif
