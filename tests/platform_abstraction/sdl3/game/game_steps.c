/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game.h"

#include "game_audio.h"
#include "game_files.h"
#include "game_frame.h"
#include "game_gpu.h"
#include "game_sockets.h"
#include "game_states.h"
#include "game_space.h"
#include "game_resources.h"
#include "game_ktx2.h"

static const char HINT[] = "IDEATEK_TEST_DISPLAYS";
static const uint64_t DEAD = (7ULL << 32) | 9ULL;

static const struct {
    const char *name;
    uint32_t type;
} TYPES[] = {
    { "quit", SDL_EVENT_QUIT },
    { "close", SDL_EVENT_WINDOW_CLOSE_REQUESTED },
    { "exposed", SDL_EVENT_WINDOW_EXPOSED },
    { "display", SDL_EVENT_DISPLAY_MOVED },
    { "clipboard", SDL_EVENT_CLIPBOARD_UPDATE },
    { "drop", SDL_EVENT_DROP_FILE },
    { "keydown", SDL_EVENT_KEY_DOWN },
    { "sensor", SDL_EVENT_SENSOR_UPDATE },
    { "user", SDL_EVENT_USER },
    { "keymap", SDL_EVENT_KEYMAP_CHANGED },
    { "joyaxis", SDL_EVENT_JOYSTICK_AXIS_MOTION },
};

static uint32_t type_of(const char *name)
{
    for (size_t i = 0; i < SDL_arraysize(TYPES); i++)
        if (!strcmp(TYPES[i].name, name))
            return (TYPES[i].type);
    return (0);
}

static SDL_WindowID window_id(void)
{
    int count = 0;
    SDL_Window **windows = SDL_GetWindows(&count);
    SDL_WindowID id = count ? SDL_GetWindowID(windows[0]) : 0;

    SDL_free(windows);
    return (id);
}

static uint64_t find_mode(const char *text)
{
    char name[128] = "";
    unsigned order = 0;
    uint64_t display;

    if (!strcmp(text, "dead"))
        return (DEAD);
    if (sscanf(text, "%127[^/]/%u", name, &order) != 2)
        return (0);
    display = game_find_display(name);
    for (uint32_t i = 0; i < game.mode_count; i++)
        if (game.modes[i].display == display && game.modes[i].order == order)
            return (game.modes[i].entity);
    return (0);
}

static ideatek_window_mode_t mode_of(const char *text)
{
    if (!strcmp(text, "fullscreen"))
        return (IDEATEK_WINDOW_MODE_FULLSCREEN);
    if (!strcmp(text, "exclusive"))
        return (IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN);
    return (IDEATEK_WINDOW_MODE_WINDOWED);
}

static void ask_window(const bridge_api_t *api, char *args)
{
    char *left = nullptr;
    char *f[7] = {};
    ideatek_window_request_t *r = &game.request;

    for (int i = 0; i < 7; i++)
        f[i] = strtok_r(i ? nullptr : args, ",", &left);
    if (!f[6])
        return;
    memset(r, 0, sizeof(*r));
    memcpy(r->title, f[0], strlen(f[0]) < 128 ? strlen(f[0]) : 128);
    r->width = (uint32_t)atoi(f[1]);
    r->height = (uint32_t)atoi(f[2]);
    r->mode = mode_of(f[3]);
    r->target = strcmp(f[4], "chosen") ? IDEATEK_DISPLAY_TARGET_DEFAULT
        : IDEATEK_DISPLAY_TARGET_CHOSEN;
    r->display.id = strcmp(f[5], "dead") ? game_find_display(f[5]) : DEAD;
    r->video_mode.id = find_mode(f[6]);
    if (!game.window)
        game.window = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, game.window, game.slots.request, *r);
}

