/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_space.h"

#if defined(GAME_SPACE)

#define TYPED(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_space_slots_t game_space_slots;

static uint64_t nodes[2048];
static ideatek_transform_t locals[2048];
static uint32_t node_count;
static uint32_t revisions[2048];

static int index_of(uint64_t id)
{
    for (uint32_t k = 0; k < node_count; k++)
        if (id && nodes[k] == id)
            return ((int)k);
    return (-1);
}

static void put_transform(const bridge_api_t *api, uint32_t k)
{
    BRIDGE_NATIVE_ATTACH(api, nodes[k], game_space_slots.transform, locals[k]);
}

static void put_parent(const bridge_api_t *api, uint32_t k, int p,
    const unsigned *v)
{
    ideatek_parent_t parent = { .order = (int32_t)v[0],
        .keep = (ideatek_keep_t)v[1], .orphan = (ideatek_orphan_t)v[2],
        .absolute = (uint8_t)v[3] };

    if (p < 0) {
        api->detach(api, nodes[k], game_space_slots.parent);
        return;
    }
    parent.entity.id = nodes[p];
    BRIDGE_NATIVE_ATTACH(api, nodes[k], game_space_slots.parent, parent);
}

static void add_node(const bridge_api_t *api, const char *args, bool child)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    int p = -1;
    unsigned v[4] = { 0, 0, 0, 0 };
    uint32_t k = node_count++;

    if (child)
        sscanf(args, "%d,%f,%f,%f,%u,%u,%u,%u", &p, &x, &y, &z, &v[0], &v[1],
            &v[2], &v[3]);
    else
        sscanf(args, "%f,%f,%f", &x, &y, &z);
    nodes[k] = api->spawn(api);
    locals[k] = (ideatek_transform_t){ .position = { x, y, z },
        .rotation = { .w = 1.0f }, .scale = { 1.0f, 1.0f, 1.0f } };
    put_transform(api, k);
    if (child)
        put_parent(api, k, p, v);
}

static void add_many(const bridge_api_t *api, const char *args, int parent)
{
    const char *count = parent < 0 ? args : strchr(args, ',');
    int n = count ? atoi(count + (parent >= 0)) : 0;
    char line[64];

    for (int i = 0; i < n; i++) {
        if (parent < 0)
            snprintf(line, sizeof(line), "%d,0,0", i);
        else
            snprintf(line, sizeof(line), "%d,%d,0,0,0,0,0,0", parent, i);
        add_node(api, line, parent >= 0);
    }
}

static void change(const bridge_api_t *api, const char *verb, char *args)
{
    unsigned k = 0;
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    sscanf(args, "%u,%f,%f,%f", &k, &a, &b, &c);
    if (!strcmp(verb, "move") || !strcmp(verb, "teleport"))
        locals[k].position = (ideatek_position_t){ a, b, c };
    if (!strcmp(verb, "turn"))
        locals[k].rotation = (ideatek_quat_t){ 0.0f,
            sinf(a * 3.14159265f / 360.0f), 0.0f,
            cosf(a * 3.14159265f / 360.0f) };
    if (!strcmp(verb, "scale"))
        locals[k].scale = (ideatek_vec3_t){ a, b, c };
    if (!strcmp(verb, "badmove"))
        locals[k].position.x = NAN;
    put_transform(api, k);
    if (!strcmp(verb, "teleport"))
        api->attach(api, nodes[k], game_space_slots.teleported, BRIDGE_ZEROED);
}

static void reparent(const bridge_api_t *api, char *args)
{
    unsigned k = 0;
    int p = -1;
    unsigned v[4] = { 0, 0, 0, 0 };

    sscanf(args, "%u,%d,%u,%u,%u,%u", &k, &p, &v[0], &v[1], &v[2], &v[3]);
    put_parent(api, k, p, v);
}

static void show_links(const bridge_api_t *api, uint32_t k)
{
    const ideatek_children_t *c = game_cell(api, nodes[k],
        game_space_slots.children);
    const ideatek_sibling_t *s = game_cell(api, nodes[k],
        game_space_slots.sibling);
    const ideatek_previous_world_transform_t *p = game_cell(api, nodes[k],
        game_space_slots.previous);

    if (c)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u children %u first %d "
            "last %d count %u", game.round, k, index_of(c->first.id),
            index_of(c->last.id), c->count);
    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u sibling %u prev %d "
            "next %d parent %d", game.round, k, index_of(s->prev.id),
            index_of(s->next.id), index_of(s->parent.id));
    if (p)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u previous %u at %.3f "
            "%.3f %.3f tick %llu", game.round, k, p->world.origin.x,
            p->world.origin.y, p->world.origin.z,
            (unsigned long long)p->tick);
}

