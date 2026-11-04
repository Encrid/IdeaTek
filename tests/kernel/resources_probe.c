/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_files.h"
#include "ideatek_resources.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef struct {
    uint32_t load;
    uint32_t handle;
    uint32_t resource;
    uint32_t label;
} shared_slots_t;

typedef struct {
    uint32_t loader;
    uint32_t bytes;
    uint32_t outcome;
} loader_slots_t;

typedef struct {
    uint32_t reload;
    uint32_t modified;
    uint32_t ready;
    uint32_t failed;
} tag_slots_t;

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_load_state_t", ideatek_load_state_t);
    ENUM(api, "ideatek:ideatek_resource_error_t", ideatek_resource_error_t);
}

[[maybe_unused]] static uint32_t declare_load(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:ResourceLoad", ideatek_resource_load_t));
}

[[maybe_unused]] static shared_slots_t declare_shared(const bridge_api_t *api)
{
    shared_slots_t slots;

    declare_fields(api);
    slots.load = declare_load(api);
    slots.handle = STRUCT(api, "ideatek:ResourceHandle",
        ideatek_resource_handle_t);
    slots.resource = STRUCT(api, "ideatek:Resource", ideatek_resource_t);
    slots.label = STRUCT(api, "ideatek:ResourceLabel",
        ideatek_resource_label_t);
    return (slots);
}

[[maybe_unused]] static loader_slots_t declare_loader(const bridge_api_t *api)
{
    loader_slots_t slots;

    slots.loader = STRUCT(api, "ideatek:ResourceLoader",
        ideatek_resource_loader_t);
    slots.bytes = STRUCT(api, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t);
    slots.outcome = STRUCT(api, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t);
    return (slots);
}

[[maybe_unused]] static tag_slots_t declare_tags(const bridge_api_t *api)
{
    tag_slots_t slots;

    slots.reload = TAG(api, "ideatek:ResourceReload");
    slots.modified = TAG(api, "ideatek:ResourceModified");
    slots.ready = TAG(api, "ideatek:ResourceReady");
    slots.failed = TAG(api, "ideatek:ResourceFailed");
    return (slots);
}

[[maybe_unused]] static bridge_text_t put(const bridge_api_t *api,
    const char *text)
{
    uint32_t len = (uint32_t)strlen(text);
    uint32_t at = api->alloc(api, len);

    memcpy(api->shared_base + at, text, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

[[maybe_unused]] static void play(const bridge_api_t *api)
{
    shared_slots_t shared = declare_shared(api);
    tag_slots_t tags = declare_tags(api);
    ideatek_resource_load_t load = { .path = put(api, "probe.png") };
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_ATTACH(api, id, shared.load, load);
    api->attach(api, id, tags.reload, BRIDGE_ZEROED);
}

[[maybe_unused]] static void read_bytes(const bridge_api_t *api)
{
    shared_slots_t shared = declare_shared(api);
    loader_slots_t own = declare_loader(api);
    ideatek_resource_loader_t loader = { .name = put(api, "png"),
        .type = put(api, "test:Image"), .extensions = put(api, "png\n") };
    ideatek_resource_outcome_t outcome = {
        .error = IDEATEK_RESOURCE_ERROR_NONE };
    uint64_t id = api->spawn(api);
    ideatek_resource_label_t label = { .resource = { .id = id },
        .label = put(api, "mesh") };
    ideatek_resource_load_t load = { .owner = { .id = id },
        .path = put(api, "probe.ktx") };

    BRIDGE_NATIVE_ATTACH(api, id, own.loader, loader);
    BRIDGE_NATIVE_ATTACH(api, id, own.outcome, outcome);
    BRIDGE_NATIVE_ATTACH(api, id, shared.label, label);
    BRIDGE_NATIVE_ATTACH(api, id, shared.load, load);
}

[[maybe_unused]] static void give(const bridge_api_t *api)
{
    shared_slots_t shared = declare_shared(api);
    loader_slots_t own = declare_loader(api);
    tag_slots_t tags = declare_tags(api);
    uint64_t id = api->spawn(api);
    ideatek_resource_handle_t handle = { .resource = { .id = id } };
    ideatek_resource_t resource = { .path = put(api, "probe.png"),
        .state = IDEATEK_LOAD_STATE_LOADED };
    ideatek_resource_bytes_t bytes = { .loader = { .id = id },
        .data = put(api, "PNG") };
    ideatek_resource_outcome_t outcome = {
        .error = IDEATEK_RESOURCE_ERROR_NONE };
    ideatek_file_request_t request = { .path = put(api, "probe.png"),
        .storage = IDEATEK_STORAGE_CONTENT,
        .action = IDEATEK_FILE_ACTION_READ };

    ENUM(api, "ideatek:ideatek_storage_t", ideatek_storage_t);
    ENUM(api, "ideatek:ideatek_file_action_t", ideatek_file_action_t);
    ENUM(api, "ideatek:ideatek_file_status_t", ideatek_file_status_t);
    STRUCT(api, "ideatek:FileResult", ideatek_file_result_t);
    STRUCT(api, "ideatek:FileChange", ideatek_file_change_t);
    BRIDGE_NATIVE_ATTACH(api, id, shared.handle, handle);
    BRIDGE_NATIVE_ATTACH(api, id, shared.resource, resource);
    BRIDGE_NATIVE_ATTACH(api, id, own.bytes, bytes);
    BRIDGE_NATIVE_ATTACH(api, id, own.outcome, outcome);
    api->attach(api, id, tags.modified, BRIDGE_ZEROED);
    api->attach(api, id, tags.ready, BRIDGE_ZEROED);
    api->attach(api, id, tags.failed, BRIDGE_ZEROED);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api),
        STRUCT(api, "ideatek:FileRequest", ideatek_file_request_t), request);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_load(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    play(api);
#elif defined(ROLE_LOADER)
    read_bytes(api);
#elif defined(ROLE_RESOURCES)
    give(api);
#else
    declare_shared(api);
    declare_loader(api);
    declare_tags(api);
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
