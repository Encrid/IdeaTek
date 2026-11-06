/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "SDL_internal.h"
#include "video/SDL_sysvideo.h"
#include "events/SDL_displayevents_c.h"
#include "events/SDL_windowevents_c.h"
#include "events/SDL_mouse_c.h"
#include "events/SDL_keyboard_c.h"
#include "events/SDL_touch_c.h"

struct SDL_DisplayData {
    char key[16];
};

typedef struct {
    char *key;
    char *name;
    char *modes;
    char *desktop;
    char *current;
    float scale;
    float headroom;
} test_display_t;

static const char HINT[] = "IDEATEK_TEST_DISPLAYS";
static const char FOCUS[] = "IDEATEK_TEST_FOCUS";
static const char TOUCH[] = "IDEATEK_TEST_TOUCH";
static const char DENSITY[] = "IDEATEK_TEST_DENSITY";
static const char FALLBACK[] = "a,Test,1920x1080@60,1920x1080@60,desktop,1,1";

static bool read_mode(const char *text, SDL_DisplayMode *mode)
{
    const char *slash = SDL_strchr(text, '/');
    const char *star = SDL_strchr(text, '*');

    SDL_zerop(mode);
    mode->format = SDL_PIXELFORMAT_XRGB8888;
    mode->pixel_density = star ? (float)SDL_atof(star + 1) : 1.0f;
    if (SDL_sscanf(text, "%dx%d@%d", &mode->w, &mode->h,
            &mode->refresh_rate_numerator) != 3) {
        return false;
    }
    mode->refresh_rate_denominator = slash ? SDL_atoi(slash + 1)
        : (mode->refresh_rate_numerator ? 1 : 0);
    return true;
}

static void add_modes(SDL_VideoDisplay *display, char *modes)
{
    SDL_DisplayMode mode;
    char *left = NULL;
    int count = 0;

    if (SDL_sscanf(modes, "gen%d", &count) == 1) {
        for (int i = 0; i < count; i++) {
            SDL_zero(mode);
            mode.format = SDL_PIXELFORMAT_XRGB8888;
            mode.w = 4000 - i;
            mode.h = 2000;
            mode.refresh_rate_numerator = 60;
            mode.refresh_rate_denominator = 1;
            SDL_AddFullscreenDisplayMode(display, &mode);
        }
        return;
    }
    for (char *m = SDL_strtok_r(modes, " ", &left); m;
            m = SDL_strtok_r(NULL, " ", &left)) {
        if (read_mode(m, &mode)) {
            SDL_AddFullscreenDisplayMode(display, &mode);
        }
    }
}

static const SDL_DisplayMode *find_current(SDL_VideoDisplay *display,
    const char *text)
{
    SDL_DisplayMode mode;

    if (!read_mode(text, &mode)) {
        return &display->desktop_mode;
    }
    for (int i = 0; i < display->num_fullscreen_modes; i++) {
        const SDL_DisplayMode *m = &display->fullscreen_modes[i];
        if (m->w == mode.w && m->h == mode.h
                && m->refresh_rate_numerator == mode.refresh_rate_numerator
                && m->pixel_density == mode.pixel_density) {
            return m;
        }
    }
    return &display->desktop_mode;
}

static SDL_VideoDisplay *find_key(const char *key)
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();

    for (int i = 0; i < device->num_displays; i++) {
        SDL_VideoDisplay *display = device->displays[i];
        if (display->internal && SDL_strcmp(display->internal->key, key) == 0) {
            return display;
        }
    }
    return NULL;
}

static SDL_VideoDisplay *add_display(const test_display_t *spec)
{
    static char cont[131];
    SDL_VideoDisplay display;
    SDL_DisplayData *data = SDL_calloc(1, sizeof(*data));
    SDL_DisplayID id;

    SDL_zero(display);
    SDL_strlcpy(data->key, spec->key, sizeof(data->key));
    display.name = spec->name;
    if (SDL_strcmp(spec->name, "@cont") == 0) {
        SDL_memset(cont, 0x80, 130);
        display.name = cont;
    }
    display.internal = data;
    read_mode(spec->desktop, &display.desktop_mode);
    id = SDL_AddVideoDisplay(&display, true);
    return SDL_GetVideoDisplay(id);
}

