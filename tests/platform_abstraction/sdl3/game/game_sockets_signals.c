/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_sockets.h"

#if defined(GAME_SOCKETS)

static void safe_name(char *out, size_t size, const char *identity)
{
    size_t i = 0;

    for (; identity[i] && i < size - 1; i++)
        out[i] = (identity[i] == ':' || identity[i] == '/'
            || identity[i] == '\\') ? '_' : identity[i];
    out[i] = '\0';
}

void game_sockets_write_signals(const bridge_api_t *api,
    const ideatek_batch_t *batch)
{
    const uint8_t *at = api->shared_base + batch->data.at;
    uint32_t left = batch->data.len;
    uint64_t connection;
    char peer[128];
    char name[128];
    char path[1024];
    uint32_t len;

    while (left >= 140) {
        memcpy(&connection, at, 8);
        memcpy(peer, at + 8, 128);
        peer[127] = '\0';
        memcpy(&len, at + 136, 4);
        if (left < 140 + len)
            break;
        safe_name(name, sizeof(name), peer);
        snprintf(path, sizeof(path), "%s/%s.%06u.sig", game_sockets.folder,
            name, game_sockets.sent_signals++);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u signal out %u to [%s] %u",
            game.round + 1, game_sock_index(connection), peer, len);
        if (game_sockets.folder[0])
            SDL_SaveFile(path, at + 140, len);
        at += 140 + len;
        left -= 140 + len;
    }
}

static SDL_EnumerationResult SDLCALL take_one(void *userdata, const char *dir,
    const char *fname)
{
    char **names = userdata;
    size_t used = strlen(*names);
    char self[128];

    (void)dir;
    safe_name(self, sizeof(self), game_sockets.identity);
    if (strncmp(fname, self, strlen(self)) || fname[strlen(self)] != '.')
        return (SDL_ENUM_CONTINUE);
    if (used + strlen(fname) + 2 < 65536) {
        strcat(*names, fname);
        strcat(*names, "\n");
    }
    return (SDL_ENUM_CONTINUE);
}

static int compare(const void *a, const void *b)
{
    return (strcmp(*(const char *const *)a, *(const char *const *)b));
}

void game_sockets_signals_step(const bridge_api_t *api)
{
    static char names[65536];
    static uint8_t batch[1 << 20];
    char *list[1024];
    size_t count = 0;
    char *left = nullptr;
    char path[1024];
    size_t len;
    void *data;
    uint32_t used = 0;
    uint32_t n;
    ideatek_batch_t b = {};

    if (!game_sockets.folder[0] || !game_sockets.identity[0])
        return;
    names[0] = '\0';
    SDL_EnumerateDirectory(game_sockets.folder, take_one, &(char *){ names });
    for (char *name = strtok_r(names, "\n", &left); name && count < 1024;
        name = strtok_r(nullptr, "\n", &left))
        list[count++] = name;
    qsort(list, count, sizeof(*list), compare);
    for (size_t i = 0; i < count; i++) {
        snprintf(path, sizeof(path), "%s/%s", game_sockets.folder, list[i]);
        data = SDL_LoadFile(path, &len);
        if (!data)
            continue;
        n = (uint32_t)len;
        memcpy(batch + used, &n, 4);
        memcpy(batch + used + 4, data, len);
        used += 4 + n;
        SDL_free(data);
        SDL_RemovePath(path);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u signal in %u",
            game.round + 1, n);
    }
    if (!used)
        return;
    b.data = game_sock_bytes(api, batch, used);
    BRIDGE_NATIVE_ATTACH(api, game_sockets.self,
        game_sockets_slots.signals_in, b);
}

#endif
