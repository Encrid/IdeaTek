/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_rendering.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_color(void)
{
    SIZE(ideatek_color_t);
    AT(ideatek_color_t, r);
    AT(ideatek_color_t, g);
    AT(ideatek_color_t, b);
    AT(ideatek_color_t, a);
}

static void show_rgb(void)
{
    SIZE(ideatek_rgb_t);
    AT(ideatek_rgb_t, r);
    AT(ideatek_rgb_t, g);
    AT(ideatek_rgb_t, b);
}

static void show_rect(void)
{
    SIZE(ideatek_rect_t);
    AT(ideatek_rect_t, x);
    AT(ideatek_rect_t, y);
    AT(ideatek_rect_t, width);
    AT(ideatek_rect_t, height);
}

static void show_projection_kind(void)
{
    SIZE(ideatek_projection_kind_t);
}

static void show_topology(void)
{
    SIZE(ideatek_topology_t);
}

static void show_index_format(void)
{
    SIZE(ideatek_index_format_t);
}

static void show_semantic(void)
{
    SIZE(ideatek_semantic_t);
}

static void show_mesh_stream(void)
{
    SIZE(ideatek_mesh_stream_t);
    AT(ideatek_mesh_stream_t, format);
    AT(ideatek_mesh_stream_t, offset);
    AT(ideatek_mesh_stream_t, stride);
    AT(ideatek_mesh_stream_t, target);
    AT(ideatek_mesh_stream_t, semantic);
    AT(ideatek_mesh_stream_t, set);
}

static void show_submesh(void)
{
    SIZE(ideatek_submesh_t);
    AT(ideatek_submesh_t, first);
    AT(ideatek_submesh_t, count);
}

static void show_mesh_lod(void)
{
    SIZE(ideatek_mesh_lod_t);
    AT(ideatek_mesh_lod_t, error);
    AT(ideatek_mesh_lod_t, first);
    AT(ideatek_mesh_lod_t, count);
}

static void show_texture_slot(void)
{
    SIZE(ideatek_texture_slot_t);
    AT(ideatek_texture_slot_t, texture);
    AT(ideatek_texture_slot_t, offset);
    AT(ideatek_texture_slot_t, scale);
    AT(ideatek_texture_slot_t, rotation);
    AT(ideatek_texture_slot_t, texcoord);
}

static void show_gi_mode(void)
{
    SIZE(ideatek_gi_mode_t);
}

static void show_alpha_mode(void)
{
    SIZE(ideatek_alpha_mode_t);
}

static void show_dimension(void)
{
    SIZE(ideatek_dimension_t);
}

static void show_filter(void)
{
    SIZE(ideatek_filter_t);
}

static void show_wrap(void)
{
    SIZE(ideatek_wrap_t);
}

static void show_light_kind(void)
{
    SIZE(ideatek_light_kind_t);
}

static void show_cast_shadow(void)
{
    SIZE(ideatek_cast_shadow_t);
}

static void show_fog_shape(void)
{
    SIZE(ideatek_fog_shape_t);
}

static void show_sky_kind(void)
{
    SIZE(ideatek_sky_kind_t);
}

static void show_tonemap_curve(void)
{
    SIZE(ideatek_tonemap_curve_t);
}

static void show_camera(void)
{
    SIZE(ideatek_camera_t);
    AT(ideatek_camera_t, target);
    AT(ideatek_camera_t, groups);
    AT(ideatek_camera_t, viewport);
    AT(ideatek_camera_t, order);
    AT(ideatek_camera_t, active);
}

static void show_projection(void)
{
    SIZE(ideatek_projection_t);
    AT(ideatek_projection_t, fov);
    AT(ideatek_projection_t, height);
    AT(ideatek_projection_t, near);
    AT(ideatek_projection_t, far);
    AT(ideatek_projection_t, kind);
}

static void show_projection_angles(void)
{
    SIZE(ideatek_projection_angles_t);
    AT(ideatek_projection_angles_t, left);
    AT(ideatek_projection_angles_t, right);
    AT(ideatek_projection_angles_t, up);
    AT(ideatek_projection_angles_t, down);
}

