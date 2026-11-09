/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_audio.h"

#if defined(GAME_AUDIO)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define SWEPT(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, name, \
    T, BRIDGE_SWEEP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define SHOW(api, name, slot) BRIDGE_NATIVE_ADD_SYSTEM(api, name, "check", \
    (&(bridge_term_t){ slot, BRIDGE_READ, BRIDGE_AND, 0 }), 1, nullptr, 0, \
    nullptr, 0)

game_audio_slots_t game_audio_slots;

static const char *const DIRECTIONS[] = { "playback", "recording" };
static const char *const STATUSES[] = { "running", "fell_back", "no_device",
    "suspended", "pending", "denied" };
static const char HINT[] = "IDEATEK_TEST_AUDIO";

static uint64_t entities[2];
static uint64_t seconds[2];
static uint64_t counts[2];
static uint32_t block;
static uint32_t queries[3];

static struct {
    uint64_t entity;
    char name[128];
} devices[32];
static uint32_t device_count;

static const char *field(char **left)
{
    char *start = *left;
    char *comma = strchr(start, ',');

    if (comma) {
        *comma = '\0';
        *left = comma + 1;
    } else
        *left = start + strlen(start);
    return (start);
}

static uint64_t device_of(const char *name)
{
    for (uint32_t i = 0; i < device_count; i++)
        if (!strcmp(devices[i].name, name))
            return (devices[i].entity);
    return (0);
}

static const char *name_of(uint64_t entity)
{
    for (uint32_t i = 0; i < device_count; i++)
        if (devices[i].entity == entity)
            return (devices[i].name);
    return ("none");
}

static void ask_audio(const bridge_api_t *api, char *args, bool second)
{
    ideatek_audio_request_t r = {};
    char *left = args;
    const char *direction = field(&left);
    const char *target = field(&left);
    const char *device = field(&left);
    uint32_t dir;
    uint64_t *slot;

    r.rate = (uint32_t)strtoul(field(&left), nullptr, 0);
    r.ahead = (uint32_t)strtoul(field(&left), nullptr, 0);
    r.channels = (uint8_t)atoi(field(&left));
    r.direction = direction[0] == 'r' ? IDEATEK_AUDIO_DIRECTION_RECORDING
        : (direction[0] == 'p' ? IDEATEK_AUDIO_DIRECTION_PLAYBACK
        : (ideatek_audio_direction_t)atoi(direction));
    r.target = target[0] == 'c' ? IDEATEK_AUDIO_TARGET_CHOSEN
        : (target[0] == 'd' ? IDEATEK_AUDIO_TARGET_DEFAULT
        : (ideatek_audio_target_t)atoi(target));
    r.device.id = device_of(device);
    dir = direction[0] == 'r';
    dir *= field(&left)[0] != 'o';
    slot = second ? &seconds[dir] : &entities[dir];
    if (!*slot)
        *slot = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, *slot, game_audio_slots.request, r);
}

static void put_frames(const bridge_api_t *api, char *args)
{
    ideatek_audio_frames_t f = {};
    char *left = args;
    uint32_t frames = (uint32_t)strtoul(field(&left), nullptr, 0);
    const char *at = field(&left);
    uint32_t channels = (uint32_t)atoi(field(&left));
    const char *shape = field(&left);
    const char *who = field(&left);
    float *samples;

    channels = channels ? channels : 2;
    if (!block)
        block = api->alloc(api, 32768);
    samples = (float *)(api->shared_base + block);
    for (uint32_t i = 0; i < frames * channels && i < 8192; i++)
        samples[i] = (float)((counts[0] + i / channels) % 100) / 100.0F;
    f.at = at[0] ? strtoull(at, nullptr, 0) : counts[0];
    f.frames = frames;
    f.data = (bridge_text_t){ .at = block, .len = frames * channels * 4 };
    f.data.len >>= shape[0] == 's';
    counts[0] += frames;
    BRIDGE_NATIVE_ATTACH(api, entities[who[0] == 'r'],
        game_audio_slots.frames, f);
}

