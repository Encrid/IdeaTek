/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game_files.h"

#if defined(GAME_FILES)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_files_slots_t game_files_slots;

static const char *const STORAGES[] = { "content", "user" };
static const char *const ACTIONS[] = { "read", "write", "list", "remove",
    "watch" };
static const char *const STATUSES[] = { "ok", "not_found", "too_big",
    "denied", "failed", "unsupported" };

static uint64_t entities[256];
static uint32_t entity_count;

static uint8_t index_of(const char *word, const char *const *words,
    uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
        if (!strcmp(word, words[i]))
            return (i);
    return ((uint8_t)atoi(word));
}

static bridge_text_t put_bytes(const bridge_api_t *api, const char *text)
{
    uint32_t len = (uint32_t)strlen(text);
    uint32_t at = api->alloc(api, len ? len : 1);

    memcpy(api->shared_base + at, text, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

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

static void ask_file(const bridge_api_t *api, char *args)
{
    ideatek_file_request_t r = {};
    char *left = args;
    const char *storage = field(&left);
    const char *action = field(&left);
    const char *path = field(&left);
    const char *priority = field(&left);
    const char *third = field(&left);
    const char *fourth = field(&left);
    uint64_t id = api->spawn(api);

    r.storage = index_of(storage, STORAGES, 2);
    r.action = index_of(action, ACTIONS, 5);
    r.path = put_bytes(api, path);
    r.priority = (uint8_t)atoi(priority);
    if (r.action == IDEATEK_FILE_ACTION_WRITE)
        r.data = put_bytes(api, third);
    if (r.action == IDEATEK_FILE_ACTION_READ) {
        r.offset = strtoull(third, nullptr, 0);
        r.length = strtoull(fourth, nullptr, 0);
    }
    entities[entity_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_files_slots.request, r);
}

static void show_text(char *out, size_t size, const bridge_api_t *api,
    bridge_text_t text)
{
    size_t len = text.len < size - 1 ? text.len : size - 1;

    memcpy(out, api->shared_base + text.at, len);
    out[len] = '\0';
    for (size_t i = 0; i < len; i++)
        if (out[i] == '\n')
            out[i] = '|';
}

static void show_results(const bridge_api_t *api)
{
    const ideatek_file_result_t *r;
    char data[512];
    uint32_t at;

    for (uint32_t k = 0; k < entity_count; k++) {
        at = bridge_native_find_value(api, entities[k], game_files_slots.result);
        if (!at)
            continue;
        r = (const void *)(api->shared_base + at);
        show_text(data, sizeof(data), api, r->data);
        bridge_native_say(api, BRIDGE_LOG_INFO,
            "r%u result %u %s size %llu [%s]", game.round, k,
            STATUSES[r->status % 6], (unsigned long long)r->size, data);
    }
}

static void show_changes(const bridge_api_t *api)
{
    const ideatek_file_change_t *c;
    char paths[512];
    uint32_t at;

    for (uint32_t k = 0; k < entity_count; k++) {
        at = bridge_native_find_value(api, entities[k], game_files_slots.change);
        if (!at)
            continue;
        c = (const void *)(api->shared_base + at);
        show_text(paths, sizeof(paths), api, c->paths);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u change %u [%s]",
            game.round, k, paths);
    }
}

bool game_files_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "filereq"))
        ask_file(api, args);
    else if (!strcmp(verb, "filedrop"))
        done = true;
    else if (!strcmp(verb, "show_results"))
        show_results(api);
    else if (!strcmp(verb, "show_changes"))
        show_changes(api);
    else if (!strcmp(verb, "rmpath"))
        SDL_RemovePath(args);
    else
        done = false;
    return (done);
}

void game_declare_files(const bridge_api_t *api)
{
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_storage_t", ideatek_storage_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_file_action_t",
        ideatek_file_action_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_file_status_t",
        ideatek_file_status_t);
    game_files_slots.request = TYPED(api, BRIDGE_STRUCT, "ideatek:FileRequest",
        ideatek_file_request_t);
    game_files_slots.result = TYPED(api, BRIDGE_STRUCT, "ideatek:FileResult",
        ideatek_file_result_t);
    game_files_slots.change = TYPED(api, BRIDGE_STRUCT, "ideatek:FileChange",
        ideatek_file_change_t);
}

static void drop_early(const bridge_api_t *api, uint32_t round)
{
    char script[sizeof(game.script)];
    char *left = nullptr;
    char *step;
    char *colon;

    memcpy(script, game.script, sizeof(script));
    for (step = strtok_r(script, "|", &left); step;
        step = strtok_r(nullptr, "|", &left)) {
        colon = strchr(step, ':');
        if (!colon || (uint32_t)atoi(step) != round
            || strncmp(colon + 1, "filedrop ", 9))
            continue;
        api->destroy(api, entities[atoi(colon + 10)]);
    }
}

BRIDGE_NATIVE(early)
{
    drop_early(api, game.round + 1);
    return (0);
}

static uint32_t seen_list;

BRIDGE_NATIVE(seen_result)
{
    const bridge_list_t *list = bridge_native_get_list(api, seen_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        for (uint32_t k = 0; k < entity_count; k++)
            if (entities[k] == t[i].id)
                bridge_native_say(api, BRIDGE_LOG_INFO, "r%u seen %u",
                    game.round + 1, k);
    return (0);
}

void game_watch_files(const bridge_api_t *api)
{
    bridge_term_t term[1] = {{ game_files_slots.result, BRIDGE_READ,
        BRIDGE_AND, 0 }};

    BRIDGE_NATIVE_ADD_SYSTEM(api, "early", "first", nullptr, 0, nullptr, 0,
        nullptr, 0);
    seen_list = BRIDGE_NATIVE_OBSERVER(api, "seen_result", BRIDGE_ON_ENTER,
        term, 1, nullptr, 0, nullptr, 0);
}

#endif
