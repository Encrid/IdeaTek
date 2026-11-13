/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_render.h"

#if defined(GAME_RENDER)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_render_slots_t game_render_slots;

static const char *const STATUS[] = { "ok", "no_target", "too_big",
    "failed" };

static uint64_t cameras[128];
static uint32_t camera_count;
static uint64_t requests[64];
static uint32_t request_count;
static uint32_t seen_list;

static void add_camera(const bridge_api_t *api, const char *args)
{
    ideatek_camera_t c = { .target = { game.window }, .groups = UINT64_MAX,
        .active = true };
    ideatek_projection_t p = { .fov = 1.0f, .near = 0.1f, .far = 100.0f,
        .kind = IDEATEK_PROJECTION_KIND_PERSPECTIVE };
    int active = 1;
    int other = 0;
    uint64_t id = api->spawn(api);

    sscanf(args, "%d,%f,%f,%f,%f,%d,%d", &c.order, &c.viewport.x,
        &c.viewport.y, &c.viewport.width, &c.viewport.height, &active, &other);
    c.active = active;
    c.target.id += (uint64_t)other * 1000000;
    cameras[camera_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_render_slots.camera, c);
    BRIDGE_NATIVE_ATTACH(api, id, game_render_slots.projection, p);
}

static void ask_capture(const bridge_api_t *api, const char *args,
    uint64_t id)
{
    ideatek_capture_request_t r = { .target = { game.window } };
    int other = 0;

    sscanf(args, "%f,%f,%f,%f,%d", &r.area.x, &r.area.y, &r.area.width,
        &r.area.height, &other);
    r.target.id += (uint64_t)other * 1000000;
    BRIDGE_NATIVE_ATTACH(api, id, game_render_slots.request, r);
}

static void show_capture(const bridge_api_t *api, uint32_t k)
{
    uint32_t at = bridge_native_find_value(api, requests[k],
        game_render_slots.capture);
    const ideatek_capture_t *c = (const void *)(api->shared_base + at);
    const uint8_t *p = api->shared_base + c->pixels.at;
    uint32_t mid = (c->height / 2 * c->width + c->width / 2) * 4;

    if (c->status != IDEATEK_CAPTURE_STATUS_OK)
        p = (const uint8_t *)"\0\0\0\0";
    else
        p += mid;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u capture %u %s %ux%u round "
        "%llu pixel %u %u %u %u", game.round, k, STATUS[c->status % 4],
        c->width, c->height, (unsigned long long)c->round, p[0], p[1], p[2],
        p[3]);
}

BRIDGE_NATIVE(seen_capture)
{
    const bridge_list_t *list = bridge_native_get_list(api, seen_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        for (uint32_t k = 0; k < request_count; k++)
            if (requests[k] == t[i].id)
                show_capture(api, k);
    return (0);
}

bool game_render_step(const bridge_api_t *api, const char *verb, char *args)
{
    uint32_t k = (uint32_t)atoi(args);
    bool done = true;

    if (!strcmp(verb, "camera"))
        add_camera(api, args);
    else if (!strcmp(verb, "cameras"))
        for (uint32_t i = 0; i < k; i++)
            add_camera(api, "0,0,0,0,0,1");
    else if (!strcmp(verb, "capture")) {
        requests[request_count] = api->spawn(api);
        ask_capture(api, args, requests[request_count++]);
    } else if (!strcmp(verb, "recapture"))
        ask_capture(api, strchr(args, ',') + 1, requests[k]);
    else if (!strcmp(verb, "uncapture"))
        api->detach(api, requests[k], game_render_slots.request);
    else if (!strcmp(verb, "uncamera"))
        api->detach(api, cameras[k], game_render_slots.camera);
    else if (!strcmp(verb, "recamera"))
        BRIDGE_NATIVE_ATTACH(api, cameras[k], game_render_slots.camera,
            ((ideatek_camera_t){ .target = { game.window },
            .groups = UINT64_MAX, .active = true }));
    else
        done = false;
    return (done);
}

void game_declare_render(const bridge_api_t *api)
{
    game_render_slots_t *s = &game_render_slots;

    TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_rect_t", ideatek_rect_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_projection_kind_t",
        ideatek_projection_kind_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_capture_status_t",
        ideatek_capture_status_t);
    s->camera = TYPED(api, BRIDGE_STRUCT, "ideatek:Camera", ideatek_camera_t);
    s->projection = TYPED(api, BRIDGE_STRUCT, "ideatek:Projection",
        ideatek_projection_t);
    s->request = TYPED(api, BRIDGE_STRUCT, "ideatek:CaptureRequest",
        ideatek_capture_request_t);
    s->capture = TYPED(api, BRIDGE_STRUCT, "ideatek:Capture",
        ideatek_capture_t);
    s->stats = BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek:RenderStats",
        ideatek_render_stats_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

void game_watch_render(const bridge_api_t *api)
{
    bridge_term_t term[1] = {{ game_render_slots.capture, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    seen_list = BRIDGE_NATIVE_OBSERVER(api, "seen_capture",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, term, 1, nullptr, 0, nullptr, 0);
}

#endif
