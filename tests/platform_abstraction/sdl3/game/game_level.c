/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "game_level.h"

#if defined(GAME_LEVEL)

#include "game_gltf.h"
#include "game_ktx2.h"
#include "game_resources.h"

typedef struct {
    uint32_t files;
    uint32_t asked;
    uint32_t window;
    uint32_t models;
    uint32_t meshes;
    uint32_t textures;
    uint64_t vertices;
    uint64_t indices;
    uint64_t texels;
    uint64_t start;
    uint64_t end;
    uint32_t start_round;
    uint32_t end_round;
} game_level_t;

static game_level_t level;
static uint32_t lists[3];

static void ask_batch(const bridge_api_t *api)
{
    ideatek_resource_load_t l = {};
    char path[32];
    uint32_t room = level.models + level.window;
    uint32_t end = room < level.files ? room : level.files;

    for (; level.asked < end; level.asked++) {
        snprintf(path, sizeof(path), "level_%03u.gltf", level.asked);
        l.path = game_put_text(api, path);
        l.type = game_put_text(api, "ideatek:Model");
        BRIDGE_NATIVE_ATTACH(api, api->spawn(api), game_resources_slots.load,
            l);
    }
}

static const bridge_trigger_t *triggers(const bridge_api_t *api,
    uint32_t list, uint32_t *count)
{
    const bridge_list_t *l = bridge_native_get_list(api, lists[list]);

    *count = l->count;
    return ((const void *)(api->private_base + l->at));
}

static void check_end(void)
{
    bool whole = level.files && level.models == level.files
        && level.meshes == level.files && level.textures == level.files;

    if (whole && !level.end) {
        level.end = SDL_GetTicksNS();
        level.end_round = game.round + 1;
    }
}

BRIDGE_NATIVE(seen_level_model)
{
    uint32_t count;
    const bridge_trigger_t *t = triggers(api, 0, &count);

    for (uint32_t i = 0; i < count; i++)
        level.models += !game_cell(api, t[i].id, game_resources_slots.label);
    check_end();
    return (0);
}

BRIDGE_NATIVE(seen_level_mesh)
{
    uint32_t count;
    const bridge_trigger_t *t = triggers(api, 1, &count);
    const ideatek_mesh_t *m;

    for (uint32_t i = 0; i < count; i++) {
        m = game_cell(api, t[i].id, game_gltf_slots.mesh);
        level.meshes += m != nullptr;
        level.vertices += m ? m->vertex_count : 0;
        level.indices += m ? m->index_count : 0;
    }
    check_end();
    return (0);
}

BRIDGE_NATIVE(seen_level_texture)
{
    uint32_t count;
    const bridge_trigger_t *t = triggers(api, 2, &count);
    const ideatek_texture_t *x;

    for (uint32_t i = 0; i < count; i++) {
        x = game_cell(api, t[i].id, game_ktx2_slots.texture);
        level.textures += x != nullptr;
        level.texels += x ? x->bytes.len : 0;
    }
    check_end();
    return (0);
}

bool game_level_step(const bridge_api_t *api, const char *verb, char *args)
{
    const char *window = strchr(args, ',');

    (void)api;
    if (strcmp(verb, "level"))
        return (false);
    level.files = (uint32_t)atoi(args);
    level.window = window ? (uint32_t)atoi(window + 1) : level.files;
    level.start = SDL_GetTicksNS();
    level.start_round = game.round;
    return (true);
}

void game_level_play(const bridge_api_t *api)
{
    if (level.asked < level.files)
        ask_batch(api);
}

void game_watch_level(const bridge_api_t *api)
{
    bridge_term_t model[1] = {{ game_gltf_slots.model, BRIDGE_READ,
        BRIDGE_AND, 0 }};
    bridge_term_t mesh[1] = {{ game_gltf_slots.mesh, BRIDGE_READ, BRIDGE_AND,
        0 }};
    bridge_term_t texture[1] = {{ game_ktx2_slots.texture, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    lists[0] = BRIDGE_NATIVE_OBSERVER(api, "seen_level_model",
        BRIDGE_ON_ENTER, model, 1, nullptr, 0, nullptr, 0);
    lists[1] = BRIDGE_NATIVE_OBSERVER(api, "seen_level_mesh", BRIDGE_ON_ENTER,
        mesh, 1, nullptr, 0, nullptr, 0);
    lists[2] = BRIDGE_NATIVE_OBSERVER(api, "seen_level_texture",
        BRIDGE_ON_ENTER, texture, 1, nullptr, 0, nullptr, 0);
}

void game_level_end(const bridge_api_t *api)
{
    bridge_native_say(api, BRIDGE_LOG_INFO, "level %u models %u meshes %u "
        "textures %u vertices %llu indices %llu texels %llu rounds %u "
        "load %.3f ms", level.files, level.models, level.meshes,
        level.textures, (unsigned long long)level.vertices,
        (unsigned long long)level.indices, (unsigned long long)level.texels,
        level.end_round - level.start_round,
        level.end ? (double)(level.end - level.start) / 1e6 : -1.0);
}

#endif