static bridge_text_t put_text(const bridge_api_t *api, const char *text)
{
    uint32_t len = (uint32_t)strlen(text);
    uint32_t at = api->alloc(api, len ? len : 1);

    memcpy(api->shared_base + at, text, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

static void push(const bridge_api_t *api, const char *name)
{
    SDL_Event e = {};
    bool done;

    e.type = type_of(name);
    e.window.windowID = window_id();
    done = SDL_PushEvent(&e);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u pushed %s %d", game.round,
        name, done);
}

static SDL_Keymod mods_of(const char *text)
{
    SDL_Keymod mods = SDL_KMOD_NONE;

    if (strstr(text, "ctrl"))
        mods |= SDL_KMOD_LCTRL;
    if (strstr(text, "gui"))
        mods |= SDL_KMOD_LGUI;
    if (strstr(text, "alt"))
        mods |= SDL_KMOD_LALT;
    return (mods);
}

static void press(const char *args, int count)
{
    char key[8] = "";
    char mods[32] = "none";
    SDL_Event e = {};

    sscanf(args, "%7s %31s", key, mods);
    e.type = SDL_EVENT_KEY_DOWN;
    e.key.windowID = window_id();
    e.key.key = key[0] == 'v' ? SDLK_V : SDLK_X;
    e.key.mod = mods_of(mods);
    e.key.down = true;
    for (int i = 0; i < count; i++)
        SDL_PushEvent(&e);
}

static void push_drop(uint32_t type, SDL_WindowID id, const char *source,
    const char *data)
{
    SDL_Event e = {};

    e.type = type;
    e.drop.windowID = id;
    e.drop.source = source;
    e.drop.data = data;
    e.drop.x = 12.0F;
    e.drop.y = 34.0F;
    SDL_PushEvent(&e);
}

static void drop(char *args, bool windowed)
{
    char *left = nullptr;
    char *kind = strtok_r(args, " ", &left);
    char *source = strtok_r(nullptr, " ", &left);
    uint32_t type = strcmp(kind, "text") ? SDL_EVENT_DROP_FILE
        : SDL_EVENT_DROP_TEXT;
    SDL_WindowID id = windowed ? window_id() : 0;

    if (source && !strcmp(source, "-"))
        source = nullptr;
    push_drop(SDL_EVENT_DROP_BEGIN, id, source, nullptr);
    for (char *item = strtok_r(nullptr, " ", &left); item;
        item = strtok_r(nullptr, " ", &left))
        push_drop(type, id, source, item);
    push_drop(SDL_EVENT_DROP_COMPLETE, id, source, nullptr);
}

static void drag(const char *args)
{
    SDL_Event e = {};

    push_drop(SDL_EVENT_DROP_BEGIN, window_id(), nullptr, nullptr);
    e.type = SDL_EVENT_DROP_POSITION;
    e.drop.windowID = window_id();
    sscanf(args, "%f %f", &e.drop.x, &e.drop.y);
    SDL_PushEvent(&e);
}

static Uint32 SDLCALL ping(void *unused, SDL_TimerID id, Uint32 ms)
{
    SDL_Event e = {};

    (void)unused;
    (void)id;
    (void)ms;
    e.type = SDL_EVENT_WINDOW_EXPOSED;
    e.window.windowID = window_id();
    SDL_PushEvent(&e);
    return (0);
}

static void say_gap(const bridge_api_t *api)
{
    uint64_t now = SDL_GetTicksNS();

    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u gap %llu", game.round,
        (unsigned long long)((now - game.last) / 1000000));
}

static void say_clipboard(const bridge_api_t *api)
{
    char *text = SDL_GetClipboardText();

    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u clipboard [%s]", game.round,
        text ? text : "");
    SDL_free(text);
}

static void say_queue(const bridge_api_t *api)
{
    SDL_Event events[64];
    int count = SDL_PeepEvents(events, 64, SDL_PEEKEVENT, SDL_EVENT_FIRST,
        SDL_EVENT_LAST);

    for (int i = 0; i < count; i++)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u queue 0x%x", game.round,
            events[i].type);
}

static void say_has(const bridge_api_t *api, const char *name)
{
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u has %s %d", game.round, name,
        SDL_HasEvent(type_of(name)));
}

static void clip(const bridge_api_t *api, const char *args, int how)
{
    ideatek_clipboard_request_t c = { .text = { .at = 0, .len = 4 } };
    uint64_t id = api->spawn(api);

    if (how != 1)
        c.text = put_text(api, args);
    if (how == 2)
        c.text.len = 1U << 30;
    if (how == 4)
        c.text.at = UINT32_MAX - 8;
    BRIDGE_NATIVE_ATTACH(api, id, game.slots.clipboard, c);
    if (how == 3)
        api->destroy(api, id);
}

static void wait_events(const bridge_api_t *api, const char *args)
{
    ideatek_event_wait_t w = { .wait = atof(args) };

    if (!game.window)
        game.window = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, game.window, game.slots.event_wait, w);
}

static void resize(const char *args)
{
    int w = 0;
    int h = 0;
    int count = 0;
    SDL_Window **windows = SDL_GetWindows(&count);

    sscanf(args, "%d %d", &w, &h);
    if (count)
        SDL_SetWindowSize(windows[0], w, h);
    SDL_free(windows);
}

static void second(const bridge_api_t *api)
{
    game.second = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, game.second, game.slots.request, game.request);
}