static void show_played(const bridge_api_t *api)
{
    const char *path = SDL_getenv("IDEATEK_TEST_AUDIO_OUTPUT");
    size_t size = 0;
    float *samples = path ? SDL_LoadFile(path, &size) : nullptr;
    size_t count = size / sizeof(float);
    float most = 0.0F;

    for (size_t i = 0; i < count; i++)
        most = samples[i] > most ? samples[i] : most;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u played %zu samples "
        "first %.2f max %.2f", game.round, count, count ? samples[0] : -1.0,
        most);
    SDL_free(samples);
}

bool game_audio_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "audioreq"))
        ask_audio(api, args, false);
    else if (!strcmp(verb, "audiosecond"))
        ask_audio(api, args, true);
    else if (!strcmp(verb, "audiodrop")) {
        api->destroy(api, entities[args[0] == 'r']);
        entities[args[0] == 'r'] = 0;
    } else if (!strcmp(verb, "audioframes"))
        put_frames(api, args);
    else if (!strcmp(verb, "audiodevices"))
        SDL_SetHintWithPriority(HINT, args, SDL_HINT_OVERRIDE);
    else if (!strcmp(verb, "show_played"))
        show_played(api);
    else
        done = false;
    return (done);
}

void game_declare_audio(const bridge_api_t *api)
{
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_audio_direction_t",
        ideatek_audio_direction_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_audio_target_t",
        ideatek_audio_target_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_audio_status_t",
        ideatek_audio_status_t);
    game_audio_slots.device = SWEPT(api, "ideatek:AudioDevice",
        ideatek_audio_device_t);
    game_audio_slots.request = TYPED(api, BRIDGE_STRUCT,
        "ideatek:AudioRequest", ideatek_audio_request_t);
    game_audio_slots.frames = TYPED(api, BRIDGE_STRUCT, "ideatek:AudioFrames",
        ideatek_audio_frames_t);
    game_audio_slots.recorded = SWEPT(api, "ideatek:AudioRecorded",
        ideatek_audio_frames_t);
    game_audio_slots.state = SWEPT(api, "ideatek:AudioState",
        ideatek_audio_state_t);
}

static void show_device(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_audio_device_t *d = cell;

    if (device_count < 32) {
        devices[device_count].entity = id;
        memcpy(devices[device_count].name, d->name, 128);
        device_count++;
    }
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u device [%s] %s default %d",
        game.round, d->name, DIRECTIONS[d->direction & 1], d->is_default);
}

static void show_state(const bridge_api_t *api, uint64_t id, const void *cell)
{
    const ideatek_audio_state_t *s = cell;

    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u %s state device [%s] rate %u "
        "ch %u queued %u wanted %u sum %u dropped %llu %s", game.round,
        DIRECTIONS[id == entities[1]], name_of(s->device.id), s->rate,
        s->channels, s->queued, s->wanted, s->queued + s->wanted,
        (unsigned long long)s->dropped, STATUSES[s->status % 6]);
}

static void show_recorded(const bridge_api_t *api, uint64_t id,
    const void *cell)
{
    const ideatek_audio_frames_t *f = cell;
    const float *samples = (const float *)(api->shared_base + f->data.at);
    uint32_t count = f->data.len / 4;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u recorded at %llu frames %u "
        "first %.2f last %.2f", game.round, (unsigned long long)f->at,
        f->frames, count ? samples[0] : -1.0, count ? samples[count - 1]
        : -1.0);
}

BRIDGE_NATIVE(show_audio)
{
    device_count = 0;
    game_rows(api, queries[0], game_audio_slots.device, show_device);
    return (0);
}

BRIDGE_NATIVE(show_streams)
{
    game_rows(api, queries[1], game_audio_slots.state, show_state);
    return (0);
}

BRIDGE_NATIVE(show_recorded)
{
    game_rows(api, queries[2], game_audio_slots.recorded, show_recorded);
    return (0);
}

void game_watch_audio(const bridge_api_t *api)
{
    queries[0] = SHOW(api, "show_audio", game_audio_slots.device);
    queries[1] = SHOW(api, "show_streams", game_audio_slots.state);
    queries[2] = SHOW(api, "show_recorded", game_audio_slots.recorded);
}

#endif
