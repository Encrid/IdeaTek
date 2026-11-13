/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static constexpr VkMemoryPropertyFlags HOST =
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

uint32_t find_memory(uint32_t types, VkMemoryPropertyFlags flags)
{
    VkPhysicalDeviceMemoryProperties p = {};
    uint32_t found = 0;

    vkGetPhysicalDeviceMemoryProperties(render_module.device.physical, &p);
    for (uint32_t k = p.memoryTypeCount; k-- > 0;)
        if ((types >> k & 1) && (p.memoryTypes[k].propertyFlags & flags)
            == flags)
            found = k;
    return (found);
}

static bool make_read(render_device_t *d, uint32_t k, uint32_t size)
{
    VkBufferCreateInfo info = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size, .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT };
    VkMemoryRequirements need = {};
    VkMemoryAllocateInfo alloc = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    bool ok = vkCreateBuffer(d->device, &info, nullptr, &d->reads[k])
        == VK_SUCCESS;

    if (ok)
        vkGetBufferMemoryRequirements(d->device, d->reads[k], &need);
    alloc.allocationSize = need.size;
    alloc.memoryTypeIndex = find_memory(need.memoryTypeBits, HOST);
    ok = ok && vkAllocateMemory(d->device, &alloc, nullptr,
        &d->read_memory[k]) == VK_SUCCESS;
    ok = ok && vkBindBufferMemory(d->device, d->reads[k], d->read_memory[k],
        0) == VK_SUCCESS;
    return (ok && vkMapMemory(d->device, d->read_memory[k], 0, VK_WHOLE_SIZE,
        0, (void **)&d->mapped[k]) == VK_SUCCESS);
}

bool make_reads(render_device_t *d, uint32_t size)
{
    bool ok = true;

    for (uint32_t k = 0; size && k < RENDER_SLOTS; k++)
        ok = ok && make_read(d, k, size);
    return (ok);
}
