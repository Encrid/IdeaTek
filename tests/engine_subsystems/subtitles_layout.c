/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_subtitles.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_caption_kind(void)
{
    SIZE(ideatek_caption_kind_t);
}

static void show_sound_caption(void)
{
    SIZE(ideatek_sound_caption_t);
    AT(ideatek_sound_caption_t, sound);
    AT(ideatek_sound_caption_t, speaker);
    AT(ideatek_sound_caption_t, text);
    AT(ideatek_sound_caption_t, time);
    AT(ideatek_sound_caption_t, duration);
    AT(ideatek_sound_caption_t, priority);
    AT(ideatek_sound_caption_t, kind);
}

static void show_subtitle_line(void)
{
    SIZE(ideatek_subtitle_line_t);
    AT(ideatek_subtitle_line_t, caption);
    AT(ideatek_subtitle_line_t, player);
    AT(ideatek_subtitle_line_t, start);
    AT(ideatek_subtitle_line_t, end);
    AT(ideatek_subtitle_line_t, kind);
}

static void show_subtitle_flags(void)
{
    SIZE(ideatek_subtitle_flags_t);
}

static void show_subtitle_settings(void)
{
    SIZE(ideatek_subtitle_settings_t);
    AT(ideatek_subtitle_settings_t, flags);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_caption_kind_t_desc, ideatek_sound_caption_t_desc,
        ideatek_subtitle_line_t_desc, ideatek_subtitle_flags_t_desc,
        ideatek_subtitle_settings_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_CAPTION_KIND_VALUES),
        TEXT_OF(IDEATEK_SOUND_CAPTION_FIELDS),
        TEXT_OF(IDEATEK_SUBTITLE_LINE_FIELDS),
        TEXT_OF(IDEATEK_SUBTITLE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SUBTITLE_SETTINGS_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "caption_kind", show_caption_kind },
        { "sound_caption", show_sound_caption },
        { "subtitle_line", show_subtitle_line },
        { "subtitle_flags", show_subtitle_flags },
        { "subtitle_settings", show_subtitle_settings },
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
