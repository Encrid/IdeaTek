/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "gltf_inside.h"

float weight_of(const cgltf_node *n, uint32_t k)
{
    if (k < n->weights_count)
        return (n->weights[k]);
    if (k < n->mesh->weights_count)
        return (n->mesh->weights[k]);
    return (0.0f);
}

static float column_length(const float m[16], uint32_t c)
{
    return (sqrtf(m[c * 4] * m[c * 4] + m[c * 4 + 1] * m[c * 4 + 1]
        + m[c * 4 + 2] * m[c * 4 + 2]));
}

void split_matrix(const float m[16], ideatek_model_node_t *out)
{
    float s[3] = { column_length(m, 0), column_length(m, 1),
        column_length(m, 2) };
    float det = m[0] * (m[5] * m[10] - m[9] * m[6]) - m[4] * (m[1] * m[10]
        - m[9] * m[2]) + m[8] * (m[1] * m[6] - m[5] * m[2]);
    float r[9];

    s[0] = det < 0.0f ? -s[0] : s[0];
    for (uint32_t k = 0; k < 9; k++)
        r[k] = s[k / 3] != 0.0f ? m[(k / 3) * 4 + k % 3] / s[k / 3] : 0.0f;
    out->position = (ideatek_position_t){ m[12], m[13], m[14] };
    out->scale = (ideatek_vec3_t){ s[0], s[1], s[2] };
    out->rotation = (ideatek_quat_t){
        copysignf(sqrtf(fmaxf(0.0f, 1.0f + r[0] - r[4] - r[8])) / 2.0f,
            r[5] - r[7]),
        copysignf(sqrtf(fmaxf(0.0f, 1.0f - r[0] + r[4] - r[8])) / 2.0f,
            r[6] - r[2]),
        copysignf(sqrtf(fmaxf(0.0f, 1.0f - r[0] - r[4] + r[8])) / 2.0f,
            r[1] - r[3]),
        sqrtf(fmaxf(0.0f, 1.0f + r[0] + r[4] + r[8])) / 2.0f };
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
