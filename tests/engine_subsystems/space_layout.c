/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_space.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_vec3(void)
{
    SIZE(ideatek_vec3_t);
    AT(ideatek_vec3_t, x);
    AT(ideatek_vec3_t, y);
    AT(ideatek_vec3_t, z);
}

static void show_quat(void)
{
    SIZE(ideatek_quat_t);
    AT(ideatek_quat_t, x);
    AT(ideatek_quat_t, y);
    AT(ideatek_quat_t, z);
    AT(ideatek_quat_t, w);
}

static void show_position(void)
{
    SIZE(ideatek_position_t);
    AT(ideatek_position_t, x);
    AT(ideatek_position_t, y);
    AT(ideatek_position_t, z);
}

static void show_keep(void)
{
    SIZE(ideatek_keep_t);
}

static void show_orphan(void)
{
    SIZE(ideatek_orphan_t);
}

static void show_absolute(void)
{
    printf("position %d\nrotation %d\nscale %d\n", IDEATEK_ABSOLUTE_POSITION,
        IDEATEK_ABSOLUTE_ROTATION, IDEATEK_ABSOLUTE_SCALE);
}

static void show_transform(void)
{
    SIZE(ideatek_transform_t);
    AT(ideatek_transform_t, position);
    AT(ideatek_transform_t, rotation);
    AT(ideatek_transform_t, scale);
}

static void show_parent(void)
{
    SIZE(ideatek_parent_t);
    AT(ideatek_parent_t, entity);
    AT(ideatek_parent_t, order);
    AT(ideatek_parent_t, keep);
    AT(ideatek_parent_t, orphan);
    AT(ideatek_parent_t, absolute);
}

static void show_world_transform(void)
{
    SIZE(ideatek_world_transform_t);
    AT(ideatek_world_transform_t, origin);
    AT(ideatek_world_transform_t, basis);
    AT(ideatek_world_transform_t, revision);
}

static void show_children(void)
{
    SIZE(ideatek_children_t);
    AT(ideatek_children_t, first);
    AT(ideatek_children_t, last);
    AT(ideatek_children_t, count);
}

static void show_sibling(void)
{
    SIZE(ideatek_sibling_t);
    AT(ideatek_sibling_t, prev);
    AT(ideatek_sibling_t, next);
    AT(ideatek_sibling_t, parent);
}

static void show_previous(void)
{
    SIZE(ideatek_previous_world_transform_t);
    AT(ideatek_previous_world_transform_t, world);
    AT(ideatek_previous_world_transform_t, tick);
}

static void show_moves(void)
{
    SIZE(ideatek_world_moves_t);
    AT(ideatek_world_moves_t, moves);
    AT(ideatek_world_moves_t, total);
    AT(ideatek_world_moves_t, capacity);
}

static void show_descriptors(void)
{
    const char *descriptors[] = { ideatek_vec3_t_desc, ideatek_quat_t_desc,
        ideatek_position_t_desc, ideatek_keep_t_desc,
        ideatek_orphan_t_desc, ideatek_transform_t_desc,
        ideatek_parent_t_desc, ideatek_world_transform_t_desc,
        ideatek_children_t_desc, ideatek_sibling_t_desc,
        ideatek_previous_world_transform_t_desc, ideatek_world_moves_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = { TEXT_OF(IDEATEK_VEC3_FIELDS),
        TEXT_OF(IDEATEK_QUAT_FIELDS), TEXT_OF(IDEATEK_POSITION_FIELDS),
        TEXT_OF(IDEATEK_KEEP_VALUES),
        TEXT_OF(IDEATEK_ORPHAN_VALUES), TEXT_OF(IDEATEK_TRANSFORM_FIELDS),
        TEXT_OF(IDEATEK_PARENT_FIELDS),
        TEXT_OF(IDEATEK_WORLD_TRANSFORM_FIELDS),
        TEXT_OF(IDEATEK_CHILDREN_FIELDS), TEXT_OF(IDEATEK_SIBLING_FIELDS),
        TEXT_OF(IDEATEK_PREVIOUS_WORLD_TRANSFORM_FIELDS),
        TEXT_OF(IDEATEK_WORLD_MOVES_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "vec3", show_vec3 }, { "quat", show_quat },
        { "position", show_position },
        { "keep", show_keep }, { "orphan", show_orphan },
        { "absolute", show_absolute }, { "transform", show_transform },
        { "parent", show_parent },
        { "world_transform", show_world_transform },
        { "children", show_children }, { "sibling", show_sibling },
        { "previous", show_previous }, { "moves", show_moves },
        { "descriptors", show_descriptors },
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
