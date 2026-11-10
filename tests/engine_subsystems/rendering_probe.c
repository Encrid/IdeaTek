/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_rendering.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    STRUCT(api, "ideatek:ideatek_rgb_t", ideatek_rgb_t);
    STRUCT(api, "ideatek:ideatek_rect_t", ideatek_rect_t);
    ENUM(api, "ideatek:ideatek_projection_kind_t", ideatek_projection_kind_t);
    ENUM(api, "ideatek:ideatek_topology_t", ideatek_topology_t);
    ENUM(api, "ideatek:ideatek_index_format_t", ideatek_index_format_t);
    ENUM(api, "ideatek:ideatek_semantic_t", ideatek_semantic_t);
    STRUCT(api, "ideatek:ideatek_mesh_stream_t", ideatek_mesh_stream_t);
    STRUCT(api, "ideatek:ideatek_submesh_t", ideatek_submesh_t);
    STRUCT(api, "ideatek:ideatek_mesh_lod_t", ideatek_mesh_lod_t);
    STRUCT(api, "ideatek:ideatek_texture_slot_t", ideatek_texture_slot_t);
    ENUM(api, "ideatek:ideatek_gi_mode_t", ideatek_gi_mode_t);
    ENUM(api, "ideatek:ideatek_alpha_mode_t", ideatek_alpha_mode_t);
    ENUM(api, "ideatek:ideatek_dimension_t", ideatek_dimension_t);
    ENUM(api, "ideatek:ideatek_filter_t", ideatek_filter_t);
    ENUM(api, "ideatek:ideatek_wrap_t", ideatek_wrap_t);
    ENUM(api, "ideatek:ideatek_light_kind_t", ideatek_light_kind_t);
    ENUM(api, "ideatek:ideatek_cast_shadow_t", ideatek_cast_shadow_t);
    ENUM(api, "ideatek:ideatek_fog_shape_t", ideatek_fog_shape_t);
    ENUM(api, "ideatek:ideatek_sky_kind_t", ideatek_sky_kind_t);
    ENUM(api, "ideatek:ideatek_tonemap_curve_t", ideatek_tonemap_curve_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Camera", ideatek_camera_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Camera", ideatek_camera_t);
    STRUCT(api, "ideatek:Projection", ideatek_projection_t);
    STRUCT(api, "ideatek:ProjectionAngles", ideatek_projection_angles_t);
    STRUCT(api, "ideatek:Mesh", ideatek_mesh_t);
    STRUCT(api, "ideatek:MeshInstance", ideatek_mesh_instance_t);
    STRUCT(api, "ideatek:MeshInstances", ideatek_mesh_instances_t);
    STRUCT(api, "ideatek:MeshInstanceValues",
        ideatek_mesh_instance_values_t);
    STRUCT(api, "ideatek:MeshMaterials", ideatek_mesh_materials_t);
    STRUCT(api, "ideatek:Visibility", ideatek_visibility_t);
    STRUCT(api, "ideatek:Affects", ideatek_affects_t);
    STRUCT(api, "ideatek:VisibilityRange", ideatek_visibility_range_t);
    STRUCT(api, "ideatek:Material", ideatek_material_t);
    STRUCT(api, "ideatek:Texture", ideatek_texture_t);
    STRUCT(api, "ideatek:Sampler", ideatek_sampler_t);
    STRUCT(api, "ideatek:Light", ideatek_light_t);
    STRUCT(api, "ideatek:Decal", ideatek_decal_t);
    STRUCT(api, "ideatek:Fog", ideatek_fog_t);
    STRUCT(api, "ideatek:Sky", ideatek_sky_t);
    STRUCT(api, "ideatek:Atmosphere", ideatek_atmosphere_t);
    STRUCT(api, "ideatek:CloudLayer", ideatek_cloud_layer_t);
    STRUCT(api, "ideatek:Exposure", ideatek_exposure_t);
    STRUCT(api, "ideatek:AutoExposure", ideatek_auto_exposure_t);
    STRUCT(api, "ideatek:Tonemap", ideatek_tonemap_t);
    STRUCT(api, "ideatek:Bloom", ideatek_bloom_t);
    STRUCT(api, "ideatek:ColorGrade", ideatek_color_grade_t);
    STRUCT(api, "ideatek:DepthOfField", ideatek_depth_of_field_t);
    STRUCT(api, "ideatek:MotionBlur", ideatek_motion_blur_t);
    STRUCT(api, "ideatek:Vignette", ideatek_vignette_t);
    STRUCT(api, "ideatek:ChromaticAberration", ideatek_chromatic_aberration_t);
    STRUCT(api, "ideatek:FilmGrain", ideatek_film_grain_t);
    STRUCT(api, "ideatek:LensFlare", ideatek_lens_flare_t);
    STRUCT(api, "ideatek:MaterialClearcoat", ideatek_material_clearcoat_t);
    STRUCT(api, "ideatek:MaterialSpecular", ideatek_material_specular_t);
    STRUCT(api, "ideatek:MaterialTransmission",
        ideatek_material_transmission_t);
    STRUCT(api, "ideatek:MaterialAnisotropy", ideatek_material_anisotropy_t);
    STRUCT(api, "ideatek:MaterialDiffuseTransmission",
        ideatek_material_diffuse_transmission_t);
    TAG(api, "ideatek:MaterialUnlit");
    STRUCT(api, "ideatek:ReflectionProbe", ideatek_reflection_probe_t);
    STRUCT(api, "ideatek:IrradianceVolume", ideatek_irradiance_volume_t);
    STRUCT(api, "ideatek:Lightmap", ideatek_lightmap_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    STRUCT(api, "ideatek:ideatek_rgb_t", ideatek_rgb_t);
    STRUCT(api, "ideatek:ideatek_rect_t", ideatek_rect_t);
    ENUM(api, "ideatek:ideatek_projection_kind_t", ideatek_projection_kind_t);
    ENUM(api, "ideatek:ideatek_topology_t", ideatek_topology_t);
    ENUM(api, "ideatek:ideatek_index_format_t", ideatek_index_format_t);
    STRUCT(api, "ideatek:ideatek_texture_slot_t", ideatek_texture_slot_t);
    ENUM(api, "ideatek:ideatek_gi_mode_t", ideatek_gi_mode_t);
    ENUM(api, "ideatek:ideatek_alpha_mode_t", ideatek_alpha_mode_t);
    ENUM(api, "ideatek:ideatek_dimension_t", ideatek_dimension_t);
    ENUM(api, "ideatek:ideatek_filter_t", ideatek_filter_t);
    ENUM(api, "ideatek:ideatek_wrap_t", ideatek_wrap_t);
    ENUM(api, "ideatek:ideatek_light_kind_t", ideatek_light_kind_t);
    ENUM(api, "ideatek:ideatek_cast_shadow_t", ideatek_cast_shadow_t);
    ENUM(api, "ideatek:ideatek_fog_shape_t", ideatek_fog_shape_t);
    ENUM(api, "ideatek:ideatek_sky_kind_t", ideatek_sky_kind_t);
    ENUM(api, "ideatek:ideatek_tonemap_curve_t", ideatek_tonemap_curve_t);
    api->attach(api, id, STRUCT(api, "ideatek:Camera", ideatek_camera_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Projection",
        ideatek_projection_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ProjectionAngles",
        ideatek_projection_angles_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Mesh", ideatek_mesh_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MeshInstance",
        ideatek_mesh_instance_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MeshInstances",
        ideatek_mesh_instances_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MeshInstanceValues",
        ideatek_mesh_instance_values_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MeshMaterials",
        ideatek_mesh_materials_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Visibility",
        ideatek_visibility_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Affects", ideatek_affects_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VisibilityRange",
        ideatek_visibility_range_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Material", ideatek_material_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Texture", ideatek_texture_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Sampler", ideatek_sampler_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Light", ideatek_light_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Decal", ideatek_decal_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Fog", ideatek_fog_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Sky", ideatek_sky_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Atmosphere",
        ideatek_atmosphere_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CloudLayer",
        ideatek_cloud_layer_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Exposure", ideatek_exposure_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AutoExposure",
        ideatek_auto_exposure_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Tonemap", ideatek_tonemap_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Bloom", ideatek_bloom_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ColorGrade",
        ideatek_color_grade_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:DepthOfField",
        ideatek_depth_of_field_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MotionBlur",
        ideatek_motion_blur_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Vignette", ideatek_vignette_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ChromaticAberration",
        ideatek_chromatic_aberration_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:FilmGrain", ideatek_film_grain_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LensFlare", ideatek_lens_flare_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MaterialClearcoat",
        ideatek_material_clearcoat_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MaterialSpecular",
        ideatek_material_specular_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MaterialTransmission",
        ideatek_material_transmission_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MaterialAnisotropy",
        ideatek_material_anisotropy_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MaterialDiffuseTransmission",
        ideatek_material_diffuse_transmission_t), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:MaterialUnlit"), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ReflectionProbe",
        ideatek_reflection_probe_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:IrradianceVolume",
        ideatek_irradiance_volume_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Lightmap", ideatek_lightmap_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_renderer(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    STRUCT(api, "ideatek:ideatek_rgb_t", ideatek_rgb_t);
    STRUCT(api, "ideatek:ideatek_rect_t", ideatek_rect_t);
    ENUM(api, "ideatek:ideatek_projection_kind_t", ideatek_projection_kind_t);
    ENUM(api, "ideatek:ideatek_topology_t", ideatek_topology_t);
    ENUM(api, "ideatek:ideatek_index_format_t", ideatek_index_format_t);
    STRUCT(api, "ideatek:ideatek_texture_slot_t", ideatek_texture_slot_t);
    ENUM(api, "ideatek:ideatek_gi_mode_t", ideatek_gi_mode_t);
    ENUM(api, "ideatek:ideatek_alpha_mode_t", ideatek_alpha_mode_t);
    ENUM(api, "ideatek:ideatek_dimension_t", ideatek_dimension_t);
    ENUM(api, "ideatek:ideatek_filter_t", ideatek_filter_t);
    ENUM(api, "ideatek:ideatek_wrap_t", ideatek_wrap_t);
    ENUM(api, "ideatek:ideatek_light_kind_t", ideatek_light_kind_t);
    ENUM(api, "ideatek:ideatek_cast_shadow_t", ideatek_cast_shadow_t);
    ENUM(api, "ideatek:ideatek_fog_shape_t", ideatek_fog_shape_t);
    ENUM(api, "ideatek:ideatek_sky_kind_t", ideatek_sky_kind_t);
    ENUM(api, "ideatek:ideatek_tonemap_curve_t", ideatek_tonemap_curve_t);
    STRUCT(api, "ideatek:Camera", ideatek_camera_t);
    STRUCT(api, "ideatek:Projection", ideatek_projection_t);
    STRUCT(api, "ideatek:ProjectionAngles", ideatek_projection_angles_t);
    STRUCT(api, "ideatek:Mesh", ideatek_mesh_t);
    STRUCT(api, "ideatek:MeshInstance", ideatek_mesh_instance_t);
    STRUCT(api, "ideatek:MeshInstances", ideatek_mesh_instances_t);
    STRUCT(api, "ideatek:MeshInstanceValues",
        ideatek_mesh_instance_values_t);
    STRUCT(api, "ideatek:MeshMaterials", ideatek_mesh_materials_t);
    STRUCT(api, "ideatek:Visibility", ideatek_visibility_t);
    STRUCT(api, "ideatek:Affects", ideatek_affects_t);
    STRUCT(api, "ideatek:VisibilityRange", ideatek_visibility_range_t);
    STRUCT(api, "ideatek:Material", ideatek_material_t);
    STRUCT(api, "ideatek:Texture", ideatek_texture_t);
    STRUCT(api, "ideatek:Sampler", ideatek_sampler_t);
    STRUCT(api, "ideatek:Light", ideatek_light_t);
    STRUCT(api, "ideatek:Decal", ideatek_decal_t);
    STRUCT(api, "ideatek:Fog", ideatek_fog_t);
    STRUCT(api, "ideatek:Sky", ideatek_sky_t);
    STRUCT(api, "ideatek:Atmosphere", ideatek_atmosphere_t);
    STRUCT(api, "ideatek:CloudLayer", ideatek_cloud_layer_t);
    STRUCT(api, "ideatek:Exposure", ideatek_exposure_t);
    STRUCT(api, "ideatek:AutoExposure", ideatek_auto_exposure_t);
    STRUCT(api, "ideatek:Tonemap", ideatek_tonemap_t);
    STRUCT(api, "ideatek:Bloom", ideatek_bloom_t);
    STRUCT(api, "ideatek:ColorGrade", ideatek_color_grade_t);
    STRUCT(api, "ideatek:DepthOfField", ideatek_depth_of_field_t);
    STRUCT(api, "ideatek:MotionBlur", ideatek_motion_blur_t);
    STRUCT(api, "ideatek:Vignette", ideatek_vignette_t);
    STRUCT(api, "ideatek:ChromaticAberration", ideatek_chromatic_aberration_t);
    STRUCT(api, "ideatek:FilmGrain", ideatek_film_grain_t);
    STRUCT(api, "ideatek:LensFlare", ideatek_lens_flare_t);
    STRUCT(api, "ideatek:MaterialClearcoat", ideatek_material_clearcoat_t);
    STRUCT(api, "ideatek:MaterialSpecular", ideatek_material_specular_t);
    STRUCT(api, "ideatek:MaterialTransmission",
        ideatek_material_transmission_t);
    STRUCT(api, "ideatek:MaterialAnisotropy", ideatek_material_anisotropy_t);
    STRUCT(api, "ideatek:MaterialDiffuseTransmission",
        ideatek_material_diffuse_transmission_t);
    TAG(api, "ideatek:MaterialUnlit");
    STRUCT(api, "ideatek:ReflectionProbe", ideatek_reflection_probe_t);
    STRUCT(api, "ideatek:IrradianceVolume", ideatek_irradiance_volume_t);
    STRUCT(api, "ideatek:Lightmap", ideatek_lightmap_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_RENDERER)
    role_renderer(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
