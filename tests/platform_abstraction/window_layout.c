/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_window_clipboard.h>
#include <ideatek_window.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_window_mode(void)
{
    SIZE(ideatek_window_mode_t);
}

static void show_display_target(void)
{
    SIZE(ideatek_display_target_t);
}

static void show_surface(void)
{
    SIZE(ideatek_surface_t);
}

static void show_drop_kind(void)
{
    SIZE(ideatek_drop_kind_t);
}

static void show_request(void)
{
    SIZE(ideatek_window_request_t);
    AT(ideatek_window_request_t, title);
    AT(ideatek_window_request_t, display);
    AT(ideatek_window_request_t, video_mode);
    AT(ideatek_window_request_t, width);
    AT(ideatek_window_request_t, height);
    AT(ideatek_window_request_t, mode);
    AT(ideatek_window_request_t, target);
}

static void show_state(void)
{
    SIZE(ideatek_window_state_t);
    AT(ideatek_window_state_t, native_display);
    AT(ideatek_window_state_t, native_window);
    AT(ideatek_window_state_t, display);
    AT(ideatek_window_state_t, video_mode);
    AT(ideatek_window_state_t, width);
    AT(ideatek_window_state_t, height);
    AT(ideatek_window_state_t, pixel_width);
    AT(ideatek_window_state_t, pixel_height);
    AT(ideatek_window_state_t, display_scale);
    AT(ideatek_window_state_t, sdr_white_level);
    AT(ideatek_window_state_t, hdr_headroom);
    AT(ideatek_window_state_t, drop_x);
    AT(ideatek_window_state_t, drop_y);
    AT(ideatek_window_state_t, mode);
    AT(ideatek_window_state_t, surface);
    AT(ideatek_window_state_t, focused);
    AT(ideatek_window_state_t, minimized);
    AT(ideatek_window_state_t, hdr);
    AT(ideatek_window_state_t, close_asked);
    AT(ideatek_window_state_t, drop_over);
}

static void show_display(void)
{
    SIZE(ideatek_display_t);
    AT(ideatek_display_t, name);
    AT(ideatek_display_t, desktop_mode);
    AT(ideatek_display_t, current_mode);
    AT(ideatek_display_t, width);
    AT(ideatek_display_t, height);
    AT(ideatek_display_t, content_scale);
    AT(ideatek_display_t, hdr);
    AT(ideatek_display_t, is_primary);
}

static void show_video_mode(void)
{
    SIZE(ideatek_video_mode_t);
    AT(ideatek_video_mode_t, display);
    AT(ideatek_video_mode_t, width);
    AT(ideatek_video_mode_t, height);
    AT(ideatek_video_mode_t, pixel_density);
    AT(ideatek_video_mode_t, refresh_numerator);
    AT(ideatek_video_mode_t, refresh_denominator);
    AT(ideatek_video_mode_t, order);
}

static void show_drop(void)
{
    SIZE(ideatek_drop_t);
    AT(ideatek_drop_t, window);
    AT(ideatek_drop_t, data);
    AT(ideatek_drop_t, source);
    AT(ideatek_drop_t, set);
    AT(ideatek_drop_t, index);
    AT(ideatek_drop_t, x);
    AT(ideatek_drop_t, y);
    AT(ideatek_drop_t, kind);
}

static void show_event_wait(void)
{
    SIZE(ideatek_event_wait_t);
    AT(ideatek_event_wait_t, wait);
}

static void show_clipboard_request(void)
{
    SIZE(ideatek_clipboard_request_t);
    AT(ideatek_clipboard_request_t, text);
}

static void show_paste(void)
{
    SIZE(ideatek_paste_t);
    AT(ideatek_paste_t, text);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_window_mode_t_desc, ideatek_display_target_t_desc,
        ideatek_surface_t_desc, ideatek_drop_kind_t_desc,
        ideatek_window_request_t_desc, ideatek_window_state_t_desc,
        ideatek_display_t_desc, ideatek_video_mode_t_desc,
        ideatek_drop_t_desc, ideatek_event_wait_t_desc,
        ideatek_clipboard_request_t_desc,
        ideatek_paste_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_WINDOW_MODE_VALUES),
        TEXT_OF(IDEATEK_DISPLAY_TARGET_VALUES),
        TEXT_OF(IDEATEK_SURFACE_VALUES), TEXT_OF(IDEATEK_DROP_KIND_VALUES),
        TEXT_OF(IDEATEK_WINDOW_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_WINDOW_STATE_FIELDS), TEXT_OF(IDEATEK_DISPLAY_FIELDS),
        TEXT_OF(IDEATEK_VIDEO_MODE_FIELDS), TEXT_OF(IDEATEK_DROP_FIELDS),
        TEXT_OF(IDEATEK_EVENT_WAIT_FIELDS),
        TEXT_OF(IDEATEK_CLIPBOARD_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_PASTE_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "window_mode", show_window_mode },
        { "display_target", show_display_target },
        { "surface", show_surface }, { "drop_kind", show_drop_kind },
        { "request", show_request }, { "state", show_state },
        { "display", show_display }, { "video_mode", show_video_mode },
        { "drop", show_drop }, { "event_wait", show_event_wait },
        { "clipboard_request", show_clipboard_request },
        { "paste", show_paste }, { "descriptors", show_descriptors },
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
