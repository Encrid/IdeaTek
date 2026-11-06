/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "sdl3_start.h"

typedef struct {
    const char *name;
    SDL_InitFlags flags;
    bool other;
} probe_mode_t;

static const probe_mode_t MODES[] = {
    { "events", SDL_INIT_EVENTS, false },
    { "sensor", SDL_INIT_SENSOR, false },
    { "shared", SDL_INIT_SENSOR, true },
};

static void show_types(void)
{
    uint32_t first = 0;
    bool off = false;

    for (uint32_t type = 0; type <= 0x10000; type++) {
        bool now = type < 0x10000 && !SDL_EventEnabled(type);

        if (now && !off)
            first = type;
        if (!now && off)
            printf("off 0x%04x-0x%04x\n", first, type - 1);
        off = now;
    }
}

static int show_start(const probe_mode_t *mode)
{
    char reason[64] = "";
    bool started;

    if (mode->other)
        SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_SetHint(SDL_HINT_APP_NAME, "probe");
    SDL_SetError("no probe");
    started = sdl3_start_subsystem(mode->flags, reason, sizeof(reason));
    printf("start %s: [%s]\n", started ? "true" : "false", reason);
    printf("held %d\n", SDL_WasInit(mode->flags) == mode->flags);
    if (started)
        sdl3_stop_subsystem(mode->flags);
    printf("SDL3 %s\n", SDL_GetHint(SDL_HINT_APP_NAME) ? "runs" : "stopped");
    if (mode->other)
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    return (0);
}

static int show_list(bool other)
{
    char reason[64] = "";

    if (other)
        SDL_InitSubSystem(SDL_INIT_EVENTS);
    if (!sdl3_start_subsystem(SDL_INIT_EVENTS, reason, sizeof(reason)))
        return (1);
    show_types();
    sdl3_stop_subsystem(SDL_INIT_EVENTS);
    if (other)
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "types"))
        return (show_list(false));
    if (argc > 1 && !strcmp(argv[1], "second"))
        return (show_list(true));
    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name))
            return (show_start(&MODES[i]));
    printf("usage: start_probe <mode>\n");
    return (2);
}