static void run_step(const bridge_api_t *api, const char *verb, char *args)
{
    if (game_input_step(api, verb, args))
        return;
#if defined(GAME_GPU)
    if (game_gpu_step(api, verb, args))
        return;
#endif
#if defined(GAME_FRAME)
    if (game_frame_step(api, verb, args))
        return;
#endif
#if defined(GAME_FILES)
    if (game_files_step(api, verb, args))
        return;
#endif
#if defined(GAME_AUDIO)
    if (game_audio_step(api, verb, args))
        return;
#endif
#if defined(GAME_SOCKETS)
    if (game_sockets_step(api, verb, args))
        return;
#endif
#if defined(GAME_STATES)
    if (game_states_step(api, verb, args))
        return;
#endif
#if defined(GAME_RESOURCES)
    if (game_resources_step(api, verb, args))
        return;
#endif
#if defined(GAME_KTX2)
    if (game_ktx2_step(api, verb, args))
        return;
#endif
#if defined(GAME_SPACE)
    if (game_space_step(api, verb, args))
        return;
#endif
    if (!strcmp(verb, "request"))
        ask_window(api, args);
    if (!strcmp(verb, "second"))
        second(api);
    if (!strcmp(verb, "destroy"))
        api->destroy(api, game.window);
    if (!strcmp(verb, "detach"))
        api->detach(api, game.window, game.slots.request);
    if (!strcmp(verb, "wait"))
        wait_events(api, args);
    if (!strcmp(verb, "clip"))
        clip(api, args, 0);
    if (!strcmp(verb, "clipout"))
        clip(api, args, 1);
    if (!strcmp(verb, "cliplong"))
        clip(api, args, 2);
    if (!strcmp(verb, "clipgone"))
        clip(api, args, 3);
    if (!strcmp(verb, "clipfar"))
        clip(api, args, 4);
    if (!strcmp(verb, "takekeys"))
        SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
    if (!strcmp(verb, "unsecond"))
        api->destroy(api, game.second);
    if (!strcmp(verb, "readclip"))
        say_clipboard(api);
    if (!strcmp(verb, "setclip"))
        SDL_SetClipboardText(args);
    if (!strcmp(verb, "noclip"))
        SDL_ClearClipboardData();
    if (!strcmp(verb, "key"))
        press(args, 1);
    if (!strcmp(verb, "keys"))
        press(strchr(args, ' ') + 1, atoi(args));
    if (!strcmp(verb, "push"))
        push(api, args);
    if (!strcmp(verb, "queue"))
        say_queue(api);
    if (!strcmp(verb, "has"))
        say_has(api, args);
    if (!strcmp(verb, "enabled"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u enabled %s %d",
            game.round, args, SDL_EventEnabled(type_of(args)));
    if (!strcmp(verb, "drop"))
        drop(args, true);
    if (!strcmp(verb, "drop0"))
        drop(args, false);
    if (!strcmp(verb, "drag"))
        drag(args);
    if (!strcmp(verb, "dragend"))
        push_drop(SDL_EVENT_DROP_COMPLETE, window_id(), nullptr, nullptr);
    if (!strcmp(verb, "displays"))
        SDL_SetHintWithPriority(HINT, args, SDL_HINT_OVERRIDE);
    if (!strcmp(verb, "resize"))
        resize(args);
    if (!strcmp(verb, "ping"))
        SDL_AddTimer((Uint32)atoi(args), ping, nullptr);
    if (!strcmp(verb, "stall"))
        SDL_Delay((Uint32)atoi(args));
    if (!strcmp(verb, "gap"))
        say_gap(api);
}

static void read_script(const bridge_api_t *api)
{
    const game_settings_t *s = bridge_native_get_single(api,
        game.slots.settings);
    uint32_t len = s->script.len < sizeof(game.script) - 1 ? s->script.len
        : (uint32_t)sizeof(game.script) - 1;

    memcpy(game.script, api->shared_base + s->script.at, len);
    game.script[len] = '\0';
}

static void run_steps(const bridge_api_t *api)
{
    static char steps[65536];
    char *left = nullptr;
    char *rest;
    char *verb;

    memcpy(steps, game.script, sizeof(steps));
    for (char *step = strtok_r(steps, "|", &left); step;
        step = strtok_r(nullptr, "|", &left)) {
        if ((uint32_t)atoi(step) != game.round || !strchr(step, ':'))
            continue;
        verb = strchr(step, ':') + 1;
        rest = strchr(verb, ' ');
        if (rest)
            *rest++ = '\0';
        run_step(api, verb, rest ? rest : verb + strlen(verb));
    }
}

void game_start(const bridge_api_t *api)
{
    char request[] = "Game,640,480,windowed,default,-,-";

    ask_window(api, request);
    wait_events(api, "1");
    game.last = SDL_GetTicksNS();
}

void game_play(const bridge_api_t *api)
{
    if (!game.script[0])
        read_script(api);
    game.round++;
    run_steps(api);
    game.last = SDL_GetTicksNS();
}
