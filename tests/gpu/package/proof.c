/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_gpu.h>
#include <ideatek_vulkan.h>

int main(void)
{
    ideatek_swapchain_request_t request = {
        .present_mode = IDEATEK_PRESENT_MODE_MAILBOX };
    ideatek_vulkan_device_t device = { 0 };

    if (request.hdr || device.queue_family != 0)
        return (1);
    return (sizeof(ideatek_swapchain_t_desc) > 1 ? 0 : 1);
}
