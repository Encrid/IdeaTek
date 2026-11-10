/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    GLTF_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_load_state_t",
        ideatek_load_state_t);
    GLTF_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_resource_error_t",
        ideatek_resource_error_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_color_t", ideatek_color_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_rgb_t", ideatek_rgb_t);
    GLTF_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_topology_t",
        ideatek_topology_t);
    GLTF_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_index_format_t",
        ideatek_index_format_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_texture_slot_t",
        ideatek_texture_slot_t);
    GLTF_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_alpha_mode_t",
        ideatek_alpha_mode_t);
}

static void declare_loads(const bridge_api_t *api)
{
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:Resource", ideatek_resource_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLoader",
        ideatek_resource_loader_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLabel",
        ideatek_resource_label_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLoad",
        ideatek_resource_load_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceHandle",
        ideatek_resource_handle_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:RawBytes", ideatek_raw_bytes_t);
    GLTF_TAG(api, "ideatek:ResourceFailed");
    GLTF_TAG(api, "ideatek:ResourceReload");
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    declare_loads(api);
}

static void declare_materials(const bridge_api_t *api)
{
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:Material", ideatek_material_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:MaterialClearcoat",
        ideatek_material_clearcoat_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:MaterialSpecular",
        ideatek_material_specular_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:MaterialTransmission",
        ideatek_material_transmission_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:MaterialAnisotropy",
        ideatek_material_anisotropy_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:MaterialDiffuseTransmission",
        ideatek_material_diffuse_transmission_t);
    GLTF_TAG(api, "ideatek:MaterialUnlit");
}

void declare_outputs(const bridge_api_t *api)
{
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:Model", ideatek_model_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:Mesh", ideatek_mesh_t);
    declare_materials(api);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:SkinBinds", ideatek_skin_binds_t);
    GLTF_TYPE(api, BRIDGE_STRUCT, "ideatek:Clip", ideatek_clip_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek_loader_gltf:Settings",
        gltf_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}
