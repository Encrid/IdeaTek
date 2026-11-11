/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_matter.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_coating_rule(void)
{
    SIZE(ideatek_coating_rule_t);
}

static void show_family(void)
{
    SIZE(ideatek_matter_family_t);
    AT(ideatek_matter_family_t, thermal_conductivity);
    AT(ideatek_matter_family_t, specific_heat);
    AT(ideatek_matter_family_t, emissivity);
    AT(ideatek_matter_family_t, air_rate);
    AT(ideatek_matter_family_t, volume_keep);
    AT(ideatek_matter_family_t, smoke_share);
    AT(ideatek_matter_family_t, ash_share);
    AT(ideatek_matter_family_t, particles_burn);
}

static void show_material(void)
{
    SIZE(ideatek_matter_material_t);
    AT(ideatek_matter_material_t, family);
    AT(ideatek_matter_material_t, solid);
    AT(ideatek_matter_material_t, soft);
    AT(ideatek_matter_material_t, liquid);
    AT(ideatek_matter_material_t, gas);
    AT(ideatek_matter_material_t, softening_point);
    AT(ideatek_matter_material_t, melting_point);
    AT(ideatek_matter_material_t, boiling_point);
    AT(ideatek_matter_material_t, fire_temperature);
    AT(ideatek_matter_material_t, burn_rate);
    AT(ideatek_matter_material_t, burn_heat);
    AT(ideatek_matter_material_t, electric_conductivity);
    AT(ideatek_matter_material_t, adhesion);
    AT(ideatek_matter_material_t, flammable);
}

static void show_chunk(void)
{
    SIZE(ideatek_matter_chunk_t);
    AT(ideatek_matter_chunk_t, temperature);
    AT(ideatek_matter_chunk_t, coating_mass);
    AT(ideatek_matter_chunk_t, coating);
    AT(ideatek_matter_chunk_t, phase);
    AT(ideatek_matter_chunk_t, burning);
}

static void show_matter(void)
{
    SIZE(ideatek_matter_t);
    AT(ideatek_matter_t, material);
    AT(ideatek_matter_t, chunks);
    AT(ideatek_matter_t, chunk_count);
    AT(ideatek_matter_t, charge);
}

static void show_particle_state(void)
{
    SIZE(ideatek_particle_state_t);
    AT(ideatek_particle_state_t, temperature);
    AT(ideatek_particle_state_t, material);
}

static void show_particle_matter(void)
{
    SIZE(ideatek_particle_matter_t);
    AT(ideatek_particle_matter_t, materials);
    AT(ideatek_particle_matter_t, states);
    AT(ideatek_particle_matter_t, material_count);
    AT(ideatek_particle_matter_t, state_count);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_coating_rule_t_desc, ideatek_matter_family_t_desc,
        ideatek_matter_material_t_desc, ideatek_matter_chunk_t_desc,
        ideatek_matter_t_desc, ideatek_particle_state_t_desc,
        ideatek_particle_matter_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_COATING_RULE_VALUES),
        TEXT_OF(IDEATEK_MATTER_FAMILY_FIELDS),
        TEXT_OF(IDEATEK_MATTER_MATERIAL_FIELDS),
        TEXT_OF(IDEATEK_MATTER_CHUNK_FIELDS), TEXT_OF(IDEATEK_MATTER_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_STATE_FIELDS),
        TEXT_OF(IDEATEK_PARTICLE_MATTER_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "coating_rule", show_coating_rule }, { "family", show_family },
        { "material", show_material }, { "chunk", show_chunk },
        { "matter", show_matter }, { "particle_state", show_particle_state },
        { "particle_matter", show_particle_matter },
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
