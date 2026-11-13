/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "gltf_inside.h"
#include "math_mat3.h"

float weight_of(const cgltf_node *n, uint32_t k)
{
    if (k < n->weights_count)
        return (n->weights[k]);
    if (k < n->mesh->weights_count)
        return (n->mesh->weights[k]);
    return (0.0f);
}

void split_matrix(const float m[16], ideatek_model_node_t *out)
{
    const float basis[9] = { m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9],
        m[10] };
    quatf_t q;
    vec3f_t s;

    mat3f_split(mat3f_load(basis), &q, &s);
    out->position = (ideatek_position_t){ m[12], m[13], m[14] };
    out->scale = (ideatek_vec3_t){ s.x, s.y, s.z };
    out->rotation = (ideatek_quat_t){ q.x, q.y, q.z, q.w };
}

void fill_light(const cgltf_light *l, ideatek_light_t *out)
{
    static const uint8_t KINDS_OF[] = { 0, IDEATEK_LIGHT_KIND_DIRECTIONAL,
        IDEATEK_LIGHT_KIND_POINT, IDEATEK_LIGHT_KIND_SPOT };
    ideatek_light_t x = {
        .color = { l->color[0], l->color[1], l->color[2] },
        .intensity = l->intensity,
        .range = l->range,
        .inner_cone = l->spot_inner_cone_angle,
        .outer_cone = l->spot_outer_cone_angle,
        .kind = KINDS_OF[l->type] };

    *out = x;
}

void fill_camera(const cgltf_camera *c, ideatek_projection_t *out)
{
    if (c->type == cgltf_camera_type_orthographic)
        *out = (ideatek_projection_t){ .height = 2.0f
            * c->data.orthographic.ymag, .near = c->data.orthographic.znear,
            .far = c->data.orthographic.zfar,
            .kind = IDEATEK_PROJECTION_KIND_ORTHOGRAPHIC };
    else
        *out = (ideatek_projection_t){ .fov = c->data.perspective.yfov,
            .near = c->data.perspective.znear,
            .far = c->data.perspective.zfar,
            .kind = IDEATEK_PROJECTION_KIND_PERSPECTIVE };
}
