/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_window.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_place(void)
{
    SIZE(ideatek_place_t);
}

static void show_window_mode(void)
{
    SIZE(ideatek_window_mode_t);
}

static void show_surface(void)
{
    SIZE(ideatek_surface_t);
}

static void show_flash(void)
{
    SIZE(ideatek_flash_t);
}

static void show_orientation(void)
{
    SIZE(ideatek_orientation_t);
}

static void show_request(void)
{
    SIZE(ideatek_window_request_t);
    AT(ideatek_window_request_t, title);
    AT(ideatek_window_request_t, display);
    AT(ideatek_window_request_t, video_mode);
    AT(ideatek_window_request_t, x);
    AT(ideatek_window_request_t, y);
    AT(ideatek_window_request_t, width);
    AT(ideatek_window_request_t, height);
    AT(ideatek_window_request_t, min_width);
    AT(ideatek_window_request_t, min_height);
    AT(ideatek_window_request_t, max_width);
    AT(ideatek_window_request_t, max_height);
    AT(ideatek_window_request_t, flashes);
    AT(ideatek_window_request_t, raises);
    AT(ideatek_window_request_t, place);
    AT(ideatek_window_request_t, mode);
    AT(ideatek_window_request_t, flash);
    AT(ideatek_window_request_t, resizable);
    AT(ideatek_window_request_t, borderless);
    AT(ideatek_window_request_t, visible);
    AT(ideatek_window_request_t, always_on_top);
}

static void show_state(void)
{
    SIZE(ideatek_window_state_t);
    AT(ideatek_window_state_t, native_display);
    AT(ideatek_window_state_t, native_window);
    AT(ideatek_window_state_t, display);
    AT(ideatek_window_state_t, video_mode);
    AT(ideatek_window_state_t, x);
    AT(ideatek_window_state_t, y);
    AT(ideatek_window_state_t, width);
    AT(ideatek_window_state_t, height);
    AT(ideatek_window_state_t, pixel_width);
    AT(ideatek_window_state_t, pixel_height);
    AT(ideatek_window_state_t, border_top);
    AT(ideatek_window_state_t, border_left);
    AT(ideatek_window_state_t, border_bottom);
    AT(ideatek_window_state_t, border_right);
    AT(ideatek_window_state_t, safe_x);
    AT(ideatek_window_state_t, safe_y);
    AT(ideatek_window_state_t, safe_width);
    AT(ideatek_window_state_t, safe_height);
    AT(ideatek_window_state_t, display_scale);
    AT(ideatek_window_state_t, sdr_white_level);
    AT(ideatek_window_state_t, hdr_headroom);
    AT(ideatek_window_state_t, mode);
    AT(ideatek_window_state_t, surface);
    AT(ideatek_window_state_t, focused);
    AT(ideatek_window_state_t, mouse_inside);
    AT(ideatek_window_state_t, occluded);
    AT(ideatek_window_state_t, visible);
    AT(ideatek_window_state_t, resizable);
    AT(ideatek_window_state_t, borderless);
    AT(ideatek_window_state_t, always_on_top);
    AT(ideatek_window_state_t, hdr);
    AT(ideatek_window_state_t, close_asked);
}

static void show_display(void)
{
    SIZE(ideatek_display_t);
    AT(ideatek_display_t, name);
    AT(ideatek_display_t, desktop_mode);
    AT(ideatek_display_t, current_mode);
    AT(ideatek_display_t, x);
    AT(ideatek_display_t, y);
    AT(ideatek_display_t, width);
    AT(ideatek_display_t, height);
    AT(ideatek_display_t, usable_x);
    AT(ideatek_display_t, usable_y);
    AT(ideatek_display_t, usable_width);
    AT(ideatek_display_t, usable_height);
    AT(ideatek_display_t, content_scale);
    AT(ideatek_display_t, orientation);
    AT(ideatek_display_t, natural_orientation);
    AT(ideatek_display_t, hdr);
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

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_place_t_desc, ideatek_window_mode_t_desc,
        ideatek_surface_t_desc, ideatek_flash_t_desc,
        ideatek_orientation_t_desc, ideatek_window_request_t_desc,
        ideatek_window_state_t_desc, ideatek_display_t_desc,
        ideatek_video_mode_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_PLACE_VALUES), TEXT_OF(IDEATEK_WINDOW_MODE_VALUES),
        TEXT_OF(IDEATEK_SURFACE_VALUES), TEXT_OF(IDEATEK_FLASH_VALUES),
        TEXT_OF(IDEATEK_ORIENTATION_VALUES),
        TEXT_OF(IDEATEK_WINDOW_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_WINDOW_STATE_FIELDS), TEXT_OF(IDEATEK_DISPLAY_FIELDS),
        TEXT_OF(IDEATEK_VIDEO_MODE_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "place", show_place }, { "window_mode", show_window_mode },
        { "surface", show_surface }, { "flash", show_flash },
        { "orientation", show_orientation }, { "request", show_request },
        { "state", show_state }, { "display", show_display },
        { "video_mode", show_video_mode },
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
