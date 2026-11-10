/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

void fill_materials(gltf_build_t *b, gltf_tables_t *t, const cgltf_node *n,
    ideatek_model_node_t *out)
{
    const cgltf_mesh *m = n->mesh;
    const cgltf_material *x;
    const gltf_plan_t *plan = m ? &b->plans[m - b->data->meshes] : nullptr;

    out->material_first = t->counts[1];
    out->material_count = plan ? plan->submeshes : 0;
    for (size_t i = 0; plan && i < m->primitives_count; i++) {
        if (!is_usable(plan, &m->primitives[i]))
            continue;
        x = m->primitives[i].material;
        t->materials[t->counts[1]++].id = x ? b->parts[KIND_MATERIAL][x
            - b->data->materials]->entity : b->fallback->entity;
    }
}

void give_materials(gltf_build_t *b)
{
    gltf_part_t *p;
    ideatek_resource_label_t l = { .resource = { b->file->entity } };

    for (size_t i = 0; i < b->data->materials_count; i++) {
        p = b->parts[KIND_MATERIAL][i];
        l.label = text_of(b, b->labels[KIND_MATERIAL][i]);
        BRIDGE_NATIVE_ATTACH(b->api, p->entity, LABEL, l);
        give_material(b, p->entity, &b->data->materials[i]);
    }
    if (!b->fallback)
        return;
    l.label = text_of(b, b->fallback_label);
    BRIDGE_NATIVE_ATTACH(b->api, b->fallback->entity, LABEL, l);
    give_material(b, b->fallback->entity, &GLTF_DEFAULT);
}

static bool has_bare(const cgltf_mesh *m)
{
    for (size_t k = 0; k < m->primitives_count; k++)
        if (!m->primitives[k].material)
            return (true);
    return (false);
}

bool needs_default(const cgltf_data *data)
{
    for (size_t i = 0; i < data->meshes_count; i++)
        if (has_bare(&data->meshes[i]))
            return (true);
    return (false);
}
