/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static const char NAME[] = "gltf";
static const char TYPE[] = "ideatek:Model";
static const char EXTENSIONS[] = "gltf\nglb\n";

static bridge_text_t keep_text(const bridge_api_t *api, const char *text,
    uint32_t len)
{
    uint32_t at = api->alloc(api, len ? len : 1);

    memcpy(api->shared_base + at, text, len);
    return ((bridge_text_t){ at, len });
}

void spawn_loader(const bridge_api_t *api)
{
    ideatek_resource_loader_t l = {
        .name = keep_text(api, NAME, sizeof(NAME) - 1),
        .type = keep_text(api, TYPE, sizeof(TYPE) - 1),
        .extensions = keep_text(api, EXTENSIONS, sizeof(EXTENSIONS) - 1),
        .magic = keep_text(api, "", 0),
        .ranged = false };

    gltf_module.types[DEP_BUFFER] = keep_text(api, "ideatek:RawBytes", 16);
    gltf_module.types[DEP_IMAGE] = keep_text(api, "ideatek:Texture", 15);
    gltf_module.loader = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, gltf_module.loader, LOADER, l);
}
