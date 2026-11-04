/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_fields.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_field_shape(void)
{
    SIZE(ideatek_field_shape_t);
}

static void show_force_kind(void)
{
    SIZE(ideatek_force_kind_t);
}

static void show_wind(void)
{
    SIZE(ideatek_wind_t);
    AT(ideatek_wind_t, size);
    AT(ideatek_wind_t, speed);
    AT(ideatek_wind_t, falloff);
    AT(ideatek_wind_t, shape);
}

static void show_force_field(void)
{
    SIZE(ideatek_force_field_t);
    AT(ideatek_force_field_t, size);
    AT(ideatek_force_field_t, acceleration);
    AT(ideatek_force_field_t, falloff);
    AT(ideatek_force_field_t, kind);
    AT(ideatek_force_field_t, shape);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_field_shape_t_desc, ideatek_force_kind_t_desc,
        ideatek_wind_t_desc, ideatek_force_field_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_FIELD_SHAPE_VALUES), TEXT_OF(IDEATEK_FORCE_KIND_VALUES),
        TEXT_OF(IDEATEK_WIND_FIELDS), TEXT_OF(IDEATEK_FORCE_FIELD_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "field_shape", show_field_shape }, { "force_kind", show_force_kind },
        { "wind", show_wind }, { "force_field", show_force_field },
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
