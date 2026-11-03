/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_world.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_entity(void)
{
    SIZE(ideatek_entity_t);
    AT(ideatek_entity_t, id);
}

static void show_descriptors(void)
{
    printf("%s\n", ideatek_entity_t_desc);
}

static void show_texts(void)
{
    printf("%s\n", TEXT_OF(IDEATEK_ENTITY_FIELDS));
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "entity", show_entity }, { "descriptors", show_descriptors },
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
