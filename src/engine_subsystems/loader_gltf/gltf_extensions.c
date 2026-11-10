/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <float.h>

#include "gltf_inside.h"

static void put(const bridge_api_t *api, uint64_t id, uint32_t slot,
    const void *value)
{
    static const uint32_t SIZES[] = { [CLEARCOAT] = sizeof(
        ideatek_material_clearcoat_t), [SPECULAR] = sizeof(
        ideatek_material_specular_t), [TRANSMISSION] = sizeof(
        ideatek_material_transmission_t), [ANISOTROPY] = sizeof(
        ideatek_material_anisotropy_t), [DIFFUSE] = sizeof(
        ideatek_material_diffuse_transmission_t) };

    if (value)
        api->attach(api, id, slot, bridge_native_lend(api, value,
            SIZES[slot]));
    else
        api->detach(api, id, slot);
}

static void give_coat(gltf_build_t *b, uint64_t id, const cgltf_material *m)
{
    const cgltf_clearcoat *c = &m->clearcoat;
    ideatek_material_clearcoat_t x = { slot_of(b, &c->clearcoat_texture),
        slot_of(b, &c->clearcoat_roughness_texture),
        slot_of(b, &c->clearcoat_normal_texture), c->clearcoat_factor,
        c->clearcoat_roughness_factor };
    const cgltf_specular *s = &m->specular;
    bool has = m->has_specular;
    ideatek_material_specular_t y = { slot_of(b, &s->specular_texture),
        slot_of(b, &s->specular_color_texture), { has
        ? s->specular_color_factor[0] : 1.0f, has ? s->specular_color_factor[1]
        : 1.0f, has ? s->specular_color_factor[2] : 1.0f }, has
        ? s->specular_factor : 1.0f, m->has_ior ? m->ior.ior : 1.5f };

    put(b->api, id, CLEARCOAT, m->has_clearcoat ? &x : nullptr);
    put(b->api, id, SPECULAR, has || m->has_ior ? &y : nullptr);
}

void give_volume(gltf_build_t *b, uint64_t id, const cgltf_material *m)
{
    const cgltf_volume *v = &m->volume;
    float distance = v->attenuation_distance == FLT_MAX ? 0.0f
        : v->attenuation_distance;
    ideatek_material_transmission_t x = { slot_of(b,
        &m->transmission.transmission_texture), slot_of(b,
        &v->thickness_texture), { v->attenuation_color[0],
        v->attenuation_color[1], v->attenuation_color[2] },
        m->transmission.transmission_factor, v->thickness_factor, distance };

    put(b->api, id, TRANSMISSION, m->has_transmission || m->has_volume ? &x
        : nullptr);
}

static void give_flags(gltf_build_t *b, uint64_t id, const cgltf_material *m)
{
    if (m->unlit)
        b->api->attach(b->api, id, UNLIT, BRIDGE_ZEROED);
    else
        b->api->detach(b->api, id, UNLIT);
    if (m->has_sheen || m->has_iridescence || m->has_dispersion)
        say_path(b->api, b->file->entity, ": a material has an extension "
            "with no type");
}

void give_extensions(gltf_build_t *b, uint64_t id, const cgltf_material *m)
{
    const cgltf_anisotropy *a = &m->anisotropy;
    const cgltf_diffuse_transmission *d = &m->diffuse_transmission;
    ideatek_material_anisotropy_t x = { slot_of(b, &a->anisotropy_texture),
        a->anisotropy_strength, a->anisotropy_rotation };
    ideatek_material_diffuse_transmission_t y = { slot_of(b,
        &d->diffuse_transmission_texture), slot_of(b,
        &d->diffuse_transmission_color_texture), { d->
        diffuse_transmission_color_factor[0], d->
        diffuse_transmission_color_factor[1], d->
        diffuse_transmission_color_factor[2] },
        d->diffuse_transmission_factor };

    give_coat(b, id, m);
    give_volume(b, id, m);
    put(b->api, id, ANISOTROPY, m->has_anisotropy ? &x : nullptr);
    put(b->api, id, DIFFUSE, m->has_diffuse_transmission ? &y : nullptr);
    give_flags(b, id, m);
}
