/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static const uint8_t TOPOLOGIES[] = { 0, IDEATEK_TOPOLOGY_POINT_LIST,
    IDEATEK_TOPOLOGY_LINE_LIST, IDEATEK_TOPOLOGY_LINE_LIST,
    IDEATEK_TOPOLOGY_LINE_STRIP, IDEATEK_TOPOLOGY_TRIANGLE_LIST,
    IDEATEK_TOPOLOGY_TRIANGLE_STRIP, IDEATEK_TOPOLOGY_TRIANGLE_LIST };

uint8_t topology_of(cgltf_primitive_type type)
{
    return (TOPOLOGIES[(size_t)type]);
}

const cgltf_accessor *position_of(const cgltf_primitive *p)
{
    for (size_t i = 0; i < p->attributes_count; i++)
        if (p->attributes[i].type == cgltf_attribute_type_position)
            return (p->attributes[i].data);
    return (nullptr);
}

bool is_packed(const cgltf_primitive *p)
{
    const cgltf_accessor *a = position_of(p);
    const cgltf_buffer_view *v = a ? a->buffer_view : nullptr;

    return (p->has_draco_mesh_compression || (v
        && v->has_meshopt_compression));
}

bool is_usable(const gltf_plan_t *plan, const cgltf_primitive *p)
{
    return (!is_packed(p) && position_of(p)
        && topology_of(p->type) == plan->topology);
}

bool stream_of(const cgltf_attribute *a, gltf_stream_t *s)
{
    static const uint8_t SEMANTICS[] = { 0, IDEATEK_SEMANTIC_POSITION,
        IDEATEK_SEMANTIC_NORMAL, IDEATEK_SEMANTIC_TANGENT,
        IDEATEK_SEMANTIC_TEXCOORD, IDEATEK_SEMANTIC_COLOR,
        IDEATEK_SEMANTIC_JOINTS, IDEATEK_SEMANTIC_WEIGHTS };
    static const uint32_t COMPS[] = { 0, 3, 3, 4, 2, 4, 4, 4 };
    static const uint32_t FORMATS[] = { 0, 106, 106, 109, 103, 109, 95, 109 };
    size_t k = (size_t)a->type;

    if (k == 0 || k >= sizeof(COMPS) / sizeof(COMPS[0]))
        return (false);
    *s = (gltf_stream_t){ .semantic = SEMANTICS[k], .comps = COMPS[k],
        .format = FORMATS[k], .set = k >= 4 ? (uint8_t)a->index : 0 };
    return (true);
}
