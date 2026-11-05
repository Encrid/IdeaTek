/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_changes.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_entity_id(void)
{
    SIZE(ideatek_entity_id_t);
    AT(ideatek_entity_id_t, id);
}

static void show_change_kind(void)
{
    SIZE(ideatek_change_kind_t);
}

static void show_durability(void)
{
    SIZE(ideatek_durability_t);
}

static void show_change_condition(void)
{
    SIZE(ideatek_change_condition_t);
}

static void show_change_tag(void)
{
    SIZE(ideatek_change_tag_t);
    AT(ideatek_change_tag_t, tag);
    AT(ideatek_change_tag_t, period);
    AT(ideatek_change_tag_t, version);
    AT(ideatek_change_tag_t, durability);
    AT(ideatek_change_tag_t, condition);
}

static void show_row_changes(void)
{
    SIZE(ideatek_row_changes_t);
    AT(ideatek_row_changes_t, tag);
    AT(ideatek_row_changes_t, data);
    AT(ideatek_row_changes_t, request);
    AT(ideatek_row_changes_t, peer);
    AT(ideatek_row_changes_t, kind);
}

static void show_rows_in(void)
{
    SIZE(ideatek_rows_in_t);
    AT(ideatek_rows_in_t, tag);
    AT(ideatek_rows_in_t, data);
    AT(ideatek_rows_in_t, source);
    AT(ideatek_rows_in_t, peer);
    AT(ideatek_rows_in_t, version);
    AT(ideatek_rows_in_t, kind);
}

static void show_store_state(void)
{
    SIZE(ideatek_store_state_t);
    AT(ideatek_store_state_t, round);
    AT(ideatek_store_state_t, durable);
}

static void show_recording_action(void)
{
    SIZE(ideatek_recording_action_t);
}

static void show_recording(void)
{
    SIZE(ideatek_recording_t);
    AT(ideatek_recording_t, file);
    AT(ideatek_recording_t, action);
}

static void show_recording_status(void)
{
    SIZE(ideatek_recording_status_t);
}

static void show_recording_state(void)
{
    SIZE(ideatek_recording_state_t);
    AT(ideatek_recording_state_t, round);
    AT(ideatek_recording_state_t, status);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_entity_id_t_desc, ideatek_change_kind_t_desc,
        ideatek_durability_t_desc, ideatek_change_condition_t_desc,
        ideatek_change_tag_t_desc, ideatek_row_changes_t_desc,
        ideatek_rows_in_t_desc, ideatek_store_state_t_desc,
        ideatek_recording_action_t_desc, ideatek_recording_t_desc,
        ideatek_recording_status_t_desc, ideatek_recording_state_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_ENTITY_ID_FIELDS), TEXT_OF(IDEATEK_CHANGE_KIND_VALUES),
        TEXT_OF(IDEATEK_DURABILITY_VALUES),
        TEXT_OF(IDEATEK_CHANGE_CONDITION_VALUES),
        TEXT_OF(IDEATEK_CHANGE_TAG_FIELDS), TEXT_OF(IDEATEK_ROW_CHANGES_FIELDS),
        TEXT_OF(IDEATEK_ROWS_IN_FIELDS), TEXT_OF(IDEATEK_STORE_STATE_FIELDS),
        TEXT_OF(IDEATEK_RECORDING_ACTION_VALUES),
        TEXT_OF(IDEATEK_RECORDING_FIELDS),
        TEXT_OF(IDEATEK_RECORDING_STATUS_VALUES),
        TEXT_OF(IDEATEK_RECORDING_STATE_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "entity_id", show_entity_id }, { "change_kind", show_change_kind },
        { "durability", show_durability },
        { "change_condition", show_change_condition },
        { "change_tag", show_change_tag }, { "row_changes", show_row_changes },
        { "rows_in", show_rows_in }, { "store_state", show_store_state },
        { "recording_action", show_recording_action },
        { "recording", show_recording },
        { "recording_status", show_recording_status },
        { "recording_state", show_recording_state },
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
