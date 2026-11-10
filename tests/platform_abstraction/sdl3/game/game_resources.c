/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_resources.h"

#if defined(GAME_RESOURCES)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAGGED(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_resources_slots_t game_resources_slots;

typedef struct {
    char path[128];
    uint8_t error;
    char labels[128];
} game_answer_t;

typedef struct {
    char path[128];
    char data[256];
} game_fake_file_t;

typedef struct {
    uint64_t entity;
    uint64_t resource;
    char label[64];
} game_sub_t;

static uint64_t uses[64];
static uint32_t use_count;
static uint64_t loaders[8];
static uint32_t loader_count;
static uint64_t others[16];
static uint32_t other_count;
static game_answer_t answers[16];
static uint32_t answer_count;
static game_fake_file_t fakes[16];
static uint32_t fake_count;
static game_sub_t subs[64];
static uint32_t sub_count;
static uint32_t lists[8];

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

static void show_text(char *out, size_t size, const bridge_api_t *api,
    bridge_text_t text)
{
    size_t len = text.len < size - 1 ? text.len : size - 1;

    memcpy(out, api->shared_base + text.at, len);
    out[len] = '\0';
    for (size_t i = 0; i < len; i++) {
        out[i] = out[i] == '\n' ? '|' : out[i];
        out[i] = out[i] == '{' ? '<' : out[i];
        out[i] = out[i] == '}' ? '>' : out[i];
    }
}

static uint64_t resource_of_use(const bridge_api_t *api, uint32_t k)
{
    const ideatek_resource_handle_t *h = game_cell(api, uses[k],
        game_resources_slots.handle);

    return (h ? h->resource.id : 0);
}

static uint64_t entity_of(const bridge_api_t *api, const char *word)
{
    uint32_t k = (uint32_t)atoi(word + 1);

    if (word[0] == 'r')
        return (resource_of_use(api, k));
    if (word[0] == 'u')
        return (uses[k]);
    if (word[0] == 'o')
        return (others[k]);
    return (0);
}

static void add_loader(const bridge_api_t *api, char *args)
{
    char *left = args;
    ideatek_resource_loader_t l = {};
    uint64_t id = api->spawn(api);
    char lines[256];
    size_t n;

    l.name = put_bytes(api, field(&left));
    l.type = put_bytes(api, field(&left));
    n = strlen(strcpy(lines, field(&left)));
    for (size_t i = 0; i < n; i++)
        lines[i] = lines[i] == ';' ? '\n' : lines[i];
    if (n)
        strcat(lines, "\n");
    l.extensions = put_bytes(api, lines);
    l.magic = put_bytes(api, field(&left));
    l.ranged = atoi(field(&left)) != 0;
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.loader, l);
    loaders[loader_count++] = id;
}

void game_init_loader(const bridge_api_t *api)
{
    char args[] = "early,test:Text,txt,,0";

    add_loader(api, args);
}

static void add_source(const bridge_api_t *api, const char *name)
{
    ideatek_resource_source_t s = { .name = put_bytes(api, name) };
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.source, s);
    others[other_count++] = id;
}

static void ask_load(const bridge_api_t *api, char *args)
{
    char *left = args;
    ideatek_resource_load_t l = {};
    uint64_t id = api->spawn(api);
    const char *owner = field(&left);

    l.owner.id = strcmp(owner, "-") ? entity_of(api, owner) : 0;
    for (char *c = left; *c; c++) {
        *c = *c == '<' ? '{' : *c;
        *c = *c == '>' ? '}' : *c;
    }
    l.path = put_bytes(api, field(&left));
    l.type = put_bytes(api, field(&left));
    l.settings = put_bytes(api, field(&left));
    l.reload_owner = (uint8_t)atoi(field(&left));
    l.priority = (uint8_t)atoi(field(&left));
    uses[use_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.load, l);
}

static void ask_add(const bridge_api_t *api, const char *type)
{
    uint64_t resource = api->spawn(api);
    uint64_t id = api->spawn(api);
    game_text_t text = { .bytes = 7 };
    ideatek_resource_add_t a = { .resource = { resource },
        .type = put_bytes(api, type) };

    BRIDGE_NATIVE_ATTACH(api, resource, game_resources_slots.text, text);
    uses[use_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.add, a);
}

