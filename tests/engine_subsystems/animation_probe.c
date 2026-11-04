/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"
#include "ideatek_states.h"
#include "ideatek_animation.h"

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
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    ENUM(api, "ideatek:ideatek_channel_property_t", ideatek_channel_property_t);
    ENUM(api, "ideatek:ideatek_interpolation_t", ideatek_interpolation_t);
    ENUM(api, "ideatek:ideatek_layer_blend_t", ideatek_layer_blend_t);
    ENUM(api, "ideatek:ideatek_clip_event_kind_t", ideatek_clip_event_kind_t);
    ENUM(api, "ideatek:ideatek_parameter_type_t", ideatek_parameter_type_t);
    STRUCT(api, "ideatek:ideatek_clip_channel_t", ideatek_clip_channel_t);
    STRUCT(api, "ideatek:ideatek_clip_event_t", ideatek_clip_event_t);
    ENUM(api, "ideatek:ideatek_repeat_t", ideatek_repeat_t);
    ENUM(api, "ideatek:ideatek_state_switch_t", ideatek_state_switch_t);
    ENUM(api, "ideatek:ideatek_event_phase_t", ideatek_event_phase_t);
    MASK(api, "ideatek:ideatek_motion_axes_t", ideatek_motion_axes_t);
    ENUM(api, "ideatek:ideatek_axis_t", ideatek_axis_t);
    ENUM(api, "ideatek:ideatek_collider_shape_t", ideatek_collider_shape_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    STRUCT(api, "ideatek:Skin", ideatek_skin_t);
    STRUCT(api, "ideatek:SkinBinds", ideatek_skin_binds_t);
    STRUCT(api, "ideatek:MorphWeights", ideatek_morph_weights_t);
    STRUCT(api, "ideatek:Clip", ideatek_clip_t);
    STRUCT(api, "ideatek:AnimationTarget", ideatek_animation_target_t);
    TAG(api, "ideatek:AnimationPlayer");
    STRUCT(api, "ideatek:ClipPlay", ideatek_clip_play_t);
    STRUCT(api, "ideatek:BlendSpace", ideatek_blend_space_t);
    STRUCT(api, "ideatek:BlendPoint", ideatek_blend_point_t);
    STRUCT(api, "ideatek:AnimationStates", ideatek_animation_states_t);
    STRUCT(api, "ideatek:AnimationState", ideatek_animation_state_t);
    STRUCT(api, "ideatek:ClipEvent", ideatek_clip_event_record_t);
    STRUCT(api, "ideatek:RootMotion", ideatek_root_motion_t);
    STRUCT(api, "ideatek:RootMotionDelta", ideatek_root_motion_delta_t);
    STRUCT(api, "ideatek:PoseModifier", ideatek_pose_modifier_t);
    STRUCT(api, "ideatek:TwoBoneIK", ideatek_two_bone_ik_t);
    STRUCT(api, "ideatek:ChainIK", ideatek_chain_ik_t);
    STRUCT(api, "ideatek:LookAt", ideatek_look_at_t);
    STRUCT(api, "ideatek:CopyTransform", ideatek_copy_transform_t);
    STRUCT(api, "ideatek:SpringChain", ideatek_spring_chain_t);
    STRUCT(api, "ideatek:SpringCollider", ideatek_spring_collider_t);
    STRUCT(api, "ideatek:BoneMask", ideatek_bone_mask_t);
    STRUCT(api, "ideatek:AnimationCurves", ideatek_animation_curves_t);
    STRUCT(api, "ideatek:AnimationParameters", ideatek_animation_parameters_t);
    STRUCT(api, "ideatek:WarpTargets", ideatek_warp_targets_t);
    STRUCT(api, "ideatek:OrientationWarp", ideatek_orientation_warp_t);
    STRUCT(api, "ideatek:AnimationLod", ideatek_animation_lod_t);
    STRUCT(api, "ideatek:Trajectory", ideatek_trajectory_t);
    STRUCT(api, "ideatek:MotionMatch", ideatek_motion_match_t);
    STRUCT(api, "ideatek:Inertialize", ideatek_inertialize_t);
    STRUCT(api, "ideatek:PlayLink", ideatek_play_link_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    ENUM(api, "ideatek:ideatek_layer_blend_t", ideatek_layer_blend_t);
    ENUM(api, "ideatek:ideatek_repeat_t", ideatek_repeat_t);
    ENUM(api, "ideatek:ideatek_state_switch_t", ideatek_state_switch_t);
    MASK(api, "ideatek:ideatek_motion_axes_t", ideatek_motion_axes_t);
    ENUM(api, "ideatek:ideatek_axis_t", ideatek_axis_t);
    ENUM(api, "ideatek:ideatek_collider_shape_t", ideatek_collider_shape_t);
    api->attach(api, id, STRUCT(api, "ideatek:Joint", ideatek_joint_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Skin", ideatek_skin_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SkinBinds", ideatek_skin_binds_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Clip", ideatek_clip_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationTarget",
        ideatek_animation_target_t), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:AnimationPlayer"), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ClipPlay", ideatek_clip_play_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BlendSpace",
        ideatek_blend_space_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BlendPoint",
        ideatek_blend_point_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationStates",
        ideatek_animation_states_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationState",
        ideatek_animation_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RootMotion",
        ideatek_root_motion_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PoseModifier",
        ideatek_pose_modifier_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:TwoBoneIK",
        ideatek_two_bone_ik_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ChainIK", ideatek_chain_ik_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LookAt", ideatek_look_at_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CopyTransform",
        ideatek_copy_transform_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SpringChain",
        ideatek_spring_chain_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SpringCollider",
        ideatek_spring_collider_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PlayLink", ideatek_play_link_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BoneMask", ideatek_bone_mask_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationParameters",
        ideatek_animation_parameters_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:WarpTargets",
        ideatek_warp_targets_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:OrientationWarp",
        ideatek_orientation_warp_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationLod",
        ideatek_animation_lod_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Trajectory",
        ideatek_trajectory_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MotionMatch",
        ideatek_motion_match_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_animator(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_layer_blend_t", ideatek_layer_blend_t);
    ENUM(api, "ideatek:ideatek_repeat_t", ideatek_repeat_t);
    ENUM(api, "ideatek:ideatek_state_switch_t", ideatek_state_switch_t);
    ENUM(api, "ideatek:ideatek_event_phase_t", ideatek_event_phase_t);
    MASK(api, "ideatek:ideatek_motion_axes_t", ideatek_motion_axes_t);
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    STRUCT(api, "ideatek:Clip", ideatek_clip_t);
    STRUCT(api, "ideatek:AnimationTarget", ideatek_animation_target_t);
    TAG(api, "ideatek:AnimationPlayer");
    STRUCT(api, "ideatek:BlendSpace", ideatek_blend_space_t);
    STRUCT(api, "ideatek:BlendPoint", ideatek_blend_point_t);
    STRUCT(api, "ideatek:AnimationStates", ideatek_animation_states_t);
    STRUCT(api, "ideatek:AnimationState", ideatek_animation_state_t);
    STRUCT(api, "ideatek:RootMotion", ideatek_root_motion_t);
    STRUCT(api, "ideatek:PlayLink", ideatek_play_link_t);
    STRUCT(api, "ideatek:BoneMask", ideatek_bone_mask_t);
    STRUCT(api, "ideatek:WarpTargets", ideatek_warp_targets_t);
    STRUCT(api, "ideatek:OrientationWarp", ideatek_orientation_warp_t);
    STRUCT(api, "ideatek:AnimationLod", ideatek_animation_lod_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Frame", ideatek_frame_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:State", ideatek_state_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:StateChange", ideatek_state_change_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Transform",
        ideatek_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MorphWeights",
        ideatek_morph_weights_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RootMotionDelta",
        ideatek_root_motion_delta_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ClipEvent",
        ideatek_clip_event_record_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AnimationCurves",
        ideatek_animation_curves_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Inertialize",
        ideatek_inertialize_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ClipPlay", ideatek_clip_play_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_graph(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_layer_blend_t", ideatek_layer_blend_t);
    ENUM(api, "ideatek:ideatek_repeat_t", ideatek_repeat_t);
    ENUM(api, "ideatek:ideatek_event_phase_t", ideatek_event_phase_t);
    STRUCT(api, "ideatek:AnimationParameters", ideatek_animation_parameters_t);
    STRUCT(api, "ideatek:Clip", ideatek_clip_t);
    STRUCT(api, "ideatek:AnimationCurves", ideatek_animation_curves_t);
    STRUCT(api, "ideatek:ClipEvent", ideatek_clip_event_record_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Frame", ideatek_frame_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:BlendSpace",
        ideatek_blend_space_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Inertialize",
        ideatek_inertialize_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ClipPlay", ideatek_clip_play_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_matcher(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_layer_blend_t", ideatek_layer_blend_t);
    ENUM(api, "ideatek:ideatek_repeat_t", ideatek_repeat_t);
    STRUCT(api, "ideatek:MotionMatch", ideatek_motion_match_t);
    STRUCT(api, "ideatek:Trajectory", ideatek_trajectory_t);
    STRUCT(api, "ideatek:Clip", ideatek_clip_t);
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Frame", ideatek_frame_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:Inertialize",
        ideatek_inertialize_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ClipPlay", ideatek_clip_play_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_solver(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    MASK(api, "ideatek:ideatek_motion_axes_t", ideatek_motion_axes_t);
    ENUM(api, "ideatek:ideatek_axis_t", ideatek_axis_t);
    ENUM(api, "ideatek:ideatek_collider_shape_t", ideatek_collider_shape_t);
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    STRUCT(api, "ideatek:PoseModifier", ideatek_pose_modifier_t);
    STRUCT(api, "ideatek:TwoBoneIK", ideatek_two_bone_ik_t);
    STRUCT(api, "ideatek:ChainIK", ideatek_chain_ik_t);
    STRUCT(api, "ideatek:LookAt", ideatek_look_at_t);
    STRUCT(api, "ideatek:CopyTransform", ideatek_copy_transform_t);
    STRUCT(api, "ideatek:SpringChain", ideatek_spring_chain_t);
    STRUCT(api, "ideatek:SpringCollider", ideatek_spring_collider_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Frame", ideatek_frame_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Transform",
        ideatek_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_renderer(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:Joint", ideatek_joint_t);
    STRUCT(api, "ideatek:Skin", ideatek_skin_t);
    STRUCT(api, "ideatek:SkinBinds", ideatek_skin_binds_t);
    STRUCT(api, "ideatek:MorphWeights", ideatek_morph_weights_t);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_ANIMATOR)
    role_animator(api);
#elif defined(ROLE_GRAPH)
    role_graph(api);
#elif defined(ROLE_MATCHER)
    role_matcher(api);
#elif defined(ROLE_SOLVER)
    role_solver(api);
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
