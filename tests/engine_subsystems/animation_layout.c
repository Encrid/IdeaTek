/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_animation.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_channel_property(void)
{
    SIZE(ideatek_channel_property_t);
}

static void show_interpolation(void)
{
    SIZE(ideatek_interpolation_t);
}

static void show_layer_blend(void)
{
    SIZE(ideatek_layer_blend_t);
}

static void show_clip_event_kind(void)
{
    SIZE(ideatek_clip_event_kind_t);
}

static void show_parameter_type(void)
{
    SIZE(ideatek_parameter_type_t);
}

static void show_clip_channel(void)
{
    SIZE(ideatek_clip_channel_t);
    AT(ideatek_clip_channel_t, target);
    AT(ideatek_clip_channel_t, key_count);
    AT(ideatek_clip_channel_t, times);
    AT(ideatek_clip_channel_t, values);
    AT(ideatek_clip_channel_t, property);
    AT(ideatek_clip_channel_t, interpolation);
}

static void show_clip_event(void)
{
    SIZE(ideatek_clip_event_t);
    AT(ideatek_clip_event_t, name);
    AT(ideatek_clip_event_t, time);
    AT(ideatek_clip_event_t, duration);
    AT(ideatek_clip_event_t, value);
    AT(ideatek_clip_event_t, kind);
}

static void show_repeat(void)
{
    SIZE(ideatek_repeat_t);
}

static void show_state_switch(void)
{
    SIZE(ideatek_state_switch_t);
}

static void show_event_phase(void)
{
    SIZE(ideatek_event_phase_t);
}

static void show_motion_axes(void)
{
    SIZE(ideatek_motion_axes_t);
}

static void show_axis(void)
{
    SIZE(ideatek_axis_t);
}

static void show_collider_shape(void)
{
    SIZE(ideatek_collider_shape_t);
}

static void show_joint(void)
{
    SIZE(ideatek_joint_t);
    AT(ideatek_joint_t, skeleton);
    AT(ideatek_joint_t, index);
}

static void show_skin(void)
{
    SIZE(ideatek_skin_t);
    AT(ideatek_skin_t, skeleton);
    AT(ideatek_skin_t, binds);
}

static void show_skin_binds(void)
{
    SIZE(ideatek_skin_binds_t);
    AT(ideatek_skin_binds_t, matrices);
    AT(ideatek_skin_binds_t, count);
    AT(ideatek_skin_binds_t, revision);
}

static void show_morph_weights(void)
{
    SIZE(ideatek_morph_weights_t);
    AT(ideatek_morph_weights_t, weights);
    AT(ideatek_morph_weights_t, count);
}

static void show_hidden_joint(void)
{
    SIZE(ideatek_hidden_joint_t);
    AT(ideatek_hidden_joint_t, cap);
}

static void show_clip(void)
{
    SIZE(ideatek_clip_t);
    AT(ideatek_clip_t, bytes);
    AT(ideatek_clip_t, duration);
    AT(ideatek_clip_t, channel_count);
    AT(ideatek_clip_t, event_count);
    AT(ideatek_clip_t, revision);
    AT(ideatek_clip_t, additive);
}

static void show_animation_target(void)
{
    SIZE(ideatek_animation_target_t);
    AT(ideatek_animation_target_t, id);
    AT(ideatek_animation_target_t, player);
}

static void show_clip_play(void)
{
    SIZE(ideatek_clip_play_t);
    AT(ideatek_clip_play_t, player);
    AT(ideatek_clip_play_t, clip);
    AT(ideatek_clip_play_t, mask);
    AT(ideatek_clip_play_t, sync_group);
    AT(ideatek_clip_play_t, time);
    AT(ideatek_clip_play_t, speed);
    AT(ideatek_clip_play_t, weight);
    AT(ideatek_clip_play_t, event_weight);
    AT(ideatek_clip_play_t, layer);
    AT(ideatek_clip_play_t, repeat);
    AT(ideatek_clip_play_t, paused);
    AT(ideatek_clip_play_t, real_time);
    AT(ideatek_clip_play_t, blend);
}

static void show_blend_space(void)
{
    SIZE(ideatek_blend_space_t);
    AT(ideatek_blend_space_t, player);
    AT(ideatek_blend_space_t, x);
    AT(ideatek_blend_space_t, y);
    AT(ideatek_blend_space_t, dimensions);
    AT(ideatek_blend_space_t, synced);
}

