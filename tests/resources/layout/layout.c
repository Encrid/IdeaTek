/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_resources.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_state(void)
{
    SIZE(ideatek_load_state_t);
}

static void show_error(void)
{
    SIZE(ideatek_resource_error_t);
}

static void show_load(void)
{
    SIZE(ideatek_resource_load_t);
    AT(ideatek_resource_load_t, owner);
    AT(ideatek_resource_load_t, path);
    AT(ideatek_resource_load_t, type);
    AT(ideatek_resource_load_t, settings);
    AT(ideatek_resource_load_t, reload_owner);
}

static void show_handle(void)
{
    SIZE(ideatek_resource_handle_t);
    AT(ideatek_resource_handle_t, resource);
}

static void show_resource(void)
{
    SIZE(ideatek_resource_t);
    AT(ideatek_resource_t, path);
    AT(ideatek_resource_t, type);
    AT(ideatek_resource_t, state);
    AT(ideatek_resource_t, dependencies);
    AT(ideatek_resource_t, recursive);
    AT(ideatek_resource_t, error);
}

static void show_loader(void)
{
    SIZE(ideatek_resource_loader_t);
    AT(ideatek_resource_loader_t, name);
    AT(ideatek_resource_loader_t, type);
    AT(ideatek_resource_loader_t, extensions);
}

static void show_bytes(void)
{
    SIZE(ideatek_resource_bytes_t);
    AT(ideatek_resource_bytes_t, loader);
    AT(ideatek_resource_bytes_t, data);
    AT(ideatek_resource_bytes_t, settings);
}

static void show_outcome(void)
{
    SIZE(ideatek_resource_outcome_t);
    AT(ideatek_resource_outcome_t, error);
}

static void show_label(void)
{
    SIZE(ideatek_resource_label_t);
    AT(ideatek_resource_label_t, resource);
    AT(ideatek_resource_label_t, label);
}

static void show_descriptors(void)
{
    static const char *DESCRIPTORS[] = {
        ideatek_load_state_t_desc, ideatek_resource_error_t_desc,
        ideatek_resource_load_t_desc, ideatek_resource_handle_t_desc,
        ideatek_resource_t_desc, ideatek_resource_loader_t_desc,
        ideatek_resource_bytes_t_desc, ideatek_resource_outcome_t_desc,
        ideatek_resource_label_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *TEXTS[] = {
        TEXT_OF(IDEATEK_LOAD_STATE_VALUES),
        TEXT_OF(IDEATEK_RESOURCE_ERROR_VALUES),
        TEXT_OF(IDEATEK_RESOURCE_LOAD_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_HANDLE_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_LOADER_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_BYTES_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_OUTCOME_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_LABEL_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "state", show_state }, { "error", show_error },
        { "load", show_load }, { "handle", show_handle },
        { "resource", show_resource }, { "loader", show_loader },
        { "bytes", show_bytes }, { "outcome", show_outcome },
        { "label", show_label }, { "descriptors", show_descriptors },
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
