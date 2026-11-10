/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ktx2.h"
#include "game_resources.h"

#if defined(GAME_KTX2)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_ktx2_slots_t game_ktx2_slots;

static const uint8_t IDENTIFIER[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32,
    0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

typedef struct {
    uint64_t format, w, h, d, layers, faces, levels, scheme;
    uint64_t bx, by, bz, bpb;
    uint64_t cut, magic, vendor, dfdlen, dfdat, len0, unc0, at0, index;
    bool has_cut, has_dfdlen, has_dfdat, has_len0, has_unc0, has_at0;
    bool has_index;
} game_spec_t;

static uint32_t list[1];
static uint64_t own_texture;

static void put32(uint8_t *at, uint64_t value)
{
    for (int k = 0; k < 4; k++)
        at[k] = (uint8_t)(value >> (8 * k));
}

static void put64(uint8_t *at, uint64_t value)
{
    put32(at, value & 0xFFFFFFFFu);
    put32(at + 4, value >> 32);
}

static void set_key(game_spec_t *s, const char *key, uint64_t v)
{
    static const char *const NAMES[] = { "format", "w", "h", "d", "layers",
        "faces", "levels", "scheme", "bx", "by", "bz", "bpb", "cut", "magic",
        "vendor", "dfdlen", "dfdat", "len0", "unc0", "at0", "index" };
    uint64_t *fields[] = { &s->format, &s->w, &s->h, &s->d, &s->layers,
        &s->faces, &s->levels, &s->scheme, &s->bx, &s->by, &s->bz, &s->bpb,
        &s->cut, &s->magic, &s->vendor, &s->dfdlen, &s->dfdat, &s->len0,
        &s->unc0, &s->at0, &s->index };
    bool *flags[] = { &s->has_cut, nullptr, nullptr, &s->has_dfdlen,
        &s->has_dfdat, &s->has_len0, &s->has_unc0, &s->has_at0,
        &s->has_index };

    for (size_t k = 0; k < sizeof(NAMES) / sizeof(NAMES[0]); k++) {
        if (strcmp(key, NAMES[k]))
            continue;
        *fields[k] = v;
        if (k >= 12 && flags[k - 12])
            *flags[k - 12] = true;
    }
}

static void read_spec(char *text, game_spec_t *s)
{
    char *left = nullptr;
    char *equal;

    *s = (game_spec_t){ .format = 9, .w = 4, .h = 4, .faces = 1, .levels = 1,
        .bx = 1, .by = 1, .bz = 1, .bpb = 1, .magic = 1 };
    for (char *pair = strtok_r(text, ";", &left); pair;
        pair = strtok_r(nullptr, ";", &left)) {
        equal = strchr(pair, '=');
        if (!equal)
            continue;
        *equal = '\0';
        set_key(s, pair, strtoull(equal + 1, nullptr, 0));
    }
}

static uint64_t level_size(const game_spec_t *s, uint64_t k)
{
    uint64_t w = s->w >> k ? s->w >> k : 1;
    uint64_t h = s->h >> k ? s->h >> k : 1;
    uint64_t d = s->d >> k ? s->d >> k : 1;

    return (((w + s->bx - 1) / s->bx) * ((h + s->by - 1) / s->by)
        * ((d + s->bz - 1) / s->bz) * s->bpb * (s->layers ? s->layers : 1)
        * s->faces);
}

static void write_head(uint8_t *f, const game_spec_t *s, uint64_t count)
{
    const uint64_t values[] = { s->format, 0, s->w, s->h, s->d, s->layers,
        s->faces, s->levels, s->scheme };
    uint64_t dfd = 80 + 24 * count;

    memcpy(f, IDENTIFIER, sizeof(IDENTIFIER));
    f[0] = s->magic ? f[0] : 0;
    for (int k = 0; k < 9; k++)
        put32(f + 12 + 4 * k, values[k]);
    put32(f + 48, s->has_dfdat ? s->dfdat : dfd);
    put32(f + 52, s->has_dfdlen ? s->dfdlen : 28);
    put32(f + dfd, 28);
    put32(f + dfd + 4, s->vendor);
    put32(f + dfd + 8, 24u << 16 | 2u);
    f[dfd + 16] = (uint8_t)(s->bx - 1);
    f[dfd + 17] = (uint8_t)(s->by - 1);
    f[dfd + 18] = (uint8_t)(s->bz - 1);
    f[dfd + 20] = (uint8_t)s->bpb;
}

static uint64_t write_levels(uint8_t *f, const game_spec_t *s,
    uint64_t count, uint64_t at)
{
    uint64_t len;

    for (uint64_t k = count; k-- > 0;) {
        len = k == 0 && s->has_len0 ? s->len0 : level_size(s, k);
        put64(f + 80 + 24 * k, k == 0 && s->has_at0 ? s->at0 : at);
        put64(f + 80 + 24 * k + 8, len);
        put64(f + 80 + 24 * k + 16, k == 0 && s->has_unc0 ? s->unc0 : len);
        for (uint64_t i = 0; i < len; i++)
            f[at + i] = (uint8_t)(16 * (k + 1) + i);
        at += len;
    }
    return (at);
}

static void write_ktx2(const bridge_api_t *api, char *args)
{
    char *comma = strchr(args, ',');
    game_spec_t s;
    uint64_t count;
    uint64_t total = 0;
    uint8_t *f;
    uint64_t end;

    *comma = '\0';
    read_spec(comma + 1, &s);
    count = s.levels ? s.levels : 1;
    count = s.has_index ? s.index : count;
    for (uint64_t k = 0; k < count; k++)
        total += k == 0 && s.has_len0 ? s.len0 : level_size(&s, k);
    if (count > 64 || total > 4194304) {
        bridge_native_say(api, BRIDGE_LOG_INFO, "ktx2 %s is too big", args);
        return;
    }
    f = calloc(1, 80 + 24 * count + 28 + total);
    write_head(f, &s, count);
    end = write_levels(f, &s, count, 80 + 24 * count + 28);
    SDL_SaveFile(args, f, s.has_cut && s.cut < end ? s.cut : end);
    free(f);
}

static void spawn_own(const bridge_api_t *api, bool again)
{
    ideatek_texture_t t = { .format = 9, .width = 1, .height = 1,
        .depth_or_layers = 1, .mip_count = 1, .revision = again ? 2 : 1 };
    uint32_t at = api->alloc(api, 8);

    memset(api->shared_base + at, 0x5A, 8);
    t.bytes = (bridge_text_t){ at, 1 };
    if (!again)
        own_texture = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, own_texture, game_ktx2_slots.texture, t);
}

