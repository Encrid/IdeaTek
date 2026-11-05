/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_effects.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_curve_kind(void)
{
    SIZE(ideatek_curve_kind_t);
}

static void show_curve(void)
{
    SIZE(ideatek_curve_t);
    AT(ideatek_curve_t, bytes);
    AT(ideatek_curve_t, key_count);
    AT(ideatek_curve_t, revision);
    AT(ideatek_curve_t, kind);
}

static void show_emitter_state(void)
{
    SIZE(ideatek_emitter_state_t);
}

static void show_emitter_flags(void)
{
    SIZE(ideatek_emitter_flags_t);
}

static void show_particle_emitter(void)
{
    SIZE(ideatek_particle_emitter_t);
    AT(ideatek_particle_emitter_t, rate);
    AT(ideatek_particle_emitter_t, rate_per_meter);
    AT(ideatek_particle_emitter_t, duration);
    AT(ideatek_particle_emitter_t, delay);
    AT(ideatek_particle_emitter_t, burst_interval);
    AT(ideatek_particle_emitter_t, lifetime_min);
    AT(ideatek_particle_emitter_t, lifetime_max);
    AT(ideatek_particle_emitter_t, speed_scale);
    AT(ideatek_particle_emitter_t, prewarm);
    AT(ideatek_particle_emitter_t, lod_distance);
    AT(ideatek_particle_emitter_t, cull_distance);
    AT(ideatek_particle_emitter_t, burst);
    AT(ideatek_particle_emitter_t, max);
    AT(ideatek_particle_emitter_t, seed);
    AT(ideatek_particle_emitter_t, state);
    AT(ideatek_particle_emitter_t, flags);
}

static void show_emission_shape(void)
{
    SIZE(ideatek_emission_shape_t);
}

static void show_particle_shape(void)
{
    SIZE(ideatek_particle_shape_t);
    AT(ideatek_particle_shape_t, mesh);
    AT(ideatek_particle_shape_t, size);
    AT(ideatek_particle_shape_t, kind);
}

static void show_particle_velocity(void)
{
    SIZE(ideatek_particle_velocity_t);
    AT(ideatek_particle_velocity_t, min);
    AT(ideatek_particle_velocity_t, max);
    AT(ideatek_particle_velocity_t, radial_min);
    AT(ideatek_particle_velocity_t, radial_max);
    AT(ideatek_particle_velocity_t, inherit_velocity);
}

static void show_particle_forces(void)
{
    SIZE(ideatek_particle_forces_t);
    AT(ideatek_particle_forces_t, gravity);
    AT(ideatek_particle_forces_t, drag);
    AT(ideatek_particle_forces_t, noise_strength);
    AT(ideatek_particle_forces_t, noise_frequency);
    AT(ideatek_particle_forces_t, radial_acceleration);
    AT(ideatek_particle_forces_t, tangential_acceleration);
    AT(ideatek_particle_forces_t, max_speed);
}

static void show_particle_emitter_state(void)
{
    SIZE(ideatek_particle_emitter_state_t);
    AT(ideatek_particle_emitter_state_t, alive);
    AT(ideatek_particle_emitter_state_t, done);
}

static void show_particle_ended(void)
{
    SIZE(ideatek_particle_ended_t);
    AT(ideatek_particle_ended_t, emitter);
    AT(ideatek_particle_ended_t, order);
}

static void show_particle_kind(void)
{
    SIZE(ideatek_particle_kind_t);
}

static void show_particle_align(void)
{
    SIZE(ideatek_particle_align_t);
}

static void show_particle_sort(void)
{
    SIZE(ideatek_particle_sort_t);
}

static void show_particle_look(void)
{
    SIZE(ideatek_particle_look_t);
    AT(ideatek_particle_look_t, material);
    AT(ideatek_particle_look_t, mesh);
    AT(ideatek_particle_look_t, size_curve);
    AT(ideatek_particle_look_t, color_curve);
    AT(ideatek_particle_look_t, start_color_curve);
    AT(ideatek_particle_look_t, speed_curve);
    AT(ideatek_particle_look_t, spin_curve);
    AT(ideatek_particle_look_t, color);
    AT(ideatek_particle_look_t, rotation_min);
    AT(ideatek_particle_look_t, rotation_max);
    AT(ideatek_particle_look_t, spin_min);
    AT(ideatek_particle_look_t, spin_max);
    AT(ideatek_particle_look_t, size_min);
    AT(ideatek_particle_look_t, size_max);
    AT(ideatek_particle_look_t, frame_rate);
    AT(ideatek_particle_look_t, trail_lifetime);
    AT(ideatek_particle_look_t, frames_x);
    AT(ideatek_particle_look_t, frames_y);
    AT(ideatek_particle_look_t, kind);
    AT(ideatek_particle_look_t, align);
    AT(ideatek_particle_look_t, sort);
    AT(ideatek_particle_look_t, frame_random);
}

static void show_collision_source(void)
{
    SIZE(ideatek_collision_source_t);
}

