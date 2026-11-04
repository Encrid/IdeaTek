/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_physics.h"
#include "ideatek_fields.h"
#include "ideatek_effects.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_curve_kind_t", ideatek_curve_kind_t);
    ENUM(api, "ideatek:ideatek_emitter_state_t", ideatek_emitter_state_t);
    MASK(api, "ideatek:ideatek_emitter_flags_t", ideatek_emitter_flags_t);
    ENUM(api, "ideatek:ideatek_emission_shape_t", ideatek_emission_shape_t);
    ENUM(api, "ideatek:ideatek_particle_kind_t", ideatek_particle_kind_t);
    ENUM(api, "ideatek:ideatek_particle_align_t", ideatek_particle_align_t);
    ENUM(api, "ideatek:ideatek_particle_sort_t", ideatek_particle_sort_t);
    ENUM(api, "ideatek:ideatek_collision_source_t", ideatek_collision_source_t);
    ENUM(api, "ideatek:ideatek_collision_mode_t", ideatek_collision_mode_t);
    ENUM(api, "ideatek:ideatek_spawn_on_t", ideatek_spawn_on_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Curve", ideatek_curve_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Curve", ideatek_curve_t);
    STRUCT(api, "ideatek:ParticleEmitter", ideatek_particle_emitter_t);
    STRUCT(api, "ideatek:ParticleShape", ideatek_particle_shape_t);
    STRUCT(api, "ideatek:ParticleVelocity", ideatek_particle_velocity_t);
    STRUCT(api, "ideatek:ParticleForces", ideatek_particle_forces_t);
    STRUCT(api, "ideatek:ParticleEmitterState",
        ideatek_particle_emitter_state_t);
    STRUCT(api, "ideatek:ParticleEnded", ideatek_particle_ended_t);
    STRUCT(api, "ideatek:ParticleLook", ideatek_particle_look_t);
    STRUCT(api, "ideatek:ParticleCollision", ideatek_particle_collision_t);
    STRUCT(api, "ideatek:ParticleSpawn", ideatek_particle_spawn_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_curve_kind_t", ideatek_curve_kind_t);
    ENUM(api, "ideatek:ideatek_emitter_state_t", ideatek_emitter_state_t);
    MASK(api, "ideatek:ideatek_emitter_flags_t", ideatek_emitter_flags_t);
    ENUM(api, "ideatek:ideatek_emission_shape_t", ideatek_emission_shape_t);
    ENUM(api, "ideatek:ideatek_particle_kind_t", ideatek_particle_kind_t);
    ENUM(api, "ideatek:ideatek_particle_align_t", ideatek_particle_align_t);
    ENUM(api, "ideatek:ideatek_particle_sort_t", ideatek_particle_sort_t);
    ENUM(api, "ideatek:ideatek_collision_source_t", ideatek_collision_source_t);
    ENUM(api, "ideatek:ideatek_collision_mode_t", ideatek_collision_mode_t);
    ENUM(api, "ideatek:ideatek_spawn_on_t", ideatek_spawn_on_t);
    api->attach(api, id, STRUCT(api, "ideatek:Curve", ideatek_curve_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleEmitter",
        ideatek_particle_emitter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleShape",
        ideatek_particle_shape_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleVelocity",
        ideatek_particle_velocity_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleForces",
        ideatek_particle_forces_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleLook",
        ideatek_particle_look_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleCollision",
        ideatek_particle_collision_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleSpawn",
        ideatek_particle_spawn_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_effects(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    ENUM(api, "ideatek:ideatek_curve_kind_t", ideatek_curve_kind_t);
    ENUM(api, "ideatek:ideatek_emitter_state_t", ideatek_emitter_state_t);
    MASK(api, "ideatek:ideatek_emitter_flags_t", ideatek_emitter_flags_t);
    ENUM(api, "ideatek:ideatek_emission_shape_t", ideatek_emission_shape_t);
    ENUM(api, "ideatek:ideatek_particle_kind_t", ideatek_particle_kind_t);
    ENUM(api, "ideatek:ideatek_particle_align_t", ideatek_particle_align_t);
    ENUM(api, "ideatek:ideatek_particle_sort_t", ideatek_particle_sort_t);
    ENUM(api, "ideatek:ideatek_collision_source_t", ideatek_collision_source_t);
    ENUM(api, "ideatek:ideatek_collision_mode_t", ideatek_collision_mode_t);
    ENUM(api, "ideatek:ideatek_spawn_on_t", ideatek_spawn_on_t);
    STRUCT(api, "ideatek:Curve", ideatek_curve_t);
    STRUCT(api, "ideatek:ParticleEmitter", ideatek_particle_emitter_t);
    STRUCT(api, "ideatek:ParticleShape", ideatek_particle_shape_t);
    STRUCT(api, "ideatek:ParticleVelocity", ideatek_particle_velocity_t);
    STRUCT(api, "ideatek:ParticleForces", ideatek_particle_forces_t);
    STRUCT(api, "ideatek:ParticleLook", ideatek_particle_look_t);
    STRUCT(api, "ideatek:ParticleCollision", ideatek_particle_collision_t);
    STRUCT(api, "ideatek:ParticleSpawn", ideatek_particle_spawn_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Shape", ideatek_shape_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Wind", ideatek_wind_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ForceField", ideatek_force_field_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleEmitterState",
        ideatek_particle_emitter_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleEnded",
        ideatek_particle_ended_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_EFFECTS)
    role_effects(api);
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
