/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_navigation.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_nav_mesh_kind(void)
{
    SIZE(ideatek_nav_mesh_kind_t);
}

static void show_nav_mesh(void)
{
    SIZE(ideatek_nav_mesh_t);
    AT(ideatek_nav_mesh_t, data);
    AT(ideatek_nav_mesh_t, sources);
    AT(ideatek_nav_mesh_t, size);
    AT(ideatek_nav_mesh_t, agent_radius);
    AT(ideatek_nav_mesh_t, agent_height);
    AT(ideatek_nav_mesh_t, max_climb);
    AT(ideatek_nav_mesh_t, max_slope);
    AT(ideatek_nav_mesh_t, cell_size);
    AT(ideatek_nav_mesh_t, cell_height);
    AT(ideatek_nav_mesh_t, tile_size);
    AT(ideatek_nav_mesh_t, kind);
}

static void show_nav_build(void)
{
    SIZE(ideatek_nav_build_t);
}

static void show_nav_mesh_state(void)
{
    SIZE(ideatek_nav_mesh_state_t);
    AT(ideatek_nav_mesh_state_t, tiles);
    AT(ideatek_nav_mesh_state_t, revision);
    AT(ideatek_nav_mesh_state_t, state);
}

static void show_nav_area_flags(void)
{
    SIZE(ideatek_nav_area_flags_t);
}

static void show_nav_area(void)
{
    SIZE(ideatek_nav_area_t);
    AT(ideatek_nav_area_t, groups);
    AT(ideatek_nav_area_t, enter_cost);
    AT(ideatek_nav_area_t, travel_cost);
    AT(ideatek_nav_area_t, flags);
}

static void show_nav_modifier(void)
{
    SIZE(ideatek_nav_modifier_t);
    AT(ideatek_nav_modifier_t, area);
    AT(ideatek_nav_modifier_t, shape);
    AT(ideatek_nav_modifier_t, size);
}

static void show_nav_link_flags(void)
{
    SIZE(ideatek_nav_link_flags_t);
}

static void show_nav_link(void)
{
    SIZE(ideatek_nav_link_t);
    AT(ideatek_nav_link_t, area);
    AT(ideatek_nav_link_t, start);
    AT(ideatek_nav_link_t, end);
    AT(ideatek_nav_link_t, radius);
    AT(ideatek_nav_link_t, flags);
}

static void show_nav_query_kind(void)
{
    SIZE(ideatek_nav_query_kind_t);
}

static void show_nav_query_flags(void)
{
    SIZE(ideatek_nav_query_flags_t);
}

static void show_nav_query(void)
{
    SIZE(ideatek_nav_query_t);
    AT(ideatek_nav_query_t, nav_mesh);
    AT(ideatek_nav_query_t, filter);
    AT(ideatek_nav_query_t, start);
    AT(ideatek_nav_query_t, end);
    AT(ideatek_nav_query_t, groups);
    AT(ideatek_nav_query_t, radius);
    AT(ideatek_nav_query_t, kind);
    AT(ideatek_nav_query_t, flags);
}

static void show_nav_result(void)
{
    SIZE(ideatek_nav_result_t);
}

static void show_nav_point(void)
{
    SIZE(ideatek_nav_point_t);
    AT(ideatek_nav_point_t, position);
    AT(ideatek_nav_point_t, link);
}

static void show_nav_path(void)
{
    SIZE(ideatek_nav_path_t);
    AT(ideatek_nav_path_t, bytes);
    AT(ideatek_nav_path_t, count);
    AT(ideatek_nav_path_t, length);
    AT(ideatek_nav_path_t, revision);
    AT(ideatek_nav_path_t, state);
}

static void show_nav_agent_flags(void)
{
    SIZE(ideatek_nav_agent_flags_t);
}

static void show_nav_agent(void)
{
    SIZE(ideatek_nav_agent_t);
    AT(ideatek_nav_agent_t, nav_mesh);
    AT(ideatek_nav_agent_t, goal);
    AT(ideatek_nav_agent_t, filter);
    AT(ideatek_nav_agent_t, target);
    AT(ideatek_nav_agent_t, groups);
    AT(ideatek_nav_agent_t, avoid_groups);
    AT(ideatek_nav_agent_t, avoid_mask);
    AT(ideatek_nav_agent_t, max_speed);
    AT(ideatek_nav_agent_t, max_acceleration);
    AT(ideatek_nav_agent_t, avoidance_priority);
    AT(ideatek_nav_agent_t, radius);
    AT(ideatek_nav_agent_t, arrive_distance);
    AT(ideatek_nav_agent_t, flags);
}

static void show_nav_steer_state(void)
{
    SIZE(ideatek_nav_steer_state_t);
}

static void show_nav_steer(void)
{
    SIZE(ideatek_nav_steer_t);
    AT(ideatek_nav_steer_t, link);
    AT(ideatek_nav_steer_t, next);
    AT(ideatek_nav_steer_t, velocity);
    AT(ideatek_nav_steer_t, state);
}

static void show_nav_link_reached(void)
{
    SIZE(ideatek_nav_link_reached_t);
    AT(ideatek_nav_link_reached_t, agent);
    AT(ideatek_nav_link_reached_t, link);
    AT(ideatek_nav_link_reached_t, order);
}