bool game_ktx2_step(const bridge_api_t *api, const char *verb, char *args)
{
    if (!strcmp(verb, "ktx2"))
        write_ktx2(api, args);
    else if (!strcmp(verb, "texture"))
        spawn_own(api, false);
    else if (!strcmp(verb, "retexture"))
        spawn_own(api, true);
    else if (!strcmp(verb, "untexture"))
        api->destroy(api, own_texture);
    else
        return (false);
    return (true);
}

void game_declare_ktx2(const bridge_api_t *api)
{
    game_ktx2_slots_t *s = &game_ktx2_slots;

    s->dimension = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_dimension_t",
        ideatek_dimension_t);
    s->filter = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_filter_t",
        ideatek_filter_t);
    s->wrap = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_wrap_t",
        ideatek_wrap_t);
    s->texture = TYPED(api, BRIDGE_STRUCT, "ideatek:Texture",
        ideatek_texture_t);
    s->sampler = TYPED(api, BRIDGE_STRUCT, "ideatek:Sampler",
        ideatek_sampler_t);
}

static void show_bytes(char *out, const bridge_api_t *api, bridge_text_t b)
{
    uint32_t len = b.len < 48 ? b.len : 48;

    for (uint32_t i = 0; i < len; i++)
        snprintf(out + 2 * i, 3, "%02x", api->shared_base[b.at + i]);
    out[2 * len] = '\0';
}

static void show_texture(const bridge_api_t *api, uint64_t id)
{
    const ideatek_texture_t *t = game_cell(api, id, game_ktx2_slots.texture);
    const ideatek_sampler_t *s = game_cell(api, id, game_ktx2_slots.sampler);
    const ideatek_resource_t *r = game_cell(api, id,
        game_resources_slots.resource);
    char bytes[100];
    int len = r ? (int)r->path.len : 0;
    const char *path = r ? (const char *)api->shared_base + r->path.at : "";

    show_bytes(bytes, api, t->bytes);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u texture [%.*s] format %u "
        "size %ux%ux%u mips %u top %u rev %u dim %u len %u bytes %s",
        game.round + 1, len, path, t->format, t->width, t->height,
        t->depth_or_layers, t->mip_count, t->top_mip, t->revision,
        t->dimension, t->bytes.len, bytes);
    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u sampler [%.*s] min %u "
            "mag %u mip %u wrap %u %u %u aniso %.2f", game.round + 1, len,
            path, s->min_filter, s->mag_filter, s->mip_filter, s->wrap_u,
            s->wrap_v, s->wrap_w, (double)s->max_anisotropy);
}

BRIDGE_NATIVE(seen_texture)
{
    const bridge_list_t *l = bridge_native_get_list(api, list[0]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        show_texture(api, t[i].id);
    return (0);
}

void game_watch_ktx2(const bridge_api_t *api)
{
    bridge_term_t term[1] = {{ game_ktx2_slots.texture, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    list[0] = BRIDGE_NATIVE_OBSERVER(api, "seen_texture",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, term, 1, nullptr, 0, nullptr, 0);
}

#endif
