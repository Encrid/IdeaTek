/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_RENDERING_H
    #define BC_IDEATEK_RENDERING_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"
    #include "ideatek_space.h"

    #define IDEATEK_COLOR_FIELDS { \
        float r; \
        float g; \
        float b; \
        float a; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_color_t, IDEATEK_COLOR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_color_t, IDEATEK_COLOR_FIELDS);

    #define IDEATEK_RGB_FIELDS { \
        float r; \
        float g; \
        float b; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_rgb_t, IDEATEK_RGB_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_rgb_t, IDEATEK_RGB_FIELDS);

    #define IDEATEK_RECT_FIELDS { \
        float x; \
        float y; \
        float width; \
        float height; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_rect_t, IDEATEK_RECT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_rect_t, IDEATEK_RECT_FIELDS);

    #define IDEATEK_PROJECTION_KIND_VALUES { \
        IDEATEK_PROJECTION_KIND_PERSPECTIVE = 0, \
        IDEATEK_PROJECTION_KIND_ORTHOGRAPHIC = 1, \
        IDEATEK_PROJECTION_KIND_ASYMMETRIC = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_projection_kind_t,
    uint8_t, IDEATEK_PROJECTION_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_projection_kind_t,
    uint8_t, IDEATEK_PROJECTION_KIND_VALUES);

    #define IDEATEK_TOPOLOGY_VALUES { \
        IDEATEK_TOPOLOGY_POINT_LIST = 0, \
        IDEATEK_TOPOLOGY_LINE_LIST = 1, \
        IDEATEK_TOPOLOGY_LINE_STRIP = 2, \
        IDEATEK_TOPOLOGY_TRIANGLE_LIST = 3, \
        IDEATEK_TOPOLOGY_TRIANGLE_STRIP = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_topology_t, uint8_t, IDEATEK_TOPOLOGY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_topology_t, uint8_t, IDEATEK_TOPOLOGY_VALUES);

    #define IDEATEK_INDEX_FORMAT_VALUES { \
        IDEATEK_INDEX_FORMAT_NONE = 0, \
        IDEATEK_INDEX_FORMAT_U16 = 1, \
        IDEATEK_INDEX_FORMAT_U32 = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_index_format_t,
    uint8_t, IDEATEK_INDEX_FORMAT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_index_format_t,
    uint8_t, IDEATEK_INDEX_FORMAT_VALUES);

    #define IDEATEK_SEMANTIC_VALUES { \
        IDEATEK_SEMANTIC_POSITION = 0, \
        IDEATEK_SEMANTIC_NORMAL = 1, \
        IDEATEK_SEMANTIC_TANGENT = 2, \
        IDEATEK_SEMANTIC_TEXCOORD = 3, \
        IDEATEK_SEMANTIC_COLOR = 4, \
        IDEATEK_SEMANTIC_JOINTS = 5, \
        IDEATEK_SEMANTIC_WEIGHTS = 6, \
        IDEATEK_SEMANTIC_MORPH_POSITION = 7, \
        IDEATEK_SEMANTIC_MORPH_NORMAL = 8, \
        IDEATEK_SEMANTIC_MORPH_TANGENT = 9, \
        IDEATEK_SEMANTIC_SOFT_MAX_DISTANCE = 10, \
        IDEATEK_SEMANTIC_SOFT_BACKSTOP_DISTANCE = 11, \
        IDEATEK_SEMANTIC_SOFT_BACKSTOP_RADIUS = 12 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_semantic_t, uint8_t, IDEATEK_SEMANTIC_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_semantic_t, uint8_t, IDEATEK_SEMANTIC_VALUES);

    #define IDEATEK_MESH_STREAM_FIELDS { \
        uint32_t format; \
        uint32_t offset; \
        uint32_t stride; \
        uint16_t target; \
        ideatek_semantic_t semantic; \
        uint8_t set; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mesh_stream_t, IDEATEK_MESH_STREAM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mesh_stream_t, IDEATEK_MESH_STREAM_FIELDS);

    #define IDEATEK_SUBMESH_FIELDS { \
        uint32_t first; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_submesh_t, IDEATEK_SUBMESH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_submesh_t, IDEATEK_SUBMESH_FIELDS);

    #define IDEATEK_MESH_LOD_FIELDS { \
        float error; \
        uint32_t first; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mesh_lod_t, IDEATEK_MESH_LOD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mesh_lod_t, IDEATEK_MESH_LOD_FIELDS);

    #define IDEATEK_ALPHA_MODE_VALUES { \
        IDEATEK_ALPHA_MODE_OPAQUE = 0, \
        IDEATEK_ALPHA_MODE_MASK = 1, \
        IDEATEK_ALPHA_MODE_BLEND = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_alpha_mode_t,
    uint8_t, IDEATEK_ALPHA_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_alpha_mode_t,
    uint8_t, IDEATEK_ALPHA_MODE_VALUES);

    #define IDEATEK_DIMENSION_VALUES { \
        IDEATEK_DIMENSION_2D = 0, \
        IDEATEK_DIMENSION_2D_ARRAY = 1, \
        IDEATEK_DIMENSION_CUBE = 2, \
        IDEATEK_DIMENSION_3D = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_dimension_t,
    uint8_t, IDEATEK_DIMENSION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_dimension_t,
    uint8_t, IDEATEK_DIMENSION_VALUES);

    #define IDEATEK_FILTER_VALUES { \
        IDEATEK_FILTER_NEAREST = 0, \
        IDEATEK_FILTER_LINEAR = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_filter_t, uint8_t, IDEATEK_FILTER_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_filter_t, uint8_t, IDEATEK_FILTER_VALUES);

    #define IDEATEK_WRAP_VALUES { \
        IDEATEK_WRAP_REPEAT = 0, \
        IDEATEK_WRAP_MIRRORED_REPEAT = 1, \
        IDEATEK_WRAP_CLAMP_TO_EDGE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_wrap_t, uint8_t, IDEATEK_WRAP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_wrap_t, uint8_t, IDEATEK_WRAP_VALUES);

    #define IDEATEK_LIGHT_KIND_VALUES { \
        IDEATEK_LIGHT_KIND_DIRECTIONAL = 0, \
        IDEATEK_LIGHT_KIND_POINT = 1, \
        IDEATEK_LIGHT_KIND_SPOT = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_light_kind_t,
    uint8_t, IDEATEK_LIGHT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_light_kind_t,
    uint8_t, IDEATEK_LIGHT_KIND_VALUES);

    #define IDEATEK_CAST_SHADOW_VALUES { \
        IDEATEK_CAST_SHADOW_OFF = 0, \
        IDEATEK_CAST_SHADOW_ON = 1, \
        IDEATEK_CAST_SHADOW_ONLY = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_cast_shadow_t,
    uint8_t, IDEATEK_CAST_SHADOW_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_cast_shadow_t,
    uint8_t, IDEATEK_CAST_SHADOW_VALUES);

    #define IDEATEK_FOG_SHAPE_VALUES { \
        IDEATEK_FOG_SHAPE_INFINITE = 0, \
        IDEATEK_FOG_SHAPE_BOX = 1, \
        IDEATEK_FOG_SHAPE_ELLIPSOID = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_fog_shape_t,
    uint8_t, IDEATEK_FOG_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_fog_shape_t,
    uint8_t, IDEATEK_FOG_SHAPE_VALUES);

    #define IDEATEK_SKY_KIND_VALUES { \
        IDEATEK_SKY_KIND_ATMOSPHERE = 0, \
        IDEATEK_SKY_KIND_TEXTURE = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_sky_kind_t, uint8_t, IDEATEK_SKY_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_sky_kind_t, uint8_t, IDEATEK_SKY_KIND_VALUES);

    #define IDEATEK_TONEMAP_CURVE_VALUES { \
        IDEATEK_TONEMAP_CURVE_AGX = 0, \
        IDEATEK_TONEMAP_CURVE_NEUTRAL = 1, \
        IDEATEK_TONEMAP_CURVE_ACES = 2, \
        IDEATEK_TONEMAP_CURVE_LINEAR = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_tonemap_curve_t,
    uint8_t, IDEATEK_TONEMAP_CURVE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_tonemap_curve_t,
    uint8_t, IDEATEK_TONEMAP_CURVE_VALUES);

    #define IDEATEK_CAMERA_FIELDS { \
        ideatek_entity_t target; \
        uint64_t groups; \
        ideatek_rect_t viewport; \
        int32_t order; \
        bool active; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_camera_t, IDEATEK_CAMERA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_camera_t, IDEATEK_CAMERA_FIELDS);

    #define IDEATEK_PROJECTION_FIELDS { \
        float fov; \
        float height; \
        float near; \
        float far; \
        ideatek_projection_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_projection_t, IDEATEK_PROJECTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_projection_t, IDEATEK_PROJECTION_FIELDS);

    #define IDEATEK_PROJECTION_ANGLES_FIELDS { \
        float left; \
        float right; \
        float up; \
        float down; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_projection_angles_t,
    IDEATEK_PROJECTION_ANGLES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_projection_angles_t,
    IDEATEK_PROJECTION_ANGLES_FIELDS);

    #define IDEATEK_MESH_FIELDS { \
        bridge_text_t bytes; \
        uint32_t vertex_count; \
        uint32_t index_count; \
        uint32_t index_offset; \
        uint32_t stream_count; \
        uint32_t submesh_count; \
        uint32_t lod_count; \
        uint32_t target_count; \
        uint32_t revision; \
        ideatek_topology_t topology; \
        ideatek_index_format_t index_format; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mesh_t, IDEATEK_MESH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mesh_t, IDEATEK_MESH_FIELDS);

    #define IDEATEK_MESH_INSTANCE_FIELDS { \
        ideatek_entity_t mesh; \
        float lod_bias; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mesh_instance_t,
    IDEATEK_MESH_INSTANCE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mesh_instance_t,
    IDEATEK_MESH_INSTANCE_FIELDS);

    #define IDEATEK_MESH_MATERIALS_FIELDS { \
        bridge_text_t materials; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mesh_materials_t,
    IDEATEK_MESH_MATERIALS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mesh_materials_t,
    IDEATEK_MESH_MATERIALS_FIELDS);

    #define IDEATEK_VISIBILITY_FIELDS { \
        uint64_t groups; \
        ideatek_cast_shadow_t cast_shadow; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_visibility_t, IDEATEK_VISIBILITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_visibility_t, IDEATEK_VISIBILITY_FIELDS);

    #define IDEATEK_AFFECTS_FIELDS { \
        uint64_t groups; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_affects_t, IDEATEK_AFFECTS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_affects_t, IDEATEK_AFFECTS_FIELDS);

    #define IDEATEK_VISIBILITY_RANGE_FIELDS { \
        float begin; \
        float end; \
        float begin_margin; \
        float end_margin; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_visibility_range_t,
    IDEATEK_VISIBILITY_RANGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_visibility_range_t,
    IDEATEK_VISIBILITY_RANGE_FIELDS);

    #define IDEATEK_MATERIAL_FIELDS { \
        ideatek_entity_t base_color_texture; \
        ideatek_entity_t metallic_roughness_texture; \
        ideatek_entity_t normal_texture; \
        ideatek_entity_t occlusion_texture; \
        ideatek_entity_t emissive_texture; \
        ideatek_color_t base_color; \
        ideatek_rgb_t emissive; \
        float metallic; \
        float roughness; \
        float emissive_strength; \
        float normal_scale; \
        float occlusion_strength; \
        float alpha_cutoff; \
        ideatek_alpha_mode_t alpha_mode; \
        bool double_sided; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_material_t, IDEATEK_MATERIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_material_t, IDEATEK_MATERIAL_FIELDS);

    #define IDEATEK_TEXTURE_FIELDS { \
        bridge_text_t bytes; \
        uint32_t format; \
        uint32_t width; \
        uint32_t height; \
        uint32_t depth_or_layers; \
        uint32_t mip_count; \
        uint32_t revision; \
        ideatek_dimension_t dimension; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_texture_t, IDEATEK_TEXTURE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_texture_t, IDEATEK_TEXTURE_FIELDS);

    #define IDEATEK_SAMPLER_FIELDS { \
        float max_anisotropy; \
        ideatek_filter_t min_filter; \
        ideatek_filter_t mag_filter; \
        ideatek_filter_t mip_filter; \
        ideatek_wrap_t wrap_u; \
        ideatek_wrap_t wrap_v; \
        ideatek_wrap_t wrap_w; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sampler_t, IDEATEK_SAMPLER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sampler_t, IDEATEK_SAMPLER_FIELDS);

    #define IDEATEK_LIGHT_FIELDS { \
        ideatek_entity_t cookie; \
        ideatek_rgb_t color; \
        float intensity; \
        float range; \
        float source_radius; \
        float inner_cone; \
        float outer_cone; \
        ideatek_light_kind_t kind; \
        bool shadow; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_light_t, IDEATEK_LIGHT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_light_t, IDEATEK_LIGHT_FIELDS);

    #define IDEATEK_DECAL_FIELDS { \
        ideatek_entity_t material; \
        ideatek_vec3_t half_size; \
        float normal_fade; \
        int32_t sort_order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_decal_t, IDEATEK_DECAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_decal_t, IDEATEK_DECAL_FIELDS);

    #define IDEATEK_FOG_FIELDS { \
        ideatek_vec3_t size; \
        ideatek_rgb_t albedo; \
        ideatek_rgb_t emission; \
        float density; \
        float height_falloff; \
        float anisotropy; \
        float edge; \
        ideatek_fog_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_fog_t, IDEATEK_FOG_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_fog_t, IDEATEK_FOG_FIELDS);

    #define IDEATEK_SKY_FIELDS { \
        ideatek_entity_t texture; \
        float intensity; \
        int32_t priority; \
        ideatek_sky_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sky_t, IDEATEK_SKY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sky_t, IDEATEK_SKY_FIELDS);

    #define IDEATEK_ATMOSPHERE_FIELDS { \
        ideatek_rgb_t rayleigh; \
        ideatek_rgb_t ozone; \
        float bottom_radius; \
        float height; \
        float rayleigh_height; \
        float mie; \
        float mie_absorption; \
        float mie_height; \
        float mie_anisotropy; \
        float ground_albedo; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_atmosphere_t, IDEATEK_ATMOSPHERE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_atmosphere_t, IDEATEK_ATMOSPHERE_FIELDS);

    #define IDEATEK_CLOUD_LAYER_FIELDS { \
        ideatek_entity_t coverage_texture; \
        float bottom; \
        float thickness; \
        float coverage; \
        float density; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_cloud_layer_t, IDEATEK_CLOUD_LAYER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_cloud_layer_t, IDEATEK_CLOUD_LAYER_FIELDS);

    #define IDEATEK_EXPOSURE_FIELDS { \
        float ev100; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_exposure_t, IDEATEK_EXPOSURE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_exposure_t, IDEATEK_EXPOSURE_FIELDS);

    #define IDEATEK_AUTO_EXPOSURE_FIELDS { \
        float min_ev; \
        float max_ev; \
        float compensation; \
        float speed_up; \
        float speed_down; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_auto_exposure_t,
    IDEATEK_AUTO_EXPOSURE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_auto_exposure_t,
    IDEATEK_AUTO_EXPOSURE_FIELDS);

    #define IDEATEK_TONEMAP_FIELDS { \
        ideatek_tonemap_curve_t curve; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_tonemap_t, IDEATEK_TONEMAP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_tonemap_t, IDEATEK_TONEMAP_FIELDS);

    #define IDEATEK_BLOOM_FIELDS { \
        float intensity; \
        float threshold; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_bloom_t, IDEATEK_BLOOM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_bloom_t, IDEATEK_BLOOM_FIELDS);

    #define IDEATEK_COLOR_GRADE_FIELDS { \
        ideatek_entity_t lut; \
        float saturation; \
        float contrast; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_color_grade_t, IDEATEK_COLOR_GRADE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_color_grade_t, IDEATEK_COLOR_GRADE_FIELDS);

    #define IDEATEK_DEPTH_OF_FIELD_FIELDS { \
        float focus_distance; \
        float f_stop; \
        float sensor_width; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_depth_of_field_t,
    IDEATEK_DEPTH_OF_FIELD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_depth_of_field_t,
    IDEATEK_DEPTH_OF_FIELD_FIELDS);

    #define IDEATEK_MOTION_BLUR_FIELDS { \
        float shutter; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_motion_blur_t, IDEATEK_MOTION_BLUR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_motion_blur_t, IDEATEK_MOTION_BLUR_FIELDS);

    #define IDEATEK_VIGNETTE_FIELDS { \
        float intensity; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vignette_t, IDEATEK_VIGNETTE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vignette_t, IDEATEK_VIGNETTE_FIELDS);

    #define IDEATEK_CHROMATIC_ABERRATION_FIELDS { \
        float intensity; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_chromatic_aberration_t,
    IDEATEK_CHROMATIC_ABERRATION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_chromatic_aberration_t,
    IDEATEK_CHROMATIC_ABERRATION_FIELDS);

    #define IDEATEK_FILM_GRAIN_FIELDS { \
        float intensity; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_film_grain_t, IDEATEK_FILM_GRAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_film_grain_t, IDEATEK_FILM_GRAIN_FIELDS);

    #define IDEATEK_LENS_FLARE_FIELDS { \
        ideatek_entity_t bokeh; \
        ideatek_rgb_t tint; \
        float intensity; \
        float threshold; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lens_flare_t, IDEATEK_LENS_FLARE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lens_flare_t, IDEATEK_LENS_FLARE_FIELDS);

#endif