static void show_mesh(void)
{
    SIZE(ideatek_mesh_t);
    AT(ideatek_mesh_t, bytes);
    AT(ideatek_mesh_t, vertex_count);
    AT(ideatek_mesh_t, index_count);
    AT(ideatek_mesh_t, index_offset);
    AT(ideatek_mesh_t, stream_count);
    AT(ideatek_mesh_t, submesh_count);
    AT(ideatek_mesh_t, lod_count);
    AT(ideatek_mesh_t, target_count);
    AT(ideatek_mesh_t, revision);
    AT(ideatek_mesh_t, topology);
    AT(ideatek_mesh_t, index_format);
}

static void show_mesh_instance(void)
{
    SIZE(ideatek_mesh_instance_t);
    AT(ideatek_mesh_instance_t, mesh);
    AT(ideatek_mesh_instance_t, lod_bias);
}

static void show_mesh_instances(void)
{
    SIZE(ideatek_mesh_instances_t);
    AT(ideatek_mesh_instances_t, mesh);
    AT(ideatek_mesh_instances_t, transforms);
    AT(ideatek_mesh_instances_t, count);
    AT(ideatek_mesh_instances_t, revision);
}

static void show_mesh_instance_values(void)
{
    SIZE(ideatek_mesh_instance_values_t);
    AT(ideatek_mesh_instance_values_t, color);
    AT(ideatek_mesh_instance_values_t, values);
}

static void show_mesh_materials(void)
{
    SIZE(ideatek_mesh_materials_t);
    AT(ideatek_mesh_materials_t, materials);
}

static void show_visibility(void)
{
    SIZE(ideatek_visibility_t);
    AT(ideatek_visibility_t, groups);
    AT(ideatek_visibility_t, cast_shadow);
    AT(ideatek_visibility_t, gi_mode);
    AT(ideatek_visibility_t, visible);
}

static void show_affects(void)
{
    SIZE(ideatek_affects_t);
    AT(ideatek_affects_t, groups);
}

static void show_visibility_range(void)
{
    SIZE(ideatek_visibility_range_t);
    AT(ideatek_visibility_range_t, begin);
    AT(ideatek_visibility_range_t, end);
    AT(ideatek_visibility_range_t, begin_margin);
    AT(ideatek_visibility_range_t, end_margin);
}

static void show_material(void)
{
    SIZE(ideatek_material_t);
    AT(ideatek_material_t, base_color_texture);
    AT(ideatek_material_t, metallic_roughness_texture);
    AT(ideatek_material_t, normal_texture);
    AT(ideatek_material_t, occlusion_texture);
    AT(ideatek_material_t, emissive_texture);
    AT(ideatek_material_t, base_color);
    AT(ideatek_material_t, emissive);
    AT(ideatek_material_t, metallic);
    AT(ideatek_material_t, roughness);
    AT(ideatek_material_t, emissive_strength);
    AT(ideatek_material_t, normal_scale);
    AT(ideatek_material_t, occlusion_strength);
    AT(ideatek_material_t, alpha_cutoff);
    AT(ideatek_material_t, alpha_mode);
    AT(ideatek_material_t, double_sided);
}

static void show_texture(void)
{
    SIZE(ideatek_texture_t);
    AT(ideatek_texture_t, bytes);
    AT(ideatek_texture_t, format);
    AT(ideatek_texture_t, width);
    AT(ideatek_texture_t, height);
    AT(ideatek_texture_t, depth_or_layers);
    AT(ideatek_texture_t, mip_count);
    AT(ideatek_texture_t, top_mip);
    AT(ideatek_texture_t, revision);
    AT(ideatek_texture_t, dimension);
}

static void show_sampler(void)
{
    SIZE(ideatek_sampler_t);
    AT(ideatek_sampler_t, max_anisotropy);
    AT(ideatek_sampler_t, min_filter);
    AT(ideatek_sampler_t, mag_filter);
    AT(ideatek_sampler_t, mip_filter);
    AT(ideatek_sampler_t, wrap_u);
    AT(ideatek_sampler_t, wrap_v);
    AT(ideatek_sampler_t, wrap_w);
}

