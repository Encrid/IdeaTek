/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "game.h"

#include "game_files.h"
#include "game_frame.h"
#include "game_gpu.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

game_t game;

static uint32_t queries[6];
static uint32_t lists[2];


static const char *const MODES[] = { "windowed", "fullscreen", "exclusive" };
static const char *const SURFACES[] = { "none", "windows", "metal", "xlib",
    "xcb", "wayland", "android", "web" };

const void *game_cell(const bridge_api_t *api, uint64_t id, uint32_t slot)
{
    uint32_t at = bridge_native_find_value(api, id, slot);

    return (at ? api->shared_base + at : nullptr);
}

uint64_t game_find_display(const char *name)
{
    for (uint32_t i = 0; i < game.display_count; i++)
        if (!strcmp(game.displays[i].name, name))
            return (game.displays[i].entity);
    return (0);
}

static const char *name_of(const bridge_api_t *api, uint64_t id)
{
    const ideatek_display_t *d = game_cell(api, id, game.slots.display);

    return (d ? d->name : "none");
}

static long order_of(const bridge_api_t *api, uint64_t id)
{
    const ideatek_video_mode_t *m = game_cell(api, id, game.slots.video_mode);

    return (m ? (long)m->order : -1);
}

static void say_text(const bridge_api_t *api, const char *what,
    bridge_text_t text, uint32_t *first)
{
    if (!*first)
        *first = text.at;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u %s [%.*s] at %+lld",
        game.round, what, (int)text.len,
        (const char *)api->shared_base + text.at,
        (long long)text.at - (long long)*first);
}

static void show_state(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_window_state_t *s = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u state %s %s %ux%u %ux%u "
        "scale %.2f hdr %d %.2f %.2f focused %d minimized %d close %d over %d "
        "at %.0f %.0f display %s video %ld handles %d%d same %d", game.round,
        MODES[s->mode], SURFACES[s->surface], s->width, s->height,
        s->pixel_width, s->pixel_height, s->display_scale, s->hdr,
        s->sdr_white_level, s->hdr_headroom, s->focused, s->minimized,
        s->close_asked, s->drop_over, s->drop_x, s->drop_y,
        name_of(api, s->display.id), order_of(api, s->video_mode.id),
        s->native_display != 0, s->native_window != 0, id == game.window);
}

static void show_display(const bridge_api_t *api, uint64_t id,
    const void *cell)
{
    const ideatek_display_t *d = cell;

    if (game.display_count < 32) {
        game.displays[game.display_count].entity = id;
        memcpy(game.displays[game.display_count].name, d->name, 128);
        game.display_count++;
    }
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u display [%s] %ux%u scale "
        "%.2f hdr %d primary %d desktop %ld current %ld", game.round, d->name,
        d->width, d->height, d->content_scale, d->hdr, d->is_primary,
        order_of(api, d->desktop_mode.id), order_of(api, d->current_mode.id));
}

static void show_mode(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_video_mode_t *m = cell;

    if (game.mode_count < 1024)
        game.modes[game.mode_count++] = (game_mode_t){ m->display.id, id,
            m->order };
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u mode [%s] %u %ux%u %d/%d "
        "%.2f", game.round, name_of(api, m->display.id), m->order, m->width,
        m->height, m->refresh_numerator, m->refresh_denominator,
        m->pixel_density);
}

static void show_drop(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_drop_t *d = cell;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u drop %u.%u %s window %d "
        "at %.0f %.0f", game.round, d->set, d->index,
        d->kind == IDEATEK_DROP_FILE ? "file" : "text",
        d->window.id == game.window && d->window.id, d->x, d->y);
    say_text(api, "drop data", d->data, &game.first_drop);
    say_text(api, "drop source", d->source, &game.first_drop);
}

static void show_paste(const bridge_api_t *api, uint64_t id, const void *cell)
{
    (void)id;
    say_text(api, "paste", ((const ideatek_paste_t *)cell)->text,
        &game.first_paste);
}


uint32_t game_rows(const bridge_api_t *api, uint32_t query,
    uint32_t slot, game_show_t show)
{
    const bridge_query_t *q = bridge_native_get_query(api, query);
    const uint32_t *bases = (const uint32_t *)(api->private_base
        + bridge_native_get_header(api)->column_base);
    const uint32_t *strides = (const uint32_t *)(api->private_base
        + bridge_native_get_header(api)->stride);
    uint32_t types = bridge_native_get_header(api)->types;
    uint32_t count = 0;

    for (uint32_t s = 0; s < q->spans; s++) {
        const uint32_t *span = bridge_native_get_span(api, q, s, 1);
        const uint64_t *who = bridge_native_get_block(api, span);
        uint32_t base = bases[span[0] * types + slot];
        for (uint32_t r = 0; r < bridge_native_get_rows(api, span); r++)
            show(api, who[r], api->shared_base + base + r * strides[slot]);
        count += bridge_native_get_rows(api, span);
    }
    return (count);
}

BRIDGE_NATIVE(play)
{
    game_play(api);
    return (0);
}

BRIDGE_NATIVE(show_state)
{
    game_rows(api, queries[0], game.slots.state, show_state);
    return (0);
}