static void show_blend_point(void)
{
    SIZE(ideatek_blend_point_t);
    AT(ideatek_blend_point_t, space);
    AT(ideatek_blend_point_t, clip);
    AT(ideatek_blend_point_t, x);
    AT(ideatek_blend_point_t, y);
}

static void show_animation_states(void)
{
    SIZE(ideatek_animation_states_t);
    AT(ideatek_animation_states_t, player);
}

static void show_animation_state(void)
{
    SIZE(ideatek_animation_state_t);
    AT(ideatek_animation_state_t, machine);
    AT(ideatek_animation_state_t, source);
    AT(ideatek_animation_state_t, value);
    AT(ideatek_animation_state_t, fade);
    AT(ideatek_animation_state_t, when);
    AT(ideatek_animation_state_t, inertial);
}

static void show_clip_event_record(void)
{
    SIZE(ideatek_clip_event_record_t);
    AT(ideatek_clip_event_record_t, play);
    AT(ideatek_clip_event_record_t, player);
    AT(ideatek_clip_event_record_t, name);
    AT(ideatek_clip_event_record_t, value);
    AT(ideatek_clip_event_record_t, order);
    AT(ideatek_clip_event_record_t, phase);
}

static void show_root_motion(void)
{
    SIZE(ideatek_root_motion_t);
    AT(ideatek_root_motion_t, root);
    AT(ideatek_root_motion_t, axes);
}

static void show_root_motion_delta(void)
{
    SIZE(ideatek_root_motion_delta_t);
    AT(ideatek_root_motion_delta_t, rotation);
    AT(ideatek_root_motion_delta_t, translation);
}

static void show_pose_modifier(void)
{
    SIZE(ideatek_pose_modifier_t);
    AT(ideatek_pose_modifier_t, player);
    AT(ideatek_pose_modifier_t, influence);
    AT(ideatek_pose_modifier_t, order);
    AT(ideatek_pose_modifier_t, active);
}

static void show_two_bone_ik(void)
{
    SIZE(ideatek_two_bone_ik_t);
    AT(ideatek_two_bone_ik_t, root);
    AT(ideatek_two_bone_ik_t, end);
    AT(ideatek_two_bone_ik_t, target);
    AT(ideatek_two_bone_ik_t, pole);
}

static void show_chain_ik(void)
{
    SIZE(ideatek_chain_ik_t);
    AT(ideatek_chain_ik_t, root);
    AT(ideatek_chain_ik_t, end);
    AT(ideatek_chain_ik_t, target);
    AT(ideatek_chain_ik_t, iterations);
}

static void show_look_at(void)
{
    SIZE(ideatek_look_at_t);
    AT(ideatek_look_at_t, joint);
    AT(ideatek_look_at_t, target);
    AT(ideatek_look_at_t, max_angle);
    AT(ideatek_look_at_t, forward);
}

static void show_copy_transform(void)
{
    SIZE(ideatek_copy_transform_t);
    AT(ideatek_copy_transform_t, joint);
    AT(ideatek_copy_transform_t, source);
    AT(ideatek_copy_transform_t, axes);
}

static void show_spring_chain(void)
{
    SIZE(ideatek_spring_chain_t);
    AT(ideatek_spring_chain_t, root);
    AT(ideatek_spring_chain_t, end);
    AT(ideatek_spring_chain_t, center);
    AT(ideatek_spring_chain_t, gravity_direction);
    AT(ideatek_spring_chain_t, stiffness);
    AT(ideatek_spring_chain_t, drag);
    AT(ideatek_spring_chain_t, gravity);
    AT(ideatek_spring_chain_t, radius);
}

static void show_spring_collider(void)
{
    SIZE(ideatek_spring_collider_t);
    AT(ideatek_spring_collider_t, modifier);
    AT(ideatek_spring_collider_t, radius);
    AT(ideatek_spring_collider_t, height);
    AT(ideatek_spring_collider_t, shape);
}

static void show_bone_mask(void)
{
    SIZE(ideatek_bone_mask_t);
    AT(ideatek_bone_mask_t, weights);
    AT(ideatek_bone_mask_t, count);
    AT(ideatek_bone_mask_t, revision);
}