static void show_light(void)
{
    SIZE(ideatek_light_t);
    AT(ideatek_light_t, cookie);
    AT(ideatek_light_t, color);
    AT(ideatek_light_t, intensity);
    AT(ideatek_light_t, range);
    AT(ideatek_light_t, source_radius);
    AT(ideatek_light_t, inner_cone);
    AT(ideatek_light_t, outer_cone);
    AT(ideatek_light_t, kind);
    AT(ideatek_light_t, shadow);
}

static void show_decal(void)
{
    SIZE(ideatek_decal_t);
    AT(ideatek_decal_t, material);
    AT(ideatek_decal_t, half_size);
    AT(ideatek_decal_t, normal_fade);
    AT(ideatek_decal_t, sort_order);
}

static void show_fog(void)
{
    SIZE(ideatek_fog_t);
    AT(ideatek_fog_t, size);
    AT(ideatek_fog_t, albedo);
    AT(ideatek_fog_t, emission);
    AT(ideatek_fog_t, density);
    AT(ideatek_fog_t, height_falloff);
    AT(ideatek_fog_t, anisotropy);
    AT(ideatek_fog_t, edge);
    AT(ideatek_fog_t, shape);
}

static void show_sky(void)
{
    SIZE(ideatek_sky_t);
    AT(ideatek_sky_t, texture);
    AT(ideatek_sky_t, intensity);
    AT(ideatek_sky_t, priority);
    AT(ideatek_sky_t, kind);
}

static void show_atmosphere(void)
{
    SIZE(ideatek_atmosphere_t);
    AT(ideatek_atmosphere_t, rayleigh);
    AT(ideatek_atmosphere_t, ozone);
    AT(ideatek_atmosphere_t, bottom_radius);
    AT(ideatek_atmosphere_t, height);
    AT(ideatek_atmosphere_t, rayleigh_height);
    AT(ideatek_atmosphere_t, mie);
    AT(ideatek_atmosphere_t, mie_absorption);
    AT(ideatek_atmosphere_t, mie_height);
    AT(ideatek_atmosphere_t, mie_anisotropy);
    AT(ideatek_atmosphere_t, ground_albedo);
}

static void show_cloud_layer(void)
{
    SIZE(ideatek_cloud_layer_t);
    AT(ideatek_cloud_layer_t, coverage_texture);
    AT(ideatek_cloud_layer_t, bottom);
    AT(ideatek_cloud_layer_t, thickness);
    AT(ideatek_cloud_layer_t, coverage);
    AT(ideatek_cloud_layer_t, density);
}

static void show_exposure(void)
{
    SIZE(ideatek_exposure_t);
    AT(ideatek_exposure_t, ev100);
}

static void show_auto_exposure(void)
{
    SIZE(ideatek_auto_exposure_t);
    AT(ideatek_auto_exposure_t, min_ev);
    AT(ideatek_auto_exposure_t, max_ev);
    AT(ideatek_auto_exposure_t, compensation);
    AT(ideatek_auto_exposure_t, speed_up);
    AT(ideatek_auto_exposure_t, speed_down);
}

static void show_tonemap(void)
{
    SIZE(ideatek_tonemap_t);
    AT(ideatek_tonemap_t, curve);
}

static void show_bloom(void)
{
    SIZE(ideatek_bloom_t);
    AT(ideatek_bloom_t, intensity);
    AT(ideatek_bloom_t, threshold);
}

static void show_color_grade(void)
{
    SIZE(ideatek_color_grade_t);
    AT(ideatek_color_grade_t, lut);
    AT(ideatek_color_grade_t, saturation);
    AT(ideatek_color_grade_t, contrast);
}

static void show_depth_of_field(void)
{
    SIZE(ideatek_depth_of_field_t);
    AT(ideatek_depth_of_field_t, focus_distance);
    AT(ideatek_depth_of_field_t, f_stop);
    AT(ideatek_depth_of_field_t, sensor_width);
}

static void show_motion_blur(void)
{
    SIZE(ideatek_motion_blur_t);
    AT(ideatek_motion_blur_t, shutter);
}

static void show_vignette(void)
{
    SIZE(ideatek_vignette_t);
    AT(ideatek_vignette_t, intensity);
}

static void show_chromatic_aberration(void)
{
    SIZE(ideatek_chromatic_aberration_t);
    AT(ideatek_chromatic_aberration_t, intensity);
}