static void show_collision_mode(void)
{
    SIZE(ideatek_collision_mode_t);
}

static void show_particle_collision(void)
{
    SIZE(ideatek_particle_collision_t);
    AT(ideatek_particle_collision_t, touches);
    AT(ideatek_particle_collision_t, bounce);
    AT(ideatek_particle_collision_t, friction);
    AT(ideatek_particle_collision_t, radius);
    AT(ideatek_particle_collision_t, source);
    AT(ideatek_particle_collision_t, mode);
}

static void show_spawn_on(void)
{
    SIZE(ideatek_spawn_on_t);
}

static void show_particle_spawn(void)
{
    SIZE(ideatek_particle_spawn_t);
    AT(ideatek_particle_spawn_t, parent);
    AT(ideatek_particle_spawn_t, hit_material);
    AT(ideatek_particle_spawn_t, rate);
    AT(ideatek_particle_spawn_t, inherit_velocity);
    AT(ideatek_particle_spawn_t, count);
    AT(ideatek_particle_spawn_t, on);
}

static void show_particle_light(void)
{
    SIZE(ideatek_particle_light_t);
    AT(ideatek_particle_light_t, intensity);
    AT(ideatek_particle_light_t, range_scale);
    AT(ideatek_particle_light_t, fraction);
    AT(ideatek_particle_light_t, max);
}

static void show_particle_sound(void)
{
    SIZE(ideatek_particle_sound_t);
    AT(ideatek_particle_sound_t, sound);
    AT(ideatek_particle_sound_t, bus);
    AT(ideatek_particle_sound_t, volume);
    AT(ideatek_particle_sound_t, max);
    AT(ideatek_particle_sound_t, on);
}

static void show_particle_hits(void)
{
    SIZE(ideatek_particle_hits_t);
    AT(ideatek_particle_hits_t, emitter);
    AT(ideatek_particle_hits_t, hits);
    AT(ideatek_particle_hits_t, count);
    AT(ideatek_particle_hits_t, order);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_curve_kind_t_desc, ideatek_curve_t_desc,
        ideatek_emitter_state_t_desc, ideatek_emitter_flags_t_desc,
        ideatek_particle_emitter_t_desc, ideatek_emission_shape_t_desc,
        ideatek_particle_shape_t_desc, ideatek_particle_velocity_t_desc,
        ideatek_particle_forces_t_desc, ideatek_particle_emitter_state_t_desc,
        ideatek_particle_ended_t_desc, ideatek_particle_kind_t_desc,
        ideatek_particle_align_t_desc, ideatek_particle_sort_t_desc,
        ideatek_particle_look_t_desc, ideatek_collision_source_t_desc,
        ideatek_collision_mode_t_desc, ideatek_particle_collision_t_desc,
        ideatek_spawn_on_t_desc, ideatek_particle_spawn_t_desc,
        ideatek_particle_light_t_desc, ideatek_particle_sound_t_desc,
        ideatek_particle_hits_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_CURVE_KIND_VALUES), TEXT_OF(IDEATEK_CURVE_FIELDS),
        TEXT_OF(IDEATEK_EMITTER_STATE_VALUES),
        TEXT_OF(IDEATEK_EMITTER_FLAGS_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_EMITTER_FIELDS),
        TEXT_OF(IDEATEK_EMISSION_SHAPE_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_SHAPE_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_VELOCITY_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_FORCES_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_EMITTER_STATE_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_ENDED_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_KIND_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_ALIGN_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_SORT_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_LOOK_FIELDS),
        TEXT_OF(IDEATEK_COLLISION_SOURCE_VALUES),
        TEXT_OF(IDEATEK_COLLISION_MODE_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_COLLISION_FIELDS),
        TEXT_OF(IDEATEK_SPAWN_ON_VALUES),
        TEXT_OF(IDEATEK_PARTICLE_SPAWN_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_LIGHT_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_SOUND_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_HITS_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "curve_kind", show_curve_kind }, { "curve", show_curve },
        { "emitter_state", show_emitter_state },
        { "emitter_flags", show_emitter_flags },
        { "particle_emitter", show_particle_emitter },
        { "emission_shape", show_emission_shape },
        { "particle_shape", show_particle_shape },
        { "particle_velocity", show_particle_velocity },
        { "particle_forces", show_particle_forces },
        { "particle_emitter_state", show_particle_emitter_state },
        { "particle_ended", show_particle_ended },
        { "particle_kind", show_particle_kind },
        { "particle_align", show_particle_align },
        { "particle_sort", show_particle_sort },
        { "particle_look", show_particle_look },
        { "collision_source", show_collision_source },
        { "collision_mode", show_collision_mode },
        { "particle_collision", show_particle_collision },
        { "spawn_on", show_spawn_on },
        { "particle_spawn", show_particle_spawn },
        { "particle_light", show_particle_light },
        { "particle_sound", show_particle_sound },
        { "particle_hits", show_particle_hits },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