static void show_animation_curves(void)
{
    SIZE(ideatek_animation_curves_t);
    AT(ideatek_animation_curves_t, values);
    AT(ideatek_animation_curves_t, count);
}

static void show_animation_parameters(void)
{
    SIZE(ideatek_animation_parameters_t);
    AT(ideatek_animation_parameters_t, values);
    AT(ideatek_animation_parameters_t, count);
}

static void show_warp_targets(void)
{
    SIZE(ideatek_warp_targets_t);
    AT(ideatek_warp_targets_t, targets);
    AT(ideatek_warp_targets_t, count);
}

static void show_orientation_warp(void)
{
    SIZE(ideatek_orientation_warp_t);
    AT(ideatek_orientation_warp_t, direction);
    AT(ideatek_orientation_warp_t, max_angle);
}

static void show_animation_lod(void)
{
    SIZE(ideatek_animation_lod_t);
    AT(ideatek_animation_lod_t, joint_count);
    AT(ideatek_animation_lod_t, interval);
}

static void show_trajectory(void)
{
    SIZE(ideatek_trajectory_t);
    AT(ideatek_trajectory_t, samples);
    AT(ideatek_trajectory_t, count);
}

static void show_motion_match(void)
{
    SIZE(ideatek_motion_match_t);
    AT(ideatek_motion_match_t, player);
    AT(ideatek_motion_match_t, database);
    AT(ideatek_motion_match_t, blend_time);
    AT(ideatek_motion_match_t, search_interval);
    AT(ideatek_motion_match_t, inertial);
}

static void show_inertialize(void)
{
    SIZE(ideatek_inertialize_t);
    AT(ideatek_inertialize_t, duration);
}

