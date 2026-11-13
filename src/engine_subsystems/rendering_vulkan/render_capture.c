/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static void keep_request(const bridge_api_t *api, uint64_t id, bool leave)
{
    render_capture_t *c = render_module.captures;
    uint32_t free = RENDER_CAPTURES;
    uint32_t k = 0;

    while (k < RENDER_CAPTURES && !(c[k].state && c[k].entity == id))
        k++;
    for (uint32_t i = RENDER_CAPTURES; i-- > 0;)
        if (c[i].state == CAPTURE_FREE)
            free = i;
    if (k < RENDER_CAPTURES)
        c[k].state = !leave * CAPTURE_WAITING;
    else if (!leave && free < RENDER_CAPTURES)
        c[free] = (render_capture_t){ .entity = id, .state = CAPTURE_WAITING };
    else if (!leave)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for captures in round "
            "%llu: 1 are not drawn", (unsigned long long)render_module.round);
}

BRIDGE_NATIVE(request)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        render_module.lists[LIST_REQUESTS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        keep_request(api, t[i].id, t[i].events & BRIDGE_ON_LEAVE);
    return (0);
}

static void copy_part(VkCommandBuffer cmd, render_capture_t *c,
    const ideatek_swapchain_t *swap)
{
    VkBufferImageCopy region = { .imageSubresource = {
        VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
        .imageOffset = { c->at.x, c->at.y, 0 },
        .imageExtent = { c->size.width, c->size.height, 1 } };
    VkMemoryBarrier2 host = { .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
        .srcStageMask = VK_PIPELINE_STAGE_2_COPY_BIT,
        .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_HOST_BIT,
        .dstAccessMask = VK_ACCESS_2_HOST_READ_BIT };
    VkDependencyInfo dep = { .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .memoryBarrierCount = 1, .pMemoryBarriers = &host };

    vkCmdCopyImageToBuffer(cmd, render_module.target.image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        render_module.device.reads[swap->slot], 1, &region);
    vkCmdPipelineBarrier2(cmd, &dep);
    c->state = CAPTURE_COPYING;
    c->slot = swap->slot;
    c->round = render_module.round;
}

static ideatek_capture_status_t get_status(const bridge_api_t *api,
    render_capture_t *c, uint32_t drawn)
{
    const ideatek_capture_request_t *r = RENDER_FIND(api, c->entity,
        CAPTURE_REQUEST, ideatek_capture_request_t);
    VkRect2D area = get_area(&r->area);

    c->at = area.offset;
    c->size = area.extent;
    c->round = render_module.round;
    if (!drawn || r->target.id != render_module.window)
        return (IDEATEK_CAPTURE_STATUS_NO_TARGET);
    if ((uint64_t)area.extent.width * area.extent.height * 4
        > render_module.halves[0].len)
        return (IDEATEK_CAPTURE_STATUS_TOO_BIG);
    return (IDEATEK_CAPTURE_STATUS_OK);
}

void record_captures(const bridge_api_t *api, VkCommandBuffer cmd,
    uint32_t drawn, const ideatek_swapchain_t *swap)
{
    render_capture_t *c;
    ideatek_capture_status_t status;
    bool copied = false;

    for (uint32_t k = 0; k < RENDER_CAPTURES; k++) {
        c = &render_module.captures[k];
        if (c->state != CAPTURE_WAITING || copied)
            continue;
        status = get_status(api, c, drawn);
        if (status != IDEATEK_CAPTURE_STATUS_OK)
            answer_capture(api, c, status);
        else
            copy_part(cmd, c, swap);
        copied = status == IDEATEK_CAPTURE_STATUS_OK;
    }
}
