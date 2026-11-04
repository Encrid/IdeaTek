/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_gpu.h>
#include <ideatek_gpu_vulkan.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_kind(void)
{
    SIZE(ideatek_gpu_kind_t);
}

static void show_feature(void)
{
    SIZE(ideatek_gpu_feature_t);
}

static void show_api(void)
{
    SIZE(ideatek_gpu_api_t);
}

static void show_status(void)
{
    SIZE(ideatek_gpu_status_t);
}

static void show_present_mode(void)
{
    SIZE(ideatek_present_mode_t);
}

static void show_format(void)
{
    SIZE(ideatek_image_format_t);
}

static void show_color_space(void)
{
    SIZE(ideatek_color_space_t);
}

static void show_swapchain_status(void)
{
    SIZE(ideatek_swapchain_status_t);
}

static void show_gpu(void)
{
    SIZE(ideatek_gpu_t);
    AT(ideatek_gpu_t, name);
    AT(ideatek_gpu_t, memory_bytes);
    AT(ideatek_gpu_t, vendor_id);
    AT(ideatek_gpu_t, device_id);
    AT(ideatek_gpu_t, features);
    AT(ideatek_gpu_t, kind);
    AT(ideatek_gpu_t, usable);
}

static void show_request(void)
{
    SIZE(ideatek_gpu_request_t);
    AT(ideatek_gpu_request_t, gpu);
    AT(ideatek_gpu_request_t, features);
}

static void show_device(void)
{
    SIZE(ideatek_gpu_device_t);
    AT(ideatek_gpu_device_t, gpu);
    AT(ideatek_gpu_device_t, generation);
    AT(ideatek_gpu_device_t, features);
    AT(ideatek_gpu_device_t, api);
    AT(ideatek_gpu_device_t, status);
}

static void show_swapchain_request(void)
{
    SIZE(ideatek_swapchain_request_t);
    AT(ideatek_swapchain_request_t, present_mode);
    AT(ideatek_swapchain_request_t, hdr);
}

static void show_swapchain(void)
{
    SIZE(ideatek_swapchain_t);
    AT(ideatek_swapchain_t, frame);
    AT(ideatek_swapchain_t, width);
    AT(ideatek_swapchain_t, height);
    AT(ideatek_swapchain_t, slot);
    AT(ideatek_swapchain_t, format);
    AT(ideatek_swapchain_t, color_space);
    AT(ideatek_swapchain_t, present_mode);
    AT(ideatek_swapchain_t, status);
}

static void show_vulkan_device(void)
{
    SIZE(ideatek_vulkan_device_t);
    AT(ideatek_vulkan_device_t, instance);
    AT(ideatek_vulkan_device_t, physical_device);
    AT(ideatek_vulkan_device_t, device);
    AT(ideatek_vulkan_device_t, queue);
    AT(ideatek_vulkan_device_t, timeline);
    AT(ideatek_vulkan_device_t, queue_family);
}

static void show_vulkan_swapchain(void)
{
    SIZE(ideatek_vulkan_swapchain_t);
    AT(ideatek_vulkan_swapchain_t, image);
    AT(ideatek_vulkan_swapchain_t, view);
    AT(ideatek_vulkan_swapchain_t, format);
    AT(ideatek_vulkan_swapchain_t, color_space);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_gpu_kind_t_desc, ideatek_gpu_feature_t_desc,
        ideatek_gpu_api_t_desc,
        ideatek_gpu_status_t_desc, ideatek_present_mode_t_desc,
        ideatek_image_format_t_desc, ideatek_color_space_t_desc,
        ideatek_swapchain_status_t_desc, ideatek_gpu_t_desc,
        ideatek_gpu_request_t_desc, ideatek_gpu_device_t_desc,
        ideatek_swapchain_request_t_desc, ideatek_swapchain_t_desc,
        ideatek_vulkan_device_t_desc, ideatek_vulkan_swapchain_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_GPU_KIND_VALUES),
        TEXT_OF(IDEATEK_GPU_FEATURE_VALUES),
        TEXT_OF(IDEATEK_GPU_API_VALUES),
        TEXT_OF(IDEATEK_GPU_STATUS_VALUES),
        TEXT_OF(IDEATEK_PRESENT_MODE_VALUES),
        TEXT_OF(IDEATEK_IMAGE_FORMAT_VALUES),
        TEXT_OF(IDEATEK_COLOR_SPACE_VALUES),
        TEXT_OF(IDEATEK_SWAPCHAIN_STATUS_VALUES),
        TEXT_OF(IDEATEK_GPU_FIELDS),
        TEXT_OF(IDEATEK_GPU_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_GPU_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_SWAPCHAIN_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_SWAPCHAIN_FIELDS),
        TEXT_OF(IDEATEK_VULKAN_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_VULKAN_SWAPCHAIN_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "kind", show_kind }, { "feature", show_feature },
        { "api", show_api },
        { "status", show_status }, { "present_mode", show_present_mode },
        { "format", show_format }, { "color_space", show_color_space },
        { "swapchain_status", show_swapchain_status }, { "gpu", show_gpu },
        { "request", show_request }, { "device", show_device },
        { "swapchain_request", show_swapchain_request },
        { "swapchain", show_swapchain },
        { "vulkan_device", show_vulkan_device },
        { "vulkan_swapchain", show_vulkan_swapchain },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
