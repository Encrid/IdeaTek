/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_persistence.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_save_action(void)
{
    SIZE(ideatek_save_action_t);
}

static void show_save_request(void)
{
    SIZE(ideatek_save_request_t);
    AT(ideatek_save_request_t, groups);
    AT(ideatek_save_request_t, slot);
    AT(ideatek_save_request_t, info);
    AT(ideatek_save_request_t, action);
}

static void show_saved(void)
{
    SIZE(ideatek_saved_t);
    AT(ideatek_saved_t, prefab);
    AT(ideatek_saved_t, groups);
}

static void show_save_ready(void)
{
    SIZE(ideatek_save_ready_t);
    AT(ideatek_save_ready_t, request);
    AT(ideatek_save_ready_t, tag);
}

static void show_save_refuse(void)
{
    SIZE(ideatek_save_refuse_t);
    AT(ideatek_save_refuse_t, request);
    AT(ideatek_save_refuse_t, tag);
    AT(ideatek_save_refuse_t, reason);
}

static void show_save_state(void)
{
    SIZE(ideatek_save_state_t);
}

static void show_save_error(void)
{
    SIZE(ideatek_save_error_t);
}

static void show_save_result(void)
{
    SIZE(ideatek_save_result_t);
    AT(ideatek_save_result_t, state);
    AT(ideatek_save_result_t, error);
}

static void show_save_info(void)
{
    SIZE(ideatek_save_info_t);
    AT(ideatek_save_info_t, request);
    AT(ideatek_save_info_t, time);
    AT(ideatek_save_info_t, bytes);
    AT(ideatek_save_info_t, groups);
    AT(ideatek_save_info_t, slot);
    AT(ideatek_save_info_t, info);
    AT(ideatek_save_info_t, version);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_save_action_t_desc, ideatek_save_request_t_desc,
        ideatek_saved_t_desc, ideatek_save_ready_t_desc,
        ideatek_save_refuse_t_desc, ideatek_save_state_t_desc,
        ideatek_save_error_t_desc, ideatek_save_result_t_desc,
        ideatek_save_info_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_SAVE_ACTION_VALUES),
        TEXT_OF(IDEATEK_SAVE_REQUEST_FIELDS), TEXT_OF(IDEATEK_SAVED_FIELDS),
        TEXT_OF(IDEATEK_SAVE_READY_FIELDS), TEXT_OF(IDEATEK_SAVE_REFUSE_FIELDS),
        TEXT_OF(IDEATEK_SAVE_STATE_VALUES), TEXT_OF(IDEATEK_SAVE_ERROR_VALUES),
        TEXT_OF(IDEATEK_SAVE_RESULT_FIELDS), TEXT_OF(IDEATEK_SAVE_INFO_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "save_action", show_save_action },
        { "save_request", show_save_request }, { "saved", show_saved },
        { "save_ready", show_save_ready }, { "save_refuse", show_save_refuse },
        { "save_state", show_save_state }, { "save_error", show_save_error },
        { "save_result", show_save_result }, { "save_info", show_save_info },
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