static void show_play_link(void)
{
    SIZE(ideatek_play_link_t);
    AT(ideatek_play_link_t, parent);
    AT(ideatek_play_link_t, start);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_channel_property_t_desc, ideatek_interpolation_t_desc,
        ideatek_layer_blend_t_desc, ideatek_clip_event_kind_t_desc,
        ideatek_parameter_type_t_desc, ideatek_clip_channel_t_desc,
        ideatek_clip_event_t_desc, ideatek_repeat_t_desc,
        ideatek_state_switch_t_desc, ideatek_event_phase_t_desc,
        ideatek_motion_axes_t_desc, ideatek_axis_t_desc,
        ideatek_collider_shape_t_desc, ideatek_joint_t_desc,
        ideatek_skin_t_desc, ideatek_skin_binds_t_desc,
        ideatek_morph_weights_t_desc, ideatek_hidden_joint_t_desc,
        ideatek_clip_t_desc, ideatek_animation_target_t_desc,
        ideatek_clip_play_t_desc, ideatek_blend_space_t_desc,
        ideatek_blend_point_t_desc, ideatek_animation_states_t_desc,
        ideatek_animation_state_t_desc, ideatek_clip_event_record_t_desc,
        ideatek_root_motion_t_desc, ideatek_root_motion_delta_t_desc,
        ideatek_pose_modifier_t_desc, ideatek_two_bone_ik_t_desc,
        ideatek_chain_ik_t_desc, ideatek_look_at_t_desc,
        ideatek_copy_transform_t_desc, ideatek_spring_chain_t_desc,
        ideatek_spring_collider_t_desc, ideatek_bone_mask_t_desc,
        ideatek_animation_curves_t_desc, ideatek_animation_parameters_t_desc,
        ideatek_warp_targets_t_desc, ideatek_orientation_warp_t_desc,
        ideatek_animation_lod_t_desc, ideatek_trajectory_t_desc,
        ideatek_motion_match_t_desc, ideatek_inertialize_t_desc,
        ideatek_play_link_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_CHANNEL_PROPERTY_VALUES),
        TEXT_OF(IDEATEK_INTERPOLATION_VALUES),
        TEXT_OF(IDEATEK_LAYER_BLEND_VALUES),
        TEXT_OF(IDEATEK_CLIP_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_PARAMETER_TYPE_VALUES),
        TEXT_OF(IDEATEK_CLIP_CHANNEL_FIELDS),
        TEXT_OF(IDEATEK_CLIP_EVENT_FIELDS), TEXT_OF(IDEATEK_REPEAT_VALUES),
        TEXT_OF(IDEATEK_STATE_SWITCH_VALUES),
        TEXT_OF(IDEATEK_EVENT_PHASE_VALUES),
        TEXT_OF(IDEATEK_MOTION_AXES_VALUES), TEXT_OF(IDEATEK_AXIS_VALUES),
        TEXT_OF(IDEATEK_COLLIDER_SHAPE_VALUES), TEXT_OF(IDEATEK_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SKIN_FIELDS), TEXT_OF(IDEATEK_SKIN_BINDS_FIELDS),
        TEXT_OF(IDEATEK_MORPH_WEIGHTS_FIELDS),
        TEXT_OF(IDEATEK_HIDDEN_JOINT_FIELDS), TEXT_OF(IDEATEK_CLIP_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_TARGET_FIELDS),
        TEXT_OF(IDEATEK_CLIP_PLAY_FIELDS), TEXT_OF(IDEATEK_BLEND_SPACE_FIELDS),
        TEXT_OF(IDEATEK_BLEND_POINT_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_STATES_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_STATE_FIELDS),
        TEXT_OF(IDEATEK_CLIP_EVENT_RECORD_FIELDS),
        TEXT_OF(IDEATEK_ROOT_MOTION_FIELDS),
        TEXT_OF(IDEATEK_ROOT_MOTION_DELTA_FIELDS),
        TEXT_OF(IDEATEK_POSE_MODIFIER_FIELDS),
        TEXT_OF(IDEATEK_TWO_BONE_IK_FIELDS), TEXT_OF(IDEATEK_CHAIN_IK_FIELDS),
        TEXT_OF(IDEATEK_LOOK_AT_FIELDS), TEXT_OF(IDEATEK_COPY_TRANSFORM_FIELDS),
        TEXT_OF(IDEATEK_SPRING_CHAIN_FIELDS),
        TEXT_OF(IDEATEK_SPRING_COLLIDER_FIELDS),
        TEXT_OF(IDEATEK_BONE_MASK_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_CURVES_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_PARAMETERS_FIELDS),
        TEXT_OF(IDEATEK_WARP_TARGETS_FIELDS),
        TEXT_OF(IDEATEK_ORIENTATION_WARP_FIELDS),
        TEXT_OF(IDEATEK_ANIMATION_LOD_FIELDS),
        TEXT_OF(IDEATEK_TRAJECTORY_FIELDS),
        TEXT_OF(IDEATEK_MOTION_MATCH_FIELDS),
        TEXT_OF(IDEATEK_INERTIALIZE_FIELDS), TEXT_OF(IDEATEK_PLAY_LINK_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "channel_property", show_channel_property },
        { "interpolation", show_interpolation },
        { "layer_blend", show_layer_blend },
        { "clip_event_kind", show_clip_event_kind },
        { "parameter_type", show_parameter_type },
        { "clip_channel", show_clip_channel },
        { "clip_event", show_clip_event }, { "repeat", show_repeat },
        { "state_switch", show_state_switch },
        { "event_phase", show_event_phase },
        { "motion_axes", show_motion_axes }, { "axis", show_axis },
        { "collider_shape", show_collider_shape }, { "joint", show_joint },
        { "skin", show_skin }, { "skin_binds", show_skin_binds },
        { "morph_weights", show_morph_weights },
        { "hidden_joint", show_hidden_joint }, { "clip", show_clip },
        { "animation_target", show_animation_target },
        { "clip_play", show_clip_play }, { "blend_space", show_blend_space },
        { "blend_point", show_blend_point },
        { "animation_states", show_animation_states },
        { "animation_state", show_animation_state },
        { "clip_event_record", show_clip_event_record },
        { "root_motion", show_root_motion },
        { "root_motion_delta", show_root_motion_delta },
        { "pose_modifier", show_pose_modifier },
        { "two_bone_ik", show_two_bone_ik }, { "chain_ik", show_chain_ik },
        { "look_at", show_look_at }, { "copy_transform", show_copy_transform },
        { "spring_chain", show_spring_chain },
        { "spring_collider", show_spring_collider },
        { "bone_mask", show_bone_mask },
        { "animation_curves", show_animation_curves },
        { "animation_parameters", show_animation_parameters },
        { "warp_targets", show_warp_targets },
        { "orientation_warp", show_orientation_warp },
        { "animation_lod", show_animation_lod },
        { "trajectory", show_trajectory },
        { "motion_match", show_motion_match },
        { "inertialize", show_inertialize }, { "play_link", show_play_link },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
