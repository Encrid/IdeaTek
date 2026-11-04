/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_files.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_storage(void)
{
    SIZE(ideatek_storage_t);
}

static void show_action(void)
{
    SIZE(ideatek_file_action_t);
}

static void show_status(void)
{
    SIZE(ideatek_file_status_t);
}

static void show_request(void)
{
    SIZE(ideatek_file_request_t);
    AT(ideatek_file_request_t, offset);
    AT(ideatek_file_request_t, length);
    AT(ideatek_file_request_t, path);
    AT(ideatek_file_request_t, data);
    AT(ideatek_file_request_t, storage);
    AT(ideatek_file_request_t, action);
    AT(ideatek_file_request_t, priority);
}

static void show_result(void)
{
    SIZE(ideatek_file_result_t);
    AT(ideatek_file_result_t, size);
    AT(ideatek_file_result_t, data);
    AT(ideatek_file_result_t, status);
}

static void show_change(void)
{
    SIZE(ideatek_file_change_t);
    AT(ideatek_file_change_t, paths);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_storage_t_desc, ideatek_file_action_t_desc,
        ideatek_file_status_t_desc, ideatek_file_request_t_desc,
        ideatek_file_result_t_desc, ideatek_file_change_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_STORAGE_VALUES),
        TEXT_OF(IDEATEK_FILE_ACTION_VALUES),
        TEXT_OF(IDEATEK_FILE_STATUS_VALUES),
        TEXT_OF(IDEATEK_FILE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_FILE_RESULT_FIELDS),
        TEXT_OF(IDEATEK_FILE_CHANGE_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "storage", show_storage }, { "action", show_action },
        { "status", show_status }, { "request", show_request },
        { "result", show_result }, { "change", show_change },
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
