/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define STAND_IN_EXPORT __declspec(dllexport)
#else
#include <dlfcn.h>
#define STAND_IN_EXPORT [[gnu::visibility("default")]]
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

typedef enum {
    PROPERTIES,
    PROPERTIES2,
    CREATE_DEVICE,
    DEVICE_PROC,
    BUFFER_NEEDS,
    BUFFER_NEEDS2,
    IMAGE_NEEDS,
    IMAGE_NEEDS2,
    ALLOCATE,
    WRAPS
} stand_in_call_t;

typedef struct {
    PFN_vkGetInstanceProcAddr loader;
    PFN_vkVoidFunction reals[WRAPS];
    VkInstance instance;
    VkMemoryPropertyFlags flags;
    uint32_t shift;
} stand_in_t;

static const char *const NAMES[WRAPS] = {
    "vkGetPhysicalDeviceMemoryProperties",
    "vkGetPhysicalDeviceMemoryProperties2", "vkCreateDevice",
    "vkGetDeviceProcAddr", "vkGetBufferMemoryRequirements",
    "vkGetBufferMemoryRequirements2", "vkGetImageMemoryRequirements",
    "vkGetImageMemoryRequirements2", "vkAllocateMemory",
};

static stand_in_t stand_in = {};

static PFN_vkGetInstanceProcAddr open_real(void)
{
    const char *flags = getenv("STAND_IN_MEMORY_FLAGS");
#ifdef _WIN32
    HMODULE lib = LoadLibraryA(STAND_IN_LOADER);

    stand_in.loader = lib ? (PFN_vkGetInstanceProcAddr)(void (*)(void))
        GetProcAddress(lib, "vkGetInstanceProcAddr") : nullptr;
#else
    void *lib = dlopen(STAND_IN_LOADER, RTLD_NOW | RTLD_LOCAL);

    stand_in.loader = lib ? (PFN_vkGetInstanceProcAddr)dlsym(lib,
        "vkGetInstanceProcAddr") : nullptr;
#endif
    stand_in.flags = flags ? (VkMemoryPropertyFlags)strtoul(flags, nullptr,
        10) : 0;
    return (stand_in.loader);
}

static uint32_t moved(uint32_t types)
{
    return (types << stand_in.shift | stand_in.shift);
}

static void add_type(VkPhysicalDeviceMemoryProperties *p)
{
    if (p->memoryTypeCount >= VK_MAX_MEMORY_TYPES)
        return;
    memmove(&p->memoryTypes[1], &p->memoryTypes[0],
        p->memoryTypeCount * sizeof(VkMemoryType));
    p->memoryTypes[0] = (VkMemoryType){
        .propertyFlags = stand_in.flags, .heapIndex = 0 };
    p->memoryTypeCount++;
}

static VKAPI_ATTR void VKAPI_CALL properties(VkPhysicalDevice physical,
    VkPhysicalDeviceMemoryProperties *p)
{
    ((PFN_vkGetPhysicalDeviceMemoryProperties)stand_in.reals[PROPERTIES])(
        physical, p);
    add_type(p);
}

static VKAPI_ATTR void VKAPI_CALL properties2(VkPhysicalDevice physical,
    VkPhysicalDeviceMemoryProperties2 *p)
{
    ((PFN_vkGetPhysicalDeviceMemoryProperties2)stand_in.reals[PROPERTIES2])(
        physical, p);
    add_type(&p->memoryProperties);
}

static VKAPI_ATTR VkResult VKAPI_CALL create_device(VkPhysicalDevice physical,
    const VkDeviceCreateInfo *info, const VkAllocationCallbacks *host,
    VkDevice *device)
{
    PFN_vkGetPhysicalDeviceMemoryProperties real =
        (PFN_vkGetPhysicalDeviceMemoryProperties)stand_in.loader(
        stand_in.instance, NAMES[PROPERTIES]);
    VkPhysicalDeviceMemoryProperties p = {};
    VkResult made = ((PFN_vkCreateDevice)stand_in.reals[CREATE_DEVICE])(
        physical, info, host, device);

    if (made == VK_SUCCESS && real) {
        real(physical, &p);
        stand_in.shift = p.memoryTypeCount < VK_MAX_MEMORY_TYPES;
    }
    return (made);
}