BRIDGE_NATIVE(show_displays)
{
    game.display_count = 0;
    game_rows(api, queries[1], game.slots.display, show_display);
    return (0);
}

BRIDGE_NATIVE(show_modes)
{
    game.mode_count = 0;
    game_rows(api, queries[2], game.slots.video_mode, show_mode);
    return (0);
}

BRIDGE_NATIVE(show_drops)
{
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u drops %u", game.round,
        game_rows(api, queries[3], game.slots.drop, show_drop));
    return (0);
}

BRIDGE_NATIVE(show_pastes)
{
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u pastes %u", game.round,
        game_rows(api, queries[4], game.slots.paste, show_paste));
    return (0);
}

static void see(const bridge_api_t *api, uint32_t list, const char *what)
{
    const bridge_list_t *l = bridge_native_get_list(api, list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u attached %s %s",
            game.round + 1, what, what[0] == 'd'
            ? name_of(api, t[i].id) : "window");
}

BRIDGE_NATIVE(seen_state)
{
    see(api, lists[0], "state");
    return (0);
}

BRIDGE_NATIVE(seen_display)
{
    see(api, lists[1], "display");
    return (0);
}

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define SWEPT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

static void declare_types(const bridge_api_t *api)
{
    game_slots_t *s = &game.slots;

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_window_mode_t", ideatek_window_mode_t);
    ENUM(api, "ideatek:ideatek_display_target_t", ideatek_display_target_t);
    ENUM(api, "ideatek:ideatek_surface_t", ideatek_surface_t);
    ENUM(api, "ideatek:ideatek_drop_kind_t", ideatek_drop_kind_t);
    s->request = STRUCT(api, "ideatek:WindowRequest", ideatek_window_request_t);
    s->state = STRUCT(api, "ideatek:WindowState", ideatek_window_state_t);
    s->display = SWEPT(api, "ideatek:Display", ideatek_display_t);
    s->video_mode = SWEPT(api, "ideatek:VideoMode", ideatek_video_mode_t);
    s->drop = SWEPT(api, "ideatek:Drop", ideatek_drop_t);
    s->clipboard = STRUCT(api, "ideatek:ClipboardRequest",
        ideatek_clipboard_request_t);
    s->paste = SWEPT(api, "ideatek:Paste", ideatek_paste_t);
    s->event_wait = STRUCT(api, "ideatek:EventWait", ideatek_event_wait_t);
    game_declare_input(api);
#if defined(GAME_GPU)
    game_declare_gpu(api);
#endif
#if defined(GAME_FRAME)
    game_declare_frame(api);
#endif
#if defined(GAME_FILES)
    game_declare_files(api);
#endif
    s->settings = BRIDGE_NATIVE_COMPONENT(api, "test:Settings",
        game_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

#define ADD_SHOW(api, name, slot) BRIDGE_NATIVE_ADD_SYSTEM(api, name,     "check", (&(bridge_term_t){ slot, BRIDGE_READ, BRIDGE_AND, 0 }), 1,     nullptr, 0, nullptr, 0)
#define ADD_SEEN(api, name, slot) BRIDGE_NATIVE_OBSERVER(api, name,     BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH,     (&(bridge_term_t){ slot, BRIDGE_READ, BRIDGE_AND, 0 }), 1, nullptr, 0,     nullptr, 0)

BRIDGE_NATIVE(init)
{
    uint32_t changes[11];
    uint32_t count = 6;

    declare_types(api);
    changes[0] = game.slots.request;
    changes[1] = game.slots.clipboard;
    changes[2] = game.slots.event_wait;
    changes[3] = game.input.text_request;
    changes[4] = game.input.mouse_request;
    changes[5] = game.input.gamepad_request;
#if defined(GAME_GPU)
    changes[6] = game_gpu_slots.request;
    changes[7] = game_gpu_slots.swapchain_request;
    changes[8] = game.slots.state;
    count = 9;
#endif
#if defined(GAME_FRAME)
    changes[count] = game_frame_slots.request;
    count++;
#endif
#if defined(GAME_FILES)
    changes[count] = game_files_slots.request;
    count++;
#endif
    BRIDGE_NATIVE_ADD_SYSTEM(api, "play", "check", nullptr, 0, changes, count,
        nullptr, 0);
    queries[0] = ADD_SHOW(api, "show_state", game.slots.state);
    queries[1] = ADD_SHOW(api, "show_displays", game.slots.display);
    queries[2] = ADD_SHOW(api, "show_modes", game.slots.video_mode);
    queries[3] = ADD_SHOW(api, "show_drops", game.slots.drop);
    queries[4] = ADD_SHOW(api, "show_pastes", game.slots.paste);
    lists[0] = ADD_SEEN(api, "seen_state", game.slots.state);
    lists[1] = ADD_SEEN(api, "seen_display", game.slots.display);
    game_watch_input(api);
#if defined(GAME_GPU)
    game_watch_gpu(api);
#endif
#if defined(GAME_FRAME)
    game_watch_frame(api);
#endif
#if defined(GAME_FILES)
    game_watch_files(api);
#endif
#if defined(GAME_EARLY)
    game_start(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