static void show_film_grain(void)
{
    SIZE(ideatek_film_grain_t);
    AT(ideatek_film_grain_t, intensity);
}

static void show_lens_flare(void)
{
    SIZE(ideatek_lens_flare_t);
    AT(ideatek_lens_flare_t, bokeh);
    AT(ideatek_lens_flare_t, tint);
    AT(ideatek_lens_flare_t, intensity);
    AT(ideatek_lens_flare_t, threshold);
}

static void show_material_clearcoat(void)
{
    SIZE(ideatek_material_clearcoat_t);
    AT(ideatek_material_clearcoat_t, texture);
    AT(ideatek_material_clearcoat_t, roughness_texture);
    AT(ideatek_material_clearcoat_t, normal_texture);
    AT(ideatek_material_clearcoat_t, factor);
    AT(ideatek_material_clearcoat_t, roughness);
}

static void show_material_specular(void)
{
    SIZE(ideatek_material_specular_t);
    AT(ideatek_material_specular_t, texture);
    AT(ideatek_material_specular_t, color_texture);
    AT(ideatek_material_specular_t, color);
    AT(ideatek_material_specular_t, factor);
    AT(ideatek_material_specular_t, ior);
}

static void show_material_transmission(void)
{
    SIZE(ideatek_material_transmission_t);
    AT(ideatek_material_transmission_t, texture);
    AT(ideatek_material_transmission_t, thickness_texture);
    AT(ideatek_material_transmission_t, attenuation_color);
    AT(ideatek_material_transmission_t, factor);
    AT(ideatek_material_transmission_t, thickness);
    AT(ideatek_material_transmission_t, attenuation_distance);
}

static void show_material_anisotropy(void)
{
    SIZE(ideatek_material_anisotropy_t);
    AT(ideatek_material_anisotropy_t, texture);
    AT(ideatek_material_anisotropy_t, strength);
    AT(ideatek_material_anisotropy_t, rotation);
}

static void show_material_diffuse_transmission(void)
{
    SIZE(ideatek_material_diffuse_transmission_t);
    AT(ideatek_material_diffuse_transmission_t, texture);
    AT(ideatek_material_diffuse_transmission_t, color_texture);
    AT(ideatek_material_diffuse_transmission_t, color);
    AT(ideatek_material_diffuse_transmission_t, factor);
}

static void show_reflection_probe(void)
{
    SIZE(ideatek_reflection_probe_t);
    AT(ideatek_reflection_probe_t, cube);
    AT(ideatek_reflection_probe_t, half_size);
    AT(ideatek_reflection_probe_t, falloff);
    AT(ideatek_reflection_probe_t, intensity);
    AT(ideatek_reflection_probe_t, priority);
}

static void show_irradiance_volume(void)
{
    SIZE(ideatek_irradiance_volume_t);
    AT(ideatek_irradiance_volume_t, texture);
    AT(ideatek_irradiance_volume_t, half_size);
    AT(ideatek_irradiance_volume_t, intensity);
    AT(ideatek_irradiance_volume_t, priority);
}

