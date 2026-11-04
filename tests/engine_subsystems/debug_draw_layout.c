/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_debug_draw.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_debug_kind(void)
{
    SIZE(ideatek_debug_kind_t);
}

static void show_debug_flags(void)
{
    SIZE(ideatek_debug_flags_t);
}

static void show_debug_draw(void)
{
    SIZE(ideatek_debug_draw_t);
    AT(ideatek_debug_draw_t, groups);
    AT(ideatek_debug_draw_t, data);
}

static void show_debug_view(void)
{
    SIZE(ideatek_debug_view_t);
    AT(ideatek_debug_view_t, groups);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_debug_kind_t_desc, ideatek_debug_flags_t_desc,
        ideatek_debug_draw_t_desc, ideatek_debug_view_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_DEBUG_KIND_VALUES), TEXT_OF(IDEATEK_DEBUG_FLAGS_VALUES),
        TEXT_OF(IDEATEK_DEBUG_DRAW_FIELDS), TEXT_OF(IDEATEK_DEBUG_VIEW_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "debug_kind", show_debug_kind }, { "debug_flags", show_debug_flags },
        { "debug_draw", show_debug_draw }, { "debug_view", show_debug_view },
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