static void ask_again(const bridge_api_t *api, const char *args)
{
    uint64_t id = api->spawn(api);
    ideatek_resource_add_t a = { .resource = { resource_of_use(api,
        (uint32_t)atoi(args)) }, .type = put_bytes(api, "test:Text") };

    uses[use_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.add, a);
}

static void stray(const bridge_api_t *api, const char *kind)
{
    const game_resources_slots_t *s = &game_resources_slots;
    uint64_t id = api->spawn(api);
    ideatek_resource_outcome_t o = {};
    ideatek_resource_read_t r = { .length = 1 };
    ideatek_resource_label_t l = { .label = put_bytes(api, "x") };
    ideatek_resource_source_result_t a = {};
    game_text_t text = { .bytes = 1 };

    BRIDGE_NATIVE_ATTACH(api, id, s->text, text);
    if (!strcmp(kind, "outcome"))
        BRIDGE_NATIVE_ATTACH(api, id, s->outcome, o);
    if (!strcmp(kind, "read"))
        BRIDGE_NATIVE_ATTACH(api, id, s->read, r);
    if (!strcmp(kind, "label"))
        BRIDGE_NATIVE_ATTACH(api, id, s->label, l);
    if (!strcmp(kind, "reload"))
        api->attach(api, id, s->reload, BRIDGE_ZEROED);
    if (!strcmp(kind, "answer"))
        BRIDGE_NATIVE_ATTACH(api, id, s->source_result, a);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u stray %s", game.round, kind);
}

static void ask_range(const bridge_api_t *api, char *args)
{
    char *left = args;
    uint32_t k = (uint32_t)atoi(field(&left));
    ideatek_resource_read_t r = {};
    uint64_t resource = resource_of_use(api, k);

    r.offset = strtoull(field(&left), nullptr, 0);
    r.length = strtoull(field(&left), nullptr, 0);
    r.priority = (uint8_t)atoi(field(&left));
    BRIDGE_NATIVE_ATTACH(api, resource, game_resources_slots.read, r);
}

static void set_answer(char *args, bool labels)
{
    char *left = args;
    const char *path = field(&left);
    game_answer_t *a = nullptr;

    for (uint32_t i = 0; i < answer_count; i++)
        a = !strcmp(answers[i].path, path) ? &answers[i] : a;
    if (!a) {
        a = &answers[answer_count++];
        strcpy(a->path, path);
    }
    if (labels)
        strcpy(a->labels, field(&left));
    else
        a->error = (uint8_t)atoi(field(&left));
}

static void set_fake(char *args)
{
    char *left = args;
    game_fake_file_t *f = &fakes[fake_count++];

    strcpy(f->path, field(&left));
    strcpy(f->data, left);
}

static void add_variable(const bridge_api_t *api, char *args, bool again)
{
    char *left = args;
    const char *name = field(&left);
    ideatek_resource_variable_t v = { .name = put_bytes(api, name) };
    uint64_t id = again ? 0 : api->spawn(api);
    const ideatek_resource_variable_t *old;
    char own[64];

    v.value = put_bytes(api, field(&left));
    v.fallback = put_bytes(api, field(&left));
    for (uint32_t i = 0; i < other_count && again; i++) {
        old = game_cell(api, others[i], game_resources_slots.variable);
        if (!old)
            continue;
        show_text(own, sizeof(own), api, old->name);
        id = !strcmp(own, name) ? others[i] : id;
    }
    if (!again)
        others[other_count++] = id;
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.variable, v);
}

static void show_resources(const bridge_api_t *api)
{
    const ideatek_resource_t *r;
    uint64_t resource;
    char path[128];
    char type[64];

    for (uint32_t k = 0; k < use_count; k++) {
        resource = resource_of_use(api, k);
        r = resource ? game_cell(api, resource, game_resources_slots.resource)
            : nullptr;
        if (!r) {
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u use %u %s", game.round,
                k, resource ? "no view" : "no handle");
            continue;
        }
        show_text(path, sizeof(path), api, r->path);
        show_text(type, sizeof(type), api, r->type);
        bridge_native_say(api, BRIDGE_LOG_INFO,
            "r%u use %u path [%s] type [%s] state %u deps %u rec %u error %u",
            game.round, k, path, type, r->state, r->dependencies,
            r->recursive, r->error);
    }
}

