/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_accessibility.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_theme(void)
{
    SIZE(ideatek_theme_t);
}

static void show_state(void)
{
    SIZE(ideatek_accessibility_state_t);
    AT(ideatek_accessibility_state_t, theme);
    AT(ideatek_accessibility_state_t, increase_contrast);
    AT(ideatek_accessibility_state_t, reduce_animation);
    AT(ideatek_accessibility_state_t, reduce_transparency);
    AT(ideatek_accessibility_state_t, screen_reader);
    AT(ideatek_accessibility_state_t, speech);
}

static void show_speech_request(void)
{
    SIZE(ideatek_speech_request_t);
    AT(ideatek_speech_request_t, text);
    AT(ideatek_speech_request_t, interrupt);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_theme_t_desc, ideatek_accessibility_state_t_desc,
        ideatek_speech_request_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_THEME_VALUES),
        TEXT_OF(IDEATEK_ACCESSIBILITY_STATE_FIELDS),
        TEXT_OF(IDEATEK_SPEECH_REQUEST_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "theme", show_theme }, { "state", show_state },
        { "speech_request", show_speech_request },
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
