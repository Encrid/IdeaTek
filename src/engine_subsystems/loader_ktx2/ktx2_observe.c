/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_inside.h"

static constexpr uint32_t GIVES[] = { TEXTURE, SAMPLER, OUTCOME };

void add_observers(const bridge_api_t *api)
{
    bridge_term_t bytes[1] = {{ BYTES, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t texture[1] = {{ TEXTURE, BRIDGE_READ, BRIDGE_AND, 0 }};

    ktx2_module.lists[LIST_BYTES] = BRIDGE_NATIVE_OBSERVER(api, "bytes",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, bytes, 1, GIVES, 3, nullptr, 0);
    ktx2_module.lists[LIST_SWAP] = BRIDGE_NATIVE_OBSERVER(api, "swap",
        BRIDGE_ON_ATTACH, texture, 1, nullptr, 0, nullptr, 0);
    ktx2_module.lists[LIST_GONE] = BRIDGE_NATIVE_OBSERVER(api, "gone",
        BRIDGE_ON_LEAVE, texture, 1, nullptr, 0, nullptr, 0);
}
