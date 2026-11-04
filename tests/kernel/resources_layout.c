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

static void show_load_state(void)
{
    SIZE(ideatek_load_state_t);
}

static void show_resource_error(void)
{
    SIZE(ideatek_resource_error_t);
}

static void show_resource_load(void)
{
    SIZE(ideatek_resource_load_t);
    AT(ideatek_resource_load_t, owner);
    AT(ideatek_resource_load_t, path);
    AT(ideatek_resource_load_t, type);
    AT(ideatek_resource_load_t, settings);
    AT(ideatek_resource_load_t, reload_owner);
    AT(ideatek_resource_load_t, priority);
}

static void show_resource_add(void)
{
    SIZE(ideatek_resource_add_t);
    AT(ideatek_resource_add_t, resource);
    AT(ideatek_resource_add_t, type);
}

static void show_resource_handle(void)
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

static void show_resource_loader(void)
{
    SIZE(ideatek_resource_loader_t);
    AT(ideatek_resource_loader_t, name);
    AT(ideatek_resource_loader_t, type);
    AT(ideatek_resource_loader_t, extensions);
    AT(ideatek_resource_loader_t, magic);
    AT(ideatek_resource_loader_t, ranged);
}

static void show_resource_bytes(void)
{
    SIZE(ideatek_resource_bytes_t);
    AT(ideatek_resource_bytes_t, loader);
    AT(ideatek_resource_bytes_t, offset);
    AT(ideatek_resource_bytes_t, size);
    AT(ideatek_resource_bytes_t, data);
    AT(ideatek_resource_bytes_t, settings);
    AT(ideatek_resource_bytes_t, error);
}

static void show_resource_read(void)
{
    SIZE(ideatek_resource_read_t);
    AT(ideatek_resource_read_t, offset);
    AT(ideatek_resource_read_t, length);
    AT(ideatek_resource_read_t, priority);
}

static void show_resource_outcome(void)
{
    SIZE(ideatek_resource_outcome_t);
    AT(ideatek_resource_outcome_t, error);
}

static void show_resource_label(void)
{
    SIZE(ideatek_resource_label_t);
    AT(ideatek_resource_label_t, resource);
    AT(ideatek_resource_label_t, label);
}

static void show_raw_bytes(void)
{
    SIZE(ideatek_raw_bytes_t);
    AT(ideatek_raw_bytes_t, data);
}

static void show_resource_source(void)
{
    SIZE(ideatek_resource_source_t);
    AT(ideatek_resource_source_t, name);
}

static void show_resource_source_read(void)
{
    SIZE(ideatek_resource_source_read_t);
    AT(ideatek_resource_source_read_t, source);
    AT(ideatek_resource_source_read_t, offset);
    AT(ideatek_resource_source_read_t, length);
    AT(ideatek_resource_source_read_t, path);
    AT(ideatek_resource_source_read_t, priority);
}

static void show_resource_source_result(void)
{
    SIZE(ideatek_resource_source_result_t);
    AT(ideatek_resource_source_result_t, size);
    AT(ideatek_resource_source_result_t, data);
    AT(ideatek_resource_source_result_t, error);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_load_state_t_desc, ideatek_resource_error_t_desc,
        ideatek_resource_load_t_desc, ideatek_resource_add_t_desc,
        ideatek_resource_handle_t_desc, ideatek_resource_t_desc,
        ideatek_resource_loader_t_desc, ideatek_resource_bytes_t_desc,
        ideatek_resource_read_t_desc, ideatek_resource_outcome_t_desc,
        ideatek_resource_label_t_desc, ideatek_raw_bytes_t_desc,
        ideatek_resource_source_t_desc, ideatek_resource_source_read_t_desc,
        ideatek_resource_source_result_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_LOAD_STATE_VALUES),
        TEXT_OF(IDEATEK_RESOURCE_ERROR_VALUES),
        TEXT_OF(IDEATEK_RESOURCE_LOAD_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_ADD_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_HANDLE_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_LOADER_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_BYTES_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_READ_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_OUTCOME_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_LABEL_FIELDS),
        TEXT_OF(IDEATEK_RAW_BYTES_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_SOURCE_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_SOURCE_READ_FIELDS),
        TEXT_OF(IDEATEK_RESOURCE_SOURCE_RESULT_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "load_state", show_load_state },
        { "resource_error", show_resource_error },
        { "resource_load", show_resource_load },
        { "resource_add", show_resource_add },
        { "resource_handle", show_resource_handle },
        { "resource", show_resource },
        { "resource_loader", show_resource_loader },
        { "resource_bytes", show_resource_bytes },
        { "resource_read", show_resource_read },
        { "resource_outcome", show_resource_outcome },
        { "resource_label", show_resource_label },
        { "raw_bytes", show_raw_bytes },
        { "resource_source", show_resource_source },
        { "resource_source_read", show_resource_source_read },
        { "resource_source_result", show_resource_source_result },
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
