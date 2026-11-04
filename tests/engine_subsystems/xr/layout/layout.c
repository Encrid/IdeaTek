/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_xr.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_xr_space(void)
{
    SIZE(ideatek_xr_space_t);
}

static void show_xr_blend(void)
{
    SIZE(ideatek_xr_blend_t);
}

static void show_xr_origin(void)
{
    SIZE(ideatek_xr_origin_t);
    AT(ideatek_xr_origin_t, scale);
    AT(ideatek_xr_origin_t, space);
    AT(ideatek_xr_origin_t, blend);
}

static void show_xr_session_state(void)
{
    SIZE(ideatek_xr_session_state_t);
}

static void show_xr_blends(void)
{
    SIZE(ideatek_xr_blends_t);
}

static void show_xr_session(void)
{
    SIZE(ideatek_xr_session_t);
    AT(ideatek_xr_session_t, refresh);
    AT(ideatek_xr_session_t, state);
    AT(ideatek_xr_session_t, blend);
    AT(ideatek_xr_session_t, blends);
}

static void show_xr_tracker_kind(void)
{
    SIZE(ideatek_xr_tracker_kind_t);
}

static void show_xr_hand(void)
{
    SIZE(ideatek_xr_hand_t);
}

static void show_xr_pose(void)
{
    SIZE(ideatek_xr_pose_t);
}

static void show_xr_tracker(void)
{
    SIZE(ideatek_xr_tracker_t);
    AT(ideatek_xr_tracker_t, kind);
    AT(ideatek_xr_tracker_t, hand);
    AT(ideatek_xr_tracker_t, pose);
}

static void show_xr_location(void)
{
    SIZE(ideatek_xr_location_t);
}

static void show_xr_pose_state(void)
{
    SIZE(ideatek_xr_pose_state_t);
    AT(ideatek_xr_pose_state_t, linear);
    AT(ideatek_xr_pose_state_t, angular);
    AT(ideatek_xr_pose_state_t, flags);
}

static void show_xr_binding_path(void)
{
    SIZE(ideatek_xr_binding_path_t);
    AT(ideatek_xr_binding_path_t, profile);
    AT(ideatek_xr_binding_path_t, path);
}

static void show_xr_control_state(void)
{
    SIZE(ideatek_xr_control_state_t);
    AT(ideatek_xr_control_state_t, x);
    AT(ideatek_xr_control_state_t, y);
    AT(ideatek_xr_control_state_t, active);
}

static void show_xr_hand_record(void)
{
    SIZE(ideatek_xr_hand_record_t);
    AT(ideatek_xr_hand_record_t, hand);
    AT(ideatek_xr_hand_record_t, active);
}

static void show_xr_joint_state(void)
{
    SIZE(ideatek_xr_joint_state_t);
    AT(ideatek_xr_joint_state_t, radius);
    AT(ideatek_xr_joint_state_t, flags);
}

static void show_xr_haptic(void)
{
    SIZE(ideatek_xr_haptic_t);
    AT(ideatek_xr_haptic_t, amplitude);
    AT(ideatek_xr_haptic_t, frequency);
    AT(ideatek_xr_haptic_t, duration_ms);
}

static void show_xr_play_area(void)
{
    SIZE(ideatek_xr_play_area_t);
    AT(ideatek_xr_play_area_t, width);
    AT(ideatek_xr_play_area_t, depth);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_xr_space_t_desc, ideatek_xr_blend_t_desc,
        ideatek_xr_origin_t_desc, ideatek_xr_session_state_t_desc,
        ideatek_xr_blends_t_desc, ideatek_xr_session_t_desc,
        ideatek_xr_tracker_kind_t_desc, ideatek_xr_hand_t_desc,
        ideatek_xr_pose_t_desc, ideatek_xr_tracker_t_desc,
        ideatek_xr_location_t_desc, ideatek_xr_pose_state_t_desc,
        ideatek_xr_binding_path_t_desc, ideatek_xr_control_state_t_desc,
        ideatek_xr_hand_record_t_desc, ideatek_xr_joint_state_t_desc,
        ideatek_xr_haptic_t_desc, ideatek_xr_play_area_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_XR_SPACE_VALUES), TEXT_OF(IDEATEK_XR_BLEND_VALUES),
        TEXT_OF(IDEATEK_XR_ORIGIN_FIELDS),
        TEXT_OF(IDEATEK_XR_SESSION_STATE_VALUES),
        TEXT_OF(IDEATEK_XR_BLENDS_VALUES), TEXT_OF(IDEATEK_XR_SESSION_FIELDS),
        TEXT_OF(IDEATEK_XR_TRACKER_KIND_VALUES),
        TEXT_OF(IDEATEK_XR_HAND_VALUES), TEXT_OF(IDEATEK_XR_POSE_VALUES),
        TEXT_OF(IDEATEK_XR_TRACKER_FIELDS), TEXT_OF(IDEATEK_XR_LOCATION_VALUES),
        TEXT_OF(IDEATEK_XR_POSE_STATE_FIELDS),
        TEXT_OF(IDEATEK_XR_BINDING_PATH_FIELDS),
        TEXT_OF(IDEATEK_XR_CONTROL_STATE_FIELDS),
        TEXT_OF(IDEATEK_XR_HAND_RECORD_FIELDS),
        TEXT_OF(IDEATEK_XR_JOINT_STATE_FIELDS),
        TEXT_OF(IDEATEK_XR_HAPTIC_FIELDS), TEXT_OF(IDEATEK_XR_PLAY_AREA_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "xr_space", show_xr_space }, { "xr_blend", show_xr_blend },
        { "xr_origin", show_xr_origin },
        { "xr_session_state", show_xr_session_state },
        { "xr_blends", show_xr_blends }, { "xr_session", show_xr_session },
        { "xr_tracker_kind", show_xr_tracker_kind },
        { "xr_hand", show_xr_hand }, { "xr_pose", show_xr_pose },
        { "xr_tracker", show_xr_tracker }, { "xr_location", show_xr_location },
        { "xr_pose_state", show_xr_pose_state },
        { "xr_binding_path", show_xr_binding_path },
        { "xr_control_state", show_xr_control_state },
        { "xr_hand_record", show_xr_hand_record },
        { "xr_joint_state", show_xr_joint_state },
        { "xr_haptic", show_xr_haptic }, { "xr_play_area", show_xr_play_area },
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
