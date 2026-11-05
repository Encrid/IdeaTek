/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_localization.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_locale(void)
{
    SIZE(ideatek_locale_t);
    AT(ideatek_locale_t, code);
    AT(ideatek_locale_t, fallback);
}

static void show_text_syntax(void)
{
    SIZE(ideatek_text_syntax_t);
}

static void show_string_table(void)
{
    SIZE(ideatek_string_table_t);
    AT(ideatek_string_table_t, bytes);
    AT(ideatek_string_table_t, locale);
    AT(ideatek_string_table_t, domain);
    AT(ideatek_string_table_t, count);
    AT(ideatek_string_table_t, revision);
    AT(ideatek_string_table_t, syntax);
}

static void show_text_arg(void)
{
    SIZE(ideatek_text_arg_t);
}

static void show_text_key(void)
{
    SIZE(ideatek_text_key_t);
    AT(ideatek_text_key_t, args);
    AT(ideatek_text_key_t, key);
    AT(ideatek_text_key_t, source_hash);
}

static void show_localized_text(void)
{
    SIZE(ideatek_localized_text_t);
    AT(ideatek_localized_text_t, text);
    AT(ideatek_localized_text_t, locale);
    AT(ideatek_localized_text_t, missing);
    AT(ideatek_localized_text_t, stale);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_locale_t_desc, ideatek_text_syntax_t_desc,
        ideatek_string_table_t_desc, ideatek_text_arg_t_desc,
        ideatek_text_key_t_desc, ideatek_localized_text_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_LOCALE_FIELDS), TEXT_OF(IDEATEK_TEXT_SYNTAX_VALUES),
        TEXT_OF(IDEATEK_STRING_TABLE_FIELDS), TEXT_OF(IDEATEK_TEXT_ARG_VALUES),
        TEXT_OF(IDEATEK_TEXT_KEY_FIELDS), TEXT_OF(IDEATEK_LOCALIZED_TEXT_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "locale", show_locale }, { "text_syntax", show_text_syntax },
        { "string_table", show_string_table }, { "text_arg", show_text_arg },
        { "text_key", show_text_key },
        { "localized_text", show_localized_text },
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