static VKAPI_ATTR void VKAPI_CALL buffer_needs(VkDevice device,
    VkBuffer buffer, VkMemoryRequirements *need)
{
    ((PFN_vkGetBufferMemoryRequirements)stand_in.reals[BUFFER_NEEDS])(device,
        buffer, need);
    need->memoryTypeBits = moved(need->memoryTypeBits);
}

static VKAPI_ATTR void VKAPI_CALL buffer_needs2(VkDevice device,
    const VkBufferMemoryRequirementsInfo2 *info, VkMemoryRequirements2 *need)
{
    ((PFN_vkGetBufferMemoryRequirements2)stand_in.reals[BUFFER_NEEDS2])(
        device, info, need);
    need->memoryRequirements.memoryTypeBits =
        moved(need->memoryRequirements.memoryTypeBits);
}

static VKAPI_ATTR void VKAPI_CALL image_needs(VkDevice device, VkImage image,
    VkMemoryRequirements *need)
{
    ((PFN_vkGetImageMemoryRequirements)stand_in.reals[IMAGE_NEEDS])(device,
        image, need);
    need->memoryTypeBits = moved(need->memoryTypeBits);
}

static VKAPI_ATTR void VKAPI_CALL image_needs2(VkDevice device,
    const VkImageMemoryRequirementsInfo2 *info, VkMemoryRequirements2 *need)
{
    ((PFN_vkGetImageMemoryRequirements2)stand_in.reals[IMAGE_NEEDS2])(device,
        info, need);
    need->memoryRequirements.memoryTypeBits =
        moved(need->memoryRequirements.memoryTypeBits);
}

static VKAPI_ATTR VkResult VKAPI_CALL allocate(VkDevice device,
    const VkMemoryAllocateInfo *info, const VkAllocationCallbacks *host,
    VkDeviceMemory *memory)
{
    VkMemoryAllocateInfo real = *info;

    if (stand_in.shift && !info->memoryTypeIndex)
        return (VK_ERROR_OUT_OF_DEVICE_MEMORY);
    real.memoryTypeIndex -= stand_in.shift;
    return (((PFN_vkAllocateMemory)stand_in.reals[ALLOCATE])(device, &real,
        host, memory));
}

static VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL device_proc(VkDevice device,
    const char *name);

static const PFN_vkVoidFunction WRAPPED[WRAPS] = {
    (PFN_vkVoidFunction)properties, (PFN_vkVoidFunction)properties2,
    (PFN_vkVoidFunction)create_device, (PFN_vkVoidFunction)device_proc,
    (PFN_vkVoidFunction)buffer_needs, (PFN_vkVoidFunction)buffer_needs2,
    (PFN_vkVoidFunction)image_needs, (PFN_vkVoidFunction)image_needs2,
    (PFN_vkVoidFunction)allocate,
};

static PFN_vkVoidFunction wrap(const char *name, PFN_vkVoidFunction real)
{
    for (uint32_t k = 0; real && k < WRAPS; k++)
        if (!strcmp(name, NAMES[k])) {
            stand_in.reals[k] = real;
            return (WRAPPED[k]);
        }
    return (real);
}

static VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL device_proc(VkDevice device,
    const char *name)
{
    PFN_vkGetDeviceProcAddr real =
        (PFN_vkGetDeviceProcAddr)stand_in.reals[DEVICE_PROC];

    return (wrap(name, real(device, name)));
}

STAND_IN_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
    VkInstance instance, const char *name)
{
    if (!stand_in.loader && !open_real())
        return (nullptr);
    if (!strcmp(name, "vkGetInstanceProcAddr"))
        return ((PFN_vkVoidFunction)vkGetInstanceProcAddr);
    if (instance)
        stand_in.instance = instance;
    return (wrap(name, stand_in.loader(instance, name)));
}
