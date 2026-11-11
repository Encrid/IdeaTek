/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_matter.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

static void declare_records(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_matter_chunk_t", ideatek_matter_chunk_t);
    STRUCT(api, "ideatek:ideatek_particle_state_t", ideatek_particle_state_t);
}

static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_solver_t", ideatek_solver_t);
    ENUM(api, "ideatek:ideatek_coating_rule_t", ideatek_coating_rule_t);
}

static void declare_resources(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:MatterFamily", ideatek_matter_family_t);
    STRUCT(api, "ideatek:MatterMaterial", ideatek_matter_material_t);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    declare_records(api);
    declare_resources(api);
    STRUCT(api, "ideatek:Matter", ideatek_matter_t);
    STRUCT(api, "ideatek:ParticleMatter", ideatek_particle_matter_t);
}

[[maybe_unused]] static void author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    declare_fields(api);
    declare_records(api);
    api->attach(api, id, STRUCT(api, "ideatek:MatterFamily",
        ideatek_matter_family_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MatterMaterial",
        ideatek_matter_material_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Matter", ideatek_matter_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleMatter",
        ideatek_particle_matter_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void apply_laws(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    declare_fields(api);
    declare_records(api);
    declare_resources(api);
    api->attach(api, id, STRUCT(api, "ideatek:Matter", ideatek_matter_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleMatter",
        ideatek_particle_matter_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void read_state(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_solver_t", ideatek_solver_t);
    declare_records(api);
    STRUCT(api, "ideatek:Matter", ideatek_matter_t);
    STRUCT(api, "ideatek:ParticleMatter", ideatek_particle_matter_t);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_resources(api);
    declare_fields(api);
#elif defined(ROLE_AUTHOR)
    author(api);
#elif defined(ROLE_MATTER)
    apply_laws(api);
#elif defined(ROLE_READER)
    read_state(api);
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