static void show_labels(const bridge_api_t *api)
{
    const ideatek_resource_t *r;
    char path[128];

    for (uint32_t j = 0; j < sub_count; j++) {
        r = game_cell(api, subs[j].entity, game_resources_slots.resource);
        if (!r) {
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u sub %u gone",
                game.round, j);
            continue;
        }
        show_text(path, sizeof(path), api, r->path);
        bridge_native_say(api, BRIDGE_LOG_INFO,
            "r%u sub %u path [%s] state %u error %u", game.round, j, path,
            r->state, r->error);
    }
}

static void add_other(const bridge_api_t *api)
{
    game_text_t text = { .bytes = 1 };
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.text, text);
    others[other_count++] = id;
}

static bool step_more(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "answer"))
        set_answer(args, false);
    else if (!strcmp(verb, "labels"))
        set_answer(args, true);
    else if (!strcmp(verb, "sourcefile"))
        set_fake(args);
    else if (!strcmp(verb, "variable"))
        add_variable(api, args, false);
    else if (!strcmp(verb, "varset"))
        add_variable(api, args, true);
    else if (!strcmp(verb, "reload"))
        api->attach(api, resource_of_use(api, (uint32_t)atoi(args)),
            game_resources_slots.reload, BRIDGE_ZEROED);
    else if (!strcmp(verb, "show_resources"))
        show_resources(api);
    else if (!strcmp(verb, "show_labels"))
        show_labels(api);
    else
        done = false;
    return (done);
}

bool game_resources_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "loader"))
        add_loader(api, args);
    else if (!strcmp(verb, "source"))
        add_source(api, args);
    else if (!strcmp(verb, "load"))
        ask_load(api, args);
    else if (!strcmp(verb, "add"))
        ask_add(api, args);
    else if (!strcmp(verb, "other"))
        add_other(api);
    else if (!strcmp(verb, "otherkill"))
        api->destroy(api, others[atoi(args)]);
    else if (!strcmp(verb, "usekill"))
        api->destroy(api, uses[atoi(args)]);
    else if (!strcmp(verb, "loaderkill"))
        api->destroy(api, loaders[atoi(args)]);
    else if (!strcmp(verb, "rangereq"))
        ask_range(api, args);
    else if (!strcmp(verb, "addagain"))
        ask_again(api, args);
    else if (!strcmp(verb, "stray"))
        stray(api, args);
    else
        done = step_more(api, verb, args);
    return (done);
}

void game_declare_resources(const bridge_api_t *api)
{
    game_resources_slots_t *s = &game_resources_slots;

    s->load_state = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_load_state_t",
        ideatek_load_state_t);
    s->error = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_resource_error_t",
        ideatek_resource_error_t);
    s->load = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceLoad",
        ideatek_resource_load_t);
    s->add = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceAdd",
        ideatek_resource_add_t);
    s->handle = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceHandle",
        ideatek_resource_handle_t);
    s->resource = TYPED(api, BRIDGE_STRUCT, "ideatek:Resource",
        ideatek_resource_t);
    s->loader = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceLoader",
        ideatek_resource_loader_t);
    s->bytes = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t);
    s->read = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceRead",
        ideatek_resource_read_t);
    s->outcome = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t);
    s->label = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceLabel",
        ideatek_resource_label_t);
    s->raw = TYPED(api, BRIDGE_STRUCT, "ideatek:RawBytes",
        ideatek_raw_bytes_t);
    s->source = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceSource",
        ideatek_resource_source_t);
    s->source_read = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceSourceRead",
        ideatek_resource_source_read_t);
    s->source_result = TYPED(api, BRIDGE_STRUCT,
        "ideatek:ResourceSourceResult", ideatek_resource_source_result_t);
    s->reload = TAGGED(api, "ideatek:ResourceReload");
    s->modified = TAGGED(api, "ideatek:ResourceModified");
    s->ready = TAGGED(api, "ideatek:ResourceReady");
    s->failed = TAGGED(api, "ideatek:ResourceFailed");
    s->variable = TYPED(api, BRIDGE_STRUCT, "ideatek:ResourceVariable",
        ideatek_resource_variable_t);
    s->text = TYPED(api, BRIDGE_STRUCT, "test:Text", game_text_t);
}