static void show_nav_obstacle(void)
{
    SIZE(ideatek_nav_obstacle_t);
    AT(ideatek_nav_obstacle_t, avoid_groups);
    AT(ideatek_nav_obstacle_t, radius);
    AT(ideatek_nav_obstacle_t, height);
}

static void show_nav_mesh_data(void)
{
    SIZE(ideatek_nav_mesh_data_t);
    AT(ideatek_nav_mesh_data_t, bytes);
    AT(ideatek_nav_mesh_data_t, revision);
}

static void show_nav_goal(void)
{
    SIZE(ideatek_nav_goal_t);
    AT(ideatek_nav_goal_t, position);
}

static void show_nav_filter(void)
{
    SIZE(ideatek_nav_filter_t);
    AT(ideatek_nav_filter_t, costs);
    AT(ideatek_nav_filter_t, count);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_nav_mesh_kind_t_desc, ideatek_nav_mesh_t_desc,
        ideatek_nav_build_t_desc, ideatek_nav_mesh_state_t_desc,
        ideatek_nav_area_flags_t_desc, ideatek_nav_area_t_desc,
        ideatek_nav_modifier_t_desc, ideatek_nav_link_flags_t_desc,
        ideatek_nav_link_t_desc, ideatek_nav_query_kind_t_desc,
        ideatek_nav_query_flags_t_desc, ideatek_nav_query_t_desc,
        ideatek_nav_result_t_desc, ideatek_nav_point_t_desc,
        ideatek_nav_path_t_desc, ideatek_nav_agent_flags_t_desc,
        ideatek_nav_agent_t_desc, ideatek_nav_steer_state_t_desc,
        ideatek_nav_steer_t_desc, ideatek_nav_link_reached_t_desc,
        ideatek_nav_obstacle_t_desc, ideatek_nav_mesh_data_t_desc,
        ideatek_nav_goal_t_desc, ideatek_nav_filter_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_NAV_MESH_KIND_VALUES), TEXT_OF(IDEATEK_NAV_MESH_FIELDS),
        TEXT_OF(IDEATEK_NAV_BUILD_VALUES),
        TEXT_OF(IDEATEK_NAV_MESH_STATE_FIELDS),
        TEXT_OF(IDEATEK_NAV_AREA_FLAGS_VALUES),
        TEXT_OF(IDEATEK_NAV_AREA_FIELDS), TEXT_OF(IDEATEK_NAV_MODIFIER_FIELDS),
        TEXT_OF(IDEATEK_NAV_LINK_FLAGS_VALUES),
        TEXT_OF(IDEATEK_NAV_LINK_FIELDS),
        TEXT_OF(IDEATEK_NAV_QUERY_KIND_VALUES),
        TEXT_OF(IDEATEK_NAV_QUERY_FLAGS_VALUES),
        TEXT_OF(IDEATEK_NAV_QUERY_FIELDS), TEXT_OF(IDEATEK_NAV_RESULT_VALUES),
        TEXT_OF(IDEATEK_NAV_POINT_FIELDS), TEXT_OF(IDEATEK_NAV_PATH_FIELDS),
        TEXT_OF(IDEATEK_NAV_AGENT_FLAGS_VALUES),
        TEXT_OF(IDEATEK_NAV_AGENT_FIELDS),
        TEXT_OF(IDEATEK_NAV_STEER_STATE_VALUES),
        TEXT_OF(IDEATEK_NAV_STEER_FIELDS),
        TEXT_OF(IDEATEK_NAV_LINK_REACHED_FIELDS),
        TEXT_OF(IDEATEK_NAV_OBSTACLE_FIELDS),
        TEXT_OF(IDEATEK_NAV_MESH_DATA_FIELDS), TEXT_OF(IDEATEK_NAV_GOAL_FIELDS),
        TEXT_OF(IDEATEK_NAV_FILTER_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "nav_mesh_kind", show_nav_mesh_kind }, { "nav_mesh", show_nav_mesh },
        { "nav_build", show_nav_build },
        { "nav_mesh_state", show_nav_mesh_state },
        { "nav_area_flags", show_nav_area_flags },
        { "nav_area", show_nav_area }, { "nav_modifier", show_nav_modifier },
        { "nav_link_flags", show_nav_link_flags },
        { "nav_link", show_nav_link },
        { "nav_query_kind", show_nav_query_kind },
        { "nav_query_flags", show_nav_query_flags },
        { "nav_query", show_nav_query }, { "nav_result", show_nav_result },
        { "nav_point", show_nav_point }, { "nav_path", show_nav_path },
        { "nav_agent_flags", show_nav_agent_flags },
        { "nav_agent", show_nav_agent },
        { "nav_steer_state", show_nav_steer_state },
        { "nav_steer", show_nav_steer },
        { "nav_link_reached", show_nav_link_reached },
        { "nav_obstacle", show_nav_obstacle },
        { "nav_mesh_data", show_nav_mesh_data }, { "nav_goal", show_nav_goal },
        { "nav_filter", show_nav_filter }, { "descriptors", show_descriptors },
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
