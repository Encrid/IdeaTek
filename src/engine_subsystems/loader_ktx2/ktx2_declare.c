/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    KTX2_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_load_state_t",
        ideatek_load_state_t);
    KTX2_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_resource_error_t",
        ideatek_resource_error_t);
    KTX2_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_dimension_t",
        ideatek_dimension_t);
    KTX2_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_filter_t",
        ideatek_filter_t);
    KTX2_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_wrap_t", ideatek_wrap_t);
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:Resource", ideatek_resource_t);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLoader",
        ideatek_resource_loader_t);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:Texture", ideatek_texture_t);
    KTX2_TYPE(api, BRIDGE_STRUCT, "ideatek:Sampler", ideatek_sampler_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek_loader_ktx2:Settings",
        ktx2_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}
