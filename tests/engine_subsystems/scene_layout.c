/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_scene.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_node_place(void)
{
    SIZE(ideatek_model_node_t);
    AT(ideatek_model_node_t, position);
    AT(ideatek_model_node_t, rotation);
    AT(ideatek_model_node_t, scale);
    AT(ideatek_model_node_t, mesh);
    AT(ideatek_model_node_t, binds);
    AT(ideatek_model_node_t, parent);
    AT(ideatek_model_node_t, name_offset);
    AT(ideatek_model_node_t, name_length);
}

static void show_node(void)
{
    show_node_place();
    AT(ideatek_model_node_t, material_first);
    AT(ideatek_model_node_t, material_count);
    AT(ideatek_model_node_t, weight_first);
    AT(ideatek_model_node_t, weight_count);
    AT(ideatek_model_node_t, light);
    AT(ideatek_model_node_t, camera);
    AT(ideatek_model_node_t, joint);
}

static void show_model(void)
{
    SIZE(ideatek_model_t);
    AT(ideatek_model_t, bytes);
    AT(ideatek_model_t, node_count);
    AT(ideatek_model_t, light_count);
    AT(ideatek_model_t, material_count);
    AT(ideatek_model_t, weight_count);
    AT(ideatek_model_t, camera_count);
    AT(ideatek_model_t, joint_count);
    AT(ideatek_model_t, revision);
}

static void show_instance(void)
{
    SIZE(ideatek_model_instance_t);
    AT(ideatek_model_instance_t, model);
}

static void show_node_ref(void)
{
    SIZE(ideatek_model_node_ref_t);
    AT(ideatek_model_node_ref_t, instance);
    AT(ideatek_model_node_ref_t, node);
}

static void show_none(void)
{
    printf("none %u\n", (unsigned)IDEATEK_MODEL_NONE);
}

static void show_descriptors(void)
{
    const char *descriptors[] = { ideatek_model_node_t_desc,
        ideatek_model_t_desc, ideatek_model_instance_t_desc,
        ideatek_model_node_ref_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = { TEXT_OF(IDEATEK_MODEL_NODE_FIELDS),
        TEXT_OF(IDEATEK_MODEL_FIELDS), TEXT_OF(IDEATEK_MODEL_INSTANCE_FIELDS),
        TEXT_OF(IDEATEK_MODEL_NODE_REF_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "node", show_node }, { "model", show_model },
        { "instance", show_instance }, { "node_ref", show_node_ref },
        { "none", show_none }, { "descriptors", show_descriptors },
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
