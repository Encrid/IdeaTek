/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

const cgltf_material GLTF_DEFAULT = { .pbr_metallic_roughness = {
    .base_color_factor = { 1.0f, 1.0f, 1.0f, 1.0f }, .metallic_factor = 1.0f,
    .roughness_factor = 1.0f }, .normal_texture = { .scale = 1.0f },
    .occlusion_texture = { .scale = 1.0f }, .alpha_cutoff = 0.5f };

ideatek_texture_slot_t slot_of(gltf_build_t *b, const cgltf_texture_view *v)
{
    ideatek_texture_slot_t s = { .scale = { 1.0f, 1.0f },
        .texcoord = (uint8_t)v->texcoord };
    const cgltf_image *image = v->texture ? image_of(v->texture) : nullptr;
    const gltf_dep_t *d;

    d = image ? find_dep_of(b->file->entity, DEP_IMAGE,
        (size_t)(image - b->data->images)) : nullptr;
    s.texture.id = d ? d->resource : 0;
    if (!v->has_transform)
        return (s);
    s.offset[0] = v->transform.offset[0];
    s.offset[1] = v->transform.offset[1];
    s.scale[0] = v->transform.scale[0];
    s.scale[1] = v->transform.scale[1];
    s.rotation = v->transform.rotation;
    s.texcoord = (uint8_t)(v->transform.has_texcoord ? v->transform.texcoord
        : v->texcoord);
    return (s);
}

static void give_slots(gltf_build_t *b, const cgltf_material *m,
    ideatek_material_t *x)
{
    const cgltf_pbr_metallic_roughness *p = &m->pbr_metallic_roughness;

    x->base_color_texture = slot_of(b, &p->base_color_texture);
    x->metallic_roughness_texture = slot_of(b, &p->metallic_roughness_texture);
    x->normal_texture = slot_of(b, &m->normal_texture);
    x->occlusion_texture = slot_of(b, &m->occlusion_texture);
    x->emissive_texture = slot_of(b, &m->emissive_texture);
}

static ideatek_material_t base_of(gltf_build_t *b, const cgltf_material *m)
{
    const cgltf_pbr_metallic_roughness *p = &m->pbr_metallic_roughness;
    const float *c = p->base_color_factor;
    const float *e = m->emissive_factor;
    ideatek_material_t x = {
        .base_color = { c[0], c[1], c[2], c[3] },
        .emissive = { e[0], e[1], e[2] },
        .metallic = p->metallic_factor,
        .roughness = p->roughness_factor,
        .emissive_strength = m->has_emissive_strength
        ? m->emissive_strength.emissive_strength : 1.0f,
        .normal_scale = m->normal_texture.scale,
        .occlusion_strength = m->occlusion_texture.scale,
        .alpha_cutoff = m->alpha_cutoff,
        .alpha_mode = (uint8_t)m->alpha_mode,
        .double_sided = m->double_sided };

    give_slots(b, m, &x);
    return (x);
}

void give_material(gltf_build_t *b, uint64_t id, const cgltf_material *m)
{
    ideatek_material_t x = base_of(b, m);

    BRIDGE_NATIVE_ATTACH(b->api, id, MATERIAL, x);
    give_extensions(b, id, m);
}
