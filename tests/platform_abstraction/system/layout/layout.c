/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_system.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_system_state(void)
{
    SIZE(ideatek_system_state_t);
    AT(ideatek_system_state_t, locales);
}

static void show_url_request(void)
{
    SIZE(ideatek_url_request_t);
    AT(ideatek_url_request_t, url);
}

static void show_message_level(void)
{
    SIZE(ideatek_message_level_t);
}

static void show_message_request(void)
{
    SIZE(ideatek_message_request_t);
    AT(ideatek_message_request_t, title);
    AT(ideatek_message_request_t, message);
    AT(ideatek_message_request_t, level);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_system_state_t_desc, ideatek_url_request_t_desc,
        ideatek_message_level_t_desc, ideatek_message_request_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_SYSTEM_STATE_FIELDS),
        TEXT_OF(IDEATEK_URL_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_MESSAGE_LEVEL_VALUES),
        TEXT_OF(IDEATEK_MESSAGE_REQUEST_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "system_state", show_system_state },
        { "url_request", show_url_request },
        { "message_level", show_message_level },
        { "message_request", show_message_request },
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
