/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_scene.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

static uint32_t declare_node(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:ideatek_model_node_t", ideatek_model_node_t));
}

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    declare_node(api);
    STRUCT(api, "ideatek:Model", ideatek_model_t);
    STRUCT(api, "ideatek:ModelInstance", ideatek_model_instance_t);
    STRUCT(api, "ideatek:ModelNode", ideatek_model_node_ref_t);
}

[[maybe_unused]] static void author(const bridge_api_t *api)
{
    ideatek_model_t model = { .revision = 1 };
    uint64_t id = api->spawn(api);

    declare_fields(api);
    declare_node(api);
    BRIDGE_NATIVE_ATTACH(api, id, STRUCT(api, "ideatek:Model",
        ideatek_model_t), model);
}

[[maybe_unused]] static void play(const bridge_api_t *api)
{
    ideatek_model_instance_t instance = { .model = { 0 } };
    uint64_t id = api->spawn(api);

    declare_all(api);
    BRIDGE_NATIVE_ATTACH(api, id, STRUCT(api, "ideatek:ModelInstance",
        ideatek_model_instance_t), instance);
}

[[maybe_unused]] static void spawn_node(const bridge_api_t *api)
{
    ideatek_model_node_ref_t ref = { .node = 0 };
    uint64_t id = api->spawn(api);

    declare_fields(api);
    declare_node(api);
    STRUCT(api, "ideatek:Model", ideatek_model_t);
    STRUCT(api, "ideatek:ModelInstance", ideatek_model_instance_t);
    BRIDGE_NATIVE_ATTACH(api, id, STRUCT(api, "ideatek:ModelNode",
        ideatek_model_node_ref_t), ref);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_node(api);
    declare_fields(api);
#elif defined(ROLE_AUTHOR)
    author(api);
#elif defined(ROLE_GAME)
    play(api);
#elif defined(ROLE_SCENE)
    spawn_node(api);
#else
    declare_all(api);
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
