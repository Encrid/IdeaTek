/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_states.h>

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
    SIZE(ideatek_state_t);
    AT(ideatek_state_t, value);
}

static void show_request(void)
{
    SIZE(ideatek_state_request_t);
    AT(ideatek_state_request_t, value);
}

static void show_change(void)
{
    SIZE(ideatek_state_change_t);
    AT(ideatek_state_change_t, from);
    AT(ideatek_state_change_t, to);
}

static void show_scoped(void)
{
    SIZE(ideatek_state_scoped_t);
    AT(ideatek_state_scoped_t, machine);
    AT(ideatek_state_scoped_t, value);
}

static void show_descriptors(void)
{
    const char *descriptors[] = { ideatek_state_t_desc,
        ideatek_state_request_t_desc, ideatek_state_change_t_desc,
        ideatek_state_scoped_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = { TEXT_OF(IDEATEK_STATE_FIELDS),
        TEXT_OF(IDEATEK_STATE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_STATE_CHANGE_FIELDS),
        TEXT_OF(IDEATEK_STATE_SCOPED_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "state", show_state }, { "request", show_request },
        { "change", show_change }, { "scoped", show_scoped },
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