static const char *path_of(const bridge_api_t *api, uint64_t resource,
    char *out, size_t size)
{
    const ideatek_resource_t *r = game_cell(api, resource,
        game_resources_slots.resource);

    if (r)
        show_text(out, size, api, r->path);
    else
        strcpy(out, "?");
    return (out);
}

static void spawn_label(const bridge_api_t *api, uint64_t resource,
    const char *label)
{
    game_sub_t *sub = nullptr;
    ideatek_resource_label_t l = { .resource = { resource },
        .label = put_bytes(api, label) };
    game_text_t text = { .bytes = 3 };

    for (uint32_t j = 0; j < sub_count; j++)
        sub = subs[j].resource == resource && !strcmp(subs[j].label, label)
            ? &subs[j] : sub;
    if (!sub) {
        sub = &subs[sub_count++];
        sub->entity = api->spawn(api);
        sub->resource = resource;
        strcpy(sub->label, label);
        BRIDGE_NATIVE_ATTACH(api, sub->entity, game_resources_slots.text,
            text);
    }
    BRIDGE_NATIVE_ATTACH(api, sub->entity, game_resources_slots.label, l);
}

static bool is_own_loader(uint64_t loader)
{
    for (uint32_t i = 0; i < loader_count; i++)
        if (loaders[i] == loader)
            return (true);
    return (false);
}

static void answer_bytes(const bridge_api_t *api, uint64_t resource,
    const ideatek_resource_bytes_t *b, const char *path)
{
    game_text_t text = { .bytes = b->data.len };
    ideatek_resource_outcome_t o = { .error = IDEATEK_RESOURCE_ERROR_NONE };
    char labels[128] = "";
    char *left = nullptr;

    for (uint32_t i = 0; i < answer_count; i++) {
        if (strcmp(answers[i].path, path))
            continue;
        o.error = (ideatek_resource_error_t)answers[i].error;
        strcpy(labels, answers[i].labels);
    }
    BRIDGE_NATIVE_ATTACH(api, resource, game_resources_slots.text, text);
    for (char *label = strtok_r(labels, ";", &left); label;
        label = strtok_r(nullptr, ";", &left))
        spawn_label(api, resource, label);
    BRIDGE_NATIVE_ATTACH(api, resource, game_resources_slots.outcome, o);
}

BRIDGE_NATIVE(seen_bytes)
{
    const bridge_list_t *list = bridge_native_get_list(api, lists[0]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_resource_bytes_t *b;
    char path[128];
    char data[128];
    char settings[128];

    for (uint32_t i = 0; i < list->count; i++) {
        b = game_cell(api, t[i].id, game_resources_slots.bytes);
        if (!b)
            continue;
        path_of(api, t[i].id, path, sizeof(path));
        show_text(data, sizeof(data), api, b->data);
        show_text(settings, sizeof(settings), api, b->settings);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u bytes [%s] offset %llu "
            "size %llu len %u [%s] settings [%s] error %u", game.round + 1,
            path, (unsigned long long)b->offset, (unsigned long long)b->size,
            b->data.len, data, settings, b->error);
        if (is_own_loader(b->loader.id))
            answer_bytes(api, t[i].id, b, path);
    }
    return (0);
}

static void answer_source(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_source_read_t *q, const char *path)
{
    ideatek_resource_source_result_t r = { .error
        = IDEATEK_RESOURCE_ERROR_NOT_FOUND };
    const game_fake_file_t *f = nullptr;
    char data[256] = "";
    size_t size;
    size_t from;
    size_t len;

    for (uint32_t i = 0; i < fake_count; i++)
        f = !strcmp(fakes[i].path, path) ? &fakes[i] : f;
    if (f) {
        size = strlen(f->data);
        from = q->offset < size ? (size_t)q->offset : size;
        len = q->length ? (size_t)q->length : size - from;
        len = from + len > size ? size - from : len;
        memcpy(data, f->data + from, len);
        data[len] = '\0';
        r.size = size;
        r.error = IDEATEK_RESOURCE_ERROR_NONE;
    }
    r.data = put_bytes(api, data);
    BRIDGE_NATIVE_ATTACH(api, id, game_resources_slots.source_result, r);
}

