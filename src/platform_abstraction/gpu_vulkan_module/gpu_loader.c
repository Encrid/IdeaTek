/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gpu_inside.h"

static size_t folder_length(const char *file)
{
    size_t end = 0;
    bool sep;

    for (size_t i = 0; file[i]; i++) {
        sep = (file[i] == '/') | (file[i] == '\\');
        end = end * !sep + (i + 1) * sep;
    }
    return (end);
}

static void resolve(const bridge_api_t *api, bridge_text_t loader,
    char *text, char *full)
{
    char file[GPU_PATH];
    bool over = loader.len >= GPU_PATH;
    size_t len = over * (GPU_PATH - 1) + !over * loader.len;
    bool sep;
    bool absolute;
    size_t pre;

    memcpy(text, api->shared_base + loader.at, len);
    text[len] = '\0';
    sep = strpbrk(text, "/\\") != nullptr;
    absolute = (text[0] == '/') | ((len > 1) & (text[1] == ':'));
    module_file(file, GPU_PATH);
    pre = folder_length(file) * (sep & !absolute);
    pre *= pre + len < GPU_PATH;
    memcpy(full, file, pre);
    memcpy(full + pre, text, len + 1);
}

static bool machine_loader(const bridge_api_t *api)
{
    if (volkInitialize() != VK_SUCCESS) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no loader: %s",
            "the machine has no library of Vulkan");
        return (false);
    }
    return (true);
}

bool load_vulkan(const bridge_api_t *api, bridge_text_t loader)
{
    char text[GPU_PATH] = "";
    char full[GPU_PATH];
    PFN_vkGetInstanceProcAddr proc;

    resolve(api, loader, text, full);
    if (!strcmp(text, "machine"))
        return (machine_loader(api));
    proc = open_loader(full);
    if (!proc) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no loader: no library at %s",
            full);
        return (false);
    }
    volkInitializeCustom(proc);
    return (true);
}