static void show_space(const bridge_api_t *api)
{
    const ideatek_world_transform_t *w;

    for (uint32_t k = 0; k < node_count; k++) {
        w = game_cell(api, nodes[k], game_space_slots.world);
        if (w)
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u world %u at %.3f "
                "%.3f %.3f x %.3f %.3f %.3f rev %u", game.round, k,
                w->origin.x, w->origin.y, w->origin.z, w->basis[0],
                w->basis[1], w->basis[2], w->revision);
        else
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u world %u none%s",
                game.round, k, game_cell(api, nodes[k],
                game_space_slots.transform) ? "" : " and no transform");
        show_links(api, k);
    }
}

static bool step_more(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;
    uint32_t k = (uint32_t)atoi(args);

    if (!strcmp(verb, "nodes"))
        add_many(api, args, -1);
    else if (!strcmp(verb, "children"))
        add_many(api, args, (int)k);
    else if (!strcmp(verb, "kill"))
        api->destroy(api, nodes[k]);
    else if (!strcmp(verb, "untransform"))
        api->detach(api, nodes[k], game_space_slots.transform);
    else if (!strcmp(verb, "interp"))
        api->attach(api, nodes[k], game_space_slots.interpolated,
            BRIDGE_ZEROED);
    else if (!strcmp(verb, "uninterp"))
        api->detach(api, nodes[k], game_space_slots.interpolated);
    else if (!strcmp(verb, "show_space"))
        show_space(api);
    else
        done = false;
    return (done);
}

bool game_space_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "node"))
        add_node(api, args, false);
    else if (!strcmp(verb, "child"))
        add_node(api, args, true);
    else if (!strcmp(verb, "move") || !strcmp(verb, "turn")
        || !strcmp(verb, "scale") || !strcmp(verb, "teleport")
        || !strcmp(verb, "badmove"))
        change(api, verb, args);
    else if (!strcmp(verb, "reparent"))
        reparent(api, args);
    else
        done = step_more(api, verb, args);
    return (done);
}

void game_declare_space(const bridge_api_t *api)
{
    game_space_slots_t *s = &game_space_slots;

    TYPED(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    TYPED(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    TYPED(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_keep_t", ideatek_keep_t);
    ENUM(api, "ideatek:ideatek_orphan_t", ideatek_orphan_t);
    TYPED(api, "ideatek:ideatek_world_transform_t", ideatek_world_transform_t);
    s->transform = TYPED(api, "ideatek:Transform", ideatek_transform_t);
    s->parent = TYPED(api, "ideatek:Parent", ideatek_parent_t);
    s->world = TYPED(api, "ideatek:WorldTransform", ideatek_world_transform_t);
    s->children = BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek:Children",
        ideatek_children_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_KEEP);
    s->sibling = BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek:Sibling",
        ideatek_sibling_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_KEEP);
    s->previous = TYPED(api, "ideatek:PreviousWorldTransform",
        ideatek_previous_world_transform_t);
    s->interpolated = TAG(api, "ideatek:Interpolated");
    s->teleported = TAG(api, "ideatek:Teleported");
}

BRIDGE_NATIVE(seen_world)
{
    const ideatek_world_transform_t *w;

    for (uint32_t k = 0; k < node_count; k++) {
        w = game_cell(api, nodes[k], game_space_slots.world);
        if (!w || w->revision == revisions[k])
            continue;
        revisions[k] = w->revision;
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u placed %u at %.3f %.3f "
            "%.3f rev %u", game.round, k, w->origin.x, w->origin.y,
            w->origin.z, w->revision);
    }
    return (0);
}

void game_watch_space(const bridge_api_t *api)
{
    bridge_term_t world[1] = {{ game_space_slots.world, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    BRIDGE_NATIVE_ADD_SYSTEM(api, "seen_world", "check", world, 1, nullptr, 0,
        nullptr, 0);
}

#endif
