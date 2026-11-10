/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "ktx2_inside.h"

static const char NAME[] = "ktx2";
static const char TYPE[] = "ideatek:Texture";
static const char EXTENSIONS[] = "ktx2\n";
static const char MAGIC[] = "\xAB\x4B\x54\x58\x20\x32\x30\xBB\x0D\x0A\x1A\x0A";

static bridge_text_t keep_text(const bridge_api_t *api, const char *text,
    uint32_t len)
{
    uint32_t at = api->alloc(api, len);

    memcpy(api->shared_base + at, text, len);
    return ((bridge_text_t){ at, len });
}

void spawn_loader(const bridge_api_t *api)
{
    ideatek_resource_loader_t l = {
        .name = keep_text(api, NAME, sizeof(NAME) - 1),
        .type = keep_text(api, TYPE, sizeof(TYPE) - 1),
        .extensions = keep_text(api, EXTENSIONS, sizeof(EXTENSIONS) - 1),
        .magic = keep_text(api, MAGIC, sizeof(MAGIC) - 1),
        .ranged = false };

    ktx2_module.loader = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, ktx2_module.loader, LOADER, l);
}