BRIDGE_NATIVE(seen_source_read)
{
    const bridge_list_t *list = bridge_native_get_list(api, lists[1]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    const ideatek_resource_source_read_t *q;
    char path[128];

    for (uint32_t i = 0; i < list->count; i++) {
        q = game_cell(api, t[i].id, game_resources_slots.source_read);
        if (!q)
            continue;
        show_text(path, sizeof(path), api, q->path);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u sourceread [%s] offset "
            "%llu length %llu prio %u", game.round + 1, path,
            (unsigned long long)q->offset, (unsigned long long)q->length,
            q->priority);
        answer_source(api, t[i].id, q, path);
    }
    return (0);
}

static void say_tag(const bridge_api_t *api, uint32_t list, const char *what)
{
    const bridge_list_t *l = bridge_native_get_list(api, list);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    char path[128];

    for (uint32_t i = 0; i < l->count; i++)
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u %s [%s]", game.round + 1,
            what, path_of(api, t[i].id, path, sizeof(path)));
}

BRIDGE_NATIVE(seen_ready)
{
    say_tag(api, lists[2], "ready");
    return (0);
}

BRIDGE_NATIVE(seen_failed)
{
    say_tag(api, lists[3], "failed");
    return (0);
}

BRIDGE_NATIVE(seen_modified)
{
    say_tag(api, lists[4], "modified");
    return (0);
}

BRIDGE_NATIVE(seen_raw)
{
    const bridge_list_t *l = bridge_native_get_list(api, lists[5]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    const ideatek_raw_bytes_t *raw;
    char path[128];
    char data[128];

    for (uint32_t i = 0; i < l->count; i++) {
        raw = game_cell(api, t[i].id, game_resources_slots.raw);
        if (!raw)
            continue;
        show_text(data, sizeof(data), api, raw->data);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u raw [%s] [%s]",
            game.round + 1, path_of(api, t[i].id, path, sizeof(path)), data);
    }
    return (0);
}

BRIDGE_NATIVE(seen_handle)
{
    const bridge_list_t *l = bridge_native_get_list(api, lists[6]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        for (uint32_t k = 0; k < use_count; k++)
            if (uses[k] == t[i].id)
                bridge_native_say(api, BRIDGE_LOG_INFO, "r%u handle %u",
                    game.round + 1, k);
    return (0);
}

void game_watch_resources(const bridge_api_t *api)
{
    const game_resources_slots_t *s = &game_resources_slots;
    const uint32_t slots[7] = { s->bytes, s->source_read, s->ready,
        s->failed, s->modified, s->raw, s->handle };
    const char *const names[7] = { "seen_bytes", "seen_source_read",
        "seen_ready", "seen_failed", "seen_modified", "seen_raw",
        "seen_handle" };
    const uint32_t events[7] = { BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH,
        BRIDGE_ON_ENTER, BRIDGE_ON_ENTER, BRIDGE_ON_ENTER, BRIDGE_ON_ENTER,
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, BRIDGE_ON_ENTER };
    const uint32_t changes[7][3] = {{ s->text, s->outcome, s->label },
        { s->source_result, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
        { 0, 0, 0 }, { 0, 0, 0 } };
    const uint32_t counts[7] = { 3, 1, 0, 0, 0, 0, 0 };
    bridge_term_t term[1];
    bridge_observer_statement_t statement;

    for (uint32_t i = 0; i < 7; i++) {
        term[0] = (bridge_term_t){ slots[i], BRIDGE_READ, BRIDGE_AND, 0 };
        statement = (bridge_observer_statement_t){
            .name = bridge_native_put(api, names[i],
                (uint32_t)strlen(names[i])),
            .name_len = (uint32_t)strlen(names[i]), .events = events[i],
            .terms = bridge_native_put(api, term, sizeof(term)),
            .term_count = 1,
            .changes = bridge_native_put_slots(api, changes[i], counts[i]),
            .change_count = counts[i] };
        lists[i] = api->observe(api, bridge_native_put(api, &statement,
            sizeof(statement)));
    }
}

#endif