static void update_display(const test_display_t *spec)
{
    SDL_VideoDisplay *display = find_key(spec->key);
    SDL_DisplayMode desktop;
    SDL_HDROutputProperties hdr = { 1.0f, spec->headroom };

    if (!display) {
        display = add_display(spec);
    }
    SDL_ResetFullscreenDisplayModes(display);
    if (SDL_strcmp(spec->modes, "-") != 0) {
        add_modes(display, spec->modes);
    }
    read_mode(spec->desktop, &desktop);
    SDL_SetDesktopDisplayMode(display, &desktop);
    SDL_SetCurrentDisplayMode(display, find_current(display, spec->current));
    SDL_SetDisplayContentScale(display, spec->scale);
    if (spec->headroom > 1.0f) {
        hdr.SDR_white_level = 2.0f;
    }
    SDL_SetDisplayHDRProperties(display, &hdr);
    SDL_SetBooleanProperty(SDL_GetDisplayProperties(display->id),
        SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN, spec->headroom > 1.0f);
    SDL_SendDisplayEvent(display, SDL_EVENT_DISPLAY_MOVED, 0, 0);
}

static bool read_display(char *text, test_display_t *spec)
{
    char *left = NULL;
    char *fields[7];

    for (int i = 0; i < 7; i++) {
        fields[i] = SDL_strtok_r(i ? NULL : text, ",", &left);
        if (!fields[i]) {
            return false;
        }
    }
    spec->key = fields[0];
    spec->name = fields[1];
    spec->modes = fields[2];
    spec->desktop = fields[3];
    spec->current = fields[4];
    spec->scale = (float)SDL_atof(fields[5]);
    spec->headroom = (float)SDL_atof(fields[6]);
    return true;
}

static bool is_named(const char *text, const char *key)
{
    size_t len = SDL_strlen(key);

    for (const char *at = text; at; at = SDL_strchr(at, ';')) {
        at += (*at == ';');
        if (SDL_strncmp(at, key, len) == 0 && at[len] == ',') {
            return true;
        }
    }
    return false;
}

static void remove_missing(const char *text)
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();

    for (int i = device->num_displays; i--;) {
        SDL_VideoDisplay *display = device->displays[i];
        if (display->internal && !is_named(text, display->internal->key)) {
            SDL_DelVideoDisplay(display->id, true);
        }
    }
}

static void SDLCALL apply_displays(void *unused, const char *name,
    const char *old, const char *value)
{
    char *copy = SDL_strdup(value && *value ? value : FALLBACK);
    char *left = NULL;
    test_display_t spec;

    remove_missing(copy);
    for (char *d = SDL_strtok_r(copy, ";", &left); d;
            d = SDL_strtok_r(NULL, ";", &left)) {
        if (read_display(d, &spec)) {
            update_display(&spec);
        }
    }
    SDL_free(copy);
}

static void SDLCALL apply_focus(void *unused, const char *name,
    const char *old, const char *value)
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();

    if (device && device->windows) {
        SDL_SetKeyboardFocus(value && SDL_strcmp(value, "1") == 0
            ? device->windows : NULL);
    }
}

static void SDLCALL apply_touch(void *unused, const char *name,
    const char *old, const char *value)
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();
    unsigned kind = 0;
    unsigned long long finger = 0;
    float x = 0.0f;
    float y = 0.0f;

    if (!device || !value || SDL_sscanf(value, "%u %llu %f %f", &kind,
            &finger, &x, &y) != 4) {
        return;
    }
    SDL_AddTouch(1, SDL_TOUCH_DEVICE_DIRECT, "ideatek_test");
    if (kind == 1) {
        SDL_SendTouchMotion(0, 1, finger, device->windows, x, y, 1.0f);
    } else {
        SDL_SendTouch(0, 1, finger, device->windows, kind == 0
            ? SDL_EVENT_FINGER_DOWN : (kind == 2 ? SDL_EVENT_FINGER_UP
            : SDL_EVENT_FINGER_CANCELED), x, y, 1.0f);
    }
}