static void show_lightmap(void)
{
    SIZE(ideatek_lightmap_t);
    AT(ideatek_lightmap_t, texture);
    AT(ideatek_lightmap_t, uv_rect);
    AT(ideatek_lightmap_t, texcoord);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_color_t_desc, ideatek_rgb_t_desc, ideatek_rect_t_desc,
        ideatek_projection_kind_t_desc, ideatek_topology_t_desc,
        ideatek_index_format_t_desc, ideatek_semantic_t_desc,
        ideatek_mesh_stream_t_desc, ideatek_submesh_t_desc,
        ideatek_mesh_lod_t_desc, ideatek_texture_slot_t_desc,
        ideatek_gi_mode_t_desc, ideatek_alpha_mode_t_desc,
        ideatek_dimension_t_desc, ideatek_filter_t_desc, ideatek_wrap_t_desc,
        ideatek_light_kind_t_desc, ideatek_cast_shadow_t_desc,
        ideatek_fog_shape_t_desc, ideatek_sky_kind_t_desc,
        ideatek_tonemap_curve_t_desc, ideatek_camera_t_desc,
        ideatek_projection_t_desc, ideatek_projection_angles_t_desc,
        ideatek_mesh_t_desc, ideatek_mesh_instance_t_desc,
        ideatek_mesh_instances_t_desc,
        ideatek_mesh_instance_values_t_desc, ideatek_mesh_materials_t_desc,
        ideatek_visibility_t_desc, ideatek_affects_t_desc,
        ideatek_visibility_range_t_desc, ideatek_material_t_desc,
        ideatek_texture_t_desc, ideatek_sampler_t_desc, ideatek_light_t_desc,
        ideatek_decal_t_desc, ideatek_fog_t_desc, ideatek_sky_t_desc,
        ideatek_atmosphere_t_desc, ideatek_cloud_layer_t_desc,
        ideatek_exposure_t_desc, ideatek_auto_exposure_t_desc,
        ideatek_tonemap_t_desc, ideatek_bloom_t_desc,
        ideatek_color_grade_t_desc, ideatek_depth_of_field_t_desc,
        ideatek_motion_blur_t_desc, ideatek_vignette_t_desc,
        ideatek_chromatic_aberration_t_desc, ideatek_film_grain_t_desc,
        ideatek_lens_flare_t_desc, ideatek_material_clearcoat_t_desc,
        ideatek_material_specular_t_desc, ideatek_material_transmission_t_desc,
        ideatek_material_anisotropy_t_desc,
        ideatek_material_diffuse_transmission_t_desc,
        ideatek_reflection_probe_t_desc, ideatek_irradiance_volume_t_desc,
        ideatek_lightmap_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_COLOR_FIELDS), TEXT_OF(IDEATEK_RGB_FIELDS),
        TEXT_OF(IDEATEK_RECT_FIELDS), TEXT_OF(IDEATEK_PROJECTION_KIND_VALUES),
        TEXT_OF(IDEATEK_TOPOLOGY_VALUES), TEXT_OF(IDEATEK_INDEX_FORMAT_VALUES),
        TEXT_OF(IDEATEK_SEMANTIC_VALUES), TEXT_OF(IDEATEK_MESH_STREAM_FIELDS),
        TEXT_OF(IDEATEK_SUBMESH_FIELDS), TEXT_OF(IDEATEK_MESH_LOD_FIELDS),
        TEXT_OF(IDEATEK_TEXTURE_SLOT_FIELDS), TEXT_OF(IDEATEK_GI_MODE_VALUES),
        TEXT_OF(IDEATEK_ALPHA_MODE_VALUES), TEXT_OF(IDEATEK_DIMENSION_VALUES),
        TEXT_OF(IDEATEK_FILTER_VALUES), TEXT_OF(IDEATEK_WRAP_VALUES),
        TEXT_OF(IDEATEK_LIGHT_KIND_VALUES), TEXT_OF(IDEATEK_CAST_SHADOW_VALUES),
        TEXT_OF(IDEATEK_FOG_SHAPE_VALUES), TEXT_OF(IDEATEK_SKY_KIND_VALUES),
        TEXT_OF(IDEATEK_TONEMAP_CURVE_VALUES), TEXT_OF(IDEATEK_CAMERA_FIELDS),
        TEXT_OF(IDEATEK_PROJECTION_FIELDS),
        TEXT_OF(IDEATEK_PROJECTION_ANGLES_FIELDS), TEXT_OF(IDEATEK_MESH_FIELDS),
        TEXT_OF(IDEATEK_MESH_INSTANCE_FIELDS),
        TEXT_OF(IDEATEK_MESH_INSTANCES_FIELDS),
        TEXT_OF(IDEATEK_MESH_INSTANCE_VALUES_FIELDS),
        TEXT_OF(IDEATEK_MESH_MATERIALS_FIELDS),
        TEXT_OF(IDEATEK_VISIBILITY_FIELDS), TEXT_OF(IDEATEK_AFFECTS_FIELDS),
        TEXT_OF(IDEATEK_VISIBILITY_RANGE_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_FIELDS), TEXT_OF(IDEATEK_TEXTURE_FIELDS),
        TEXT_OF(IDEATEK_SAMPLER_FIELDS), TEXT_OF(IDEATEK_LIGHT_FIELDS),
        TEXT_OF(IDEATEK_DECAL_FIELDS), TEXT_OF(IDEATEK_FOG_FIELDS),
        TEXT_OF(IDEATEK_SKY_FIELDS), TEXT_OF(IDEATEK_ATMOSPHERE_FIELDS),
        TEXT_OF(IDEATEK_CLOUD_LAYER_FIELDS), TEXT_OF(IDEATEK_EXPOSURE_FIELDS),
        TEXT_OF(IDEATEK_AUTO_EXPOSURE_FIELDS), TEXT_OF(IDEATEK_TONEMAP_FIELDS),
        TEXT_OF(IDEATEK_BLOOM_FIELDS), TEXT_OF(IDEATEK_COLOR_GRADE_FIELDS),
        TEXT_OF(IDEATEK_DEPTH_OF_FIELD_FIELDS),
        TEXT_OF(IDEATEK_MOTION_BLUR_FIELDS), TEXT_OF(IDEATEK_VIGNETTE_FIELDS),
        TEXT_OF(IDEATEK_CHROMATIC_ABERRATION_FIELDS),
        TEXT_OF(IDEATEK_FILM_GRAIN_FIELDS), TEXT_OF(IDEATEK_LENS_FLARE_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_CLEARCOAT_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_SPECULAR_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_TRANSMISSION_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_ANISOTROPY_FIELDS),
        TEXT_OF(IDEATEK_MATERIAL_DIFFUSE_TRANSMISSION_FIELDS),
        TEXT_OF(IDEATEK_REFLECTION_PROBE_FIELDS),
        TEXT_OF(IDEATEK_IRRADIANCE_VOLUME_FIELDS),
        TEXT_OF(IDEATEK_LIGHTMAP_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "color", show_color }, { "rgb", show_rgb }, { "rect", show_rect },
        { "projection_kind", show_projection_kind },
        { "topology", show_topology }, { "index_format", show_index_format },
        { "semantic", show_semantic }, { "mesh_stream", show_mesh_stream },
        { "submesh", show_submesh }, { "mesh_lod", show_mesh_lod },
        { "texture_slot", show_texture_slot }, { "gi_mode", show_gi_mode },
        { "alpha_mode", show_alpha_mode }, { "dimension", show_dimension },
        { "filter", show_filter }, { "wrap", show_wrap },
        { "light_kind", show_light_kind }, { "cast_shadow", show_cast_shadow },
        { "fog_shape", show_fog_shape }, { "sky_kind", show_sky_kind },
        { "tonemap_curve", show_tonemap_curve }, { "camera", show_camera },
        { "projection", show_projection },
        { "projection_angles", show_projection_angles }, { "mesh", show_mesh },
        { "mesh_instance", show_mesh_instance },
        { "mesh_instances", show_mesh_instances },
        { "mesh_instance_values", show_mesh_instance_values },
        { "mesh_materials", show_mesh_materials },
        { "visibility", show_visibility }, { "affects", show_affects },
        { "visibility_range", show_visibility_range },
        { "material", show_material }, { "texture", show_texture },
        { "sampler", show_sampler }, { "light", show_light },
        { "decal", show_decal }, { "fog", show_fog }, { "sky", show_sky },
        { "atmosphere", show_atmosphere }, { "cloud_layer", show_cloud_layer },
        { "exposure", show_exposure }, { "auto_exposure", show_auto_exposure },
        { "tonemap", show_tonemap }, { "bloom", show_bloom },
        { "color_grade", show_color_grade },
        { "depth_of_field", show_depth_of_field },
        { "motion_blur", show_motion_blur }, { "vignette", show_vignette },
        { "chromatic_aberration", show_chromatic_aberration },
        { "film_grain", show_film_grain }, { "lens_flare", show_lens_flare },
        { "material_clearcoat", show_material_clearcoat },
        { "material_specular", show_material_specular },
        { "material_transmission", show_material_transmission },
        { "material_anisotropy", show_material_anisotropy },
        { "material_diffuse_transmission", show_material_diffuse_transmission },
        { "reflection_probe", show_reflection_probe },
        { "irradiance_volume", show_irradiance_volume },
        { "lightmap", show_lightmap }, { "descriptors", show_descriptors },
        { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
