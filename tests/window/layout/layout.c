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

static void show_request(void)
{
    SIZE(ideatek_window_request_t);
    AT(ideatek_window_request_t, title);
    AT(ideatek_window_request_t, width);
    AT(ideatek_window_request_t, height);
    AT(ideatek_window_request_t, x);
    AT(ideatek_window_request_t, y);
    AT(ideatek_window_request_t, screen);
    AT(ideatek_window_request_t, place);
    AT(ideatek_window_request_t, mode);
    AT(ideatek_window_request_t, resizable);
    AT(ideatek_window_request_t, borderless);
}

static void show_state(void)
{
    SIZE(ideatek_window_state_t);
    AT(ideatek_window_state_t, display);
    AT(ideatek_window_state_t, window);
    AT(ideatek_window_state_t, width);
    AT(ideatek_window_state_t, height);
    AT(ideatek_window_state_t, scale);
    AT(ideatek_window_state_t, x);
    AT(ideatek_window_state_t, y);
    AT(ideatek_window_state_t, screen);
    AT(ideatek_window_state_t, mode);
    AT(ideatek_window_state_t, focused);
    AT(ideatek_window_state_t, close_asked);
    AT(ideatek_window_state_t, surface);
}

static void show_descriptors(void)
{
    printf("%s\n%s\n%s\n%s\n%s\n", ideatek_place_t_desc,
        ideatek_window_mode_t_desc, ideatek_surface_t_desc,
        ideatek_window_request_t_desc, ideatek_window_state_t_desc);
}

static void show_texts(void)
{
    printf("%s\n%s\n%s\n%s\n%s\n", TEXT_OF(IDEATEK_PLACE_VALUES),
        TEXT_OF(IDEATEK_WINDOW_MODE_VALUES), TEXT_OF(IDEATEK_SURFACE_VALUES),
        TEXT_OF(IDEATEK_WINDOW_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_WINDOW_STATE_FIELDS));
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "place", show_place }, { "window_mode", show_window_mode },
        { "surface", show_surface }, { "request", show_request },
        { "state", show_state }, { "descriptors", show_descriptors },
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