static void TEST_GetWindowSizeInPixels(SDL_VideoDevice *_this,
    SDL_Window *window, int *w, int *h)
{
    int density = SDL_GetHintBoolean(DENSITY, false) ? 2 : 1;

    *w = window->w * density;
    *h = window->h * density;
}

static SDL_Cursor *TEST_CreateSystemCursor(SDL_SystemCursor id)
{
    return SDL_calloc(1, sizeof(SDL_Cursor));
}

static bool TEST_ShowCursor(SDL_Cursor *cursor)
{
    return true;
}

static void TEST_FreeCursor(SDL_Cursor *cursor)
{
    SDL_free(cursor);
}

static bool TEST_SetRelativeMouseMode(bool enabled)
{
    return true;
}

static bool TEST_SetWindowMouseGrab(SDL_VideoDevice *_this,
    SDL_Window *window, bool grabbed)
{
    return true;
}

static bool TEST_VideoInit(SDL_VideoDevice *_this)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    mouse->CreateSystemCursor = TEST_CreateSystemCursor;
    mouse->ShowCursor = TEST_ShowCursor;
    mouse->FreeCursor = TEST_FreeCursor;
    mouse->SetRelativeMouseMode = TEST_SetRelativeMouseMode;
    SDL_AddHintCallback(HINT, apply_displays, NULL);
    SDL_AddHintCallback(FOCUS, apply_focus, NULL);
    SDL_AddHintCallback(TOUCH, apply_touch, NULL);
    return _this->num_displays > 0;
}

static void TEST_VideoQuit(SDL_VideoDevice *_this)
{
    SDL_RemoveHintCallback(HINT, apply_displays, NULL);
    SDL_RemoveHintCallback(FOCUS, apply_focus, NULL);
    SDL_RemoveHintCallback(TOUCH, apply_touch, NULL);
}

static void TEST_PumpEvents(SDL_VideoDevice *_this)
{
}

static bool TEST_SetWindowPosition(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_VideoDisplay *display;

    SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MOVED, window->pending.x,
        window->pending.y);
    display = SDL_GetVideoDisplay(SDL_GetDisplayForWindow(window));
    if (display) {
        SDL_SetWindowHDRProperties(window, &display->HDR, true);
    }
    return true;
}

static bool TEST_SetWindowIcon(SDL_VideoDevice *_this, SDL_Window *window,
    SDL_Surface *icon)
{
    return true;
}

static void TEST_SetWindowSize(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_RESIZED, window->pending.w,
        window->pending.h);
}

static void TEST_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_free(device);
}

static SDL_VideoDevice *TEST_CreateDevice(void)
{
    const char *hint = SDL_GetHint(SDL_HINT_VIDEO_DRIVER);
    SDL_VideoDevice *device;

    if (!hint || SDL_strcmp(hint, "ideatek_test") != 0) {
        return NULL;
    }
    device = SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        return NULL;
    }
    device->is_dummy = true;
    device->VideoInit = TEST_VideoInit;
    device->VideoQuit = TEST_VideoQuit;
    device->PumpEvents = TEST_PumpEvents;
    device->SetWindowSize = TEST_SetWindowSize;
    device->SetWindowPosition = TEST_SetWindowPosition;
    device->SetWindowIcon = TEST_SetWindowIcon;
    device->SetWindowMouseGrab = TEST_SetWindowMouseGrab;
    device->GetWindowSizeInPixels = TEST_GetWindowSizeInPixels;
    device->free = TEST_DeleteDevice;
    return device;
}

VideoBootStrap IDEATEK_TEST_bootstrap = {
    "ideatek_test", "The video driver of the tests of IdeaTek",
    TEST_CreateDevice, NULL, false
};
