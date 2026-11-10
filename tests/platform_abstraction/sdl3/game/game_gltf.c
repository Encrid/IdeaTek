/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_gltf.h"
#include "game_resources.h"

#if defined(GAME_GLTF)

#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

game_gltf_slots_t game_gltf_slots;

static uint32_t lists[5];

static void put_bytes(char *args)
{
    char *comma = strchr(args, ',');
    size_t len = strlen(comma + 1) / 2;
    uint8_t *bytes = calloc(1, len + 1);
    char pair[3] = "";

    *comma = '\0';
    for (size_t i = 0; i < len; i++) {
        memcpy(pair, comma + 1 + 2 * i, 2);
        bytes[i] = (uint8_t)strtoul(pair, nullptr, 16);
    }
    SDL_SaveFile(args, bytes, len);
    free(bytes);
}

bool game_gltf_step(const bridge_api_t *api, const char *verb, char *args)
{
    (void)api;
    if (strcmp(verb, "putbytes"))
        return (false);
    put_bytes(args);
    return (true);
}

static void declare_materials(const bridge_api_t *api)
{
    game_gltf_slots_t *s = &game_gltf_slots;

    s->material = TYPED(api, BRIDGE_STRUCT, "ideatek:Material",
        ideatek_material_t);
    s->clearcoat = TYPED(api, BRIDGE_STRUCT, "ideatek:MaterialClearcoat",
        ideatek_material_clearcoat_t);
    s->specular = TYPED(api, BRIDGE_STRUCT, "ideatek:MaterialSpecular",
        ideatek_material_specular_t);
    s->transmission = TYPED(api, BRIDGE_STRUCT,
        "ideatek:MaterialTransmission", ideatek_material_transmission_t);
    s->anisotropy = TYPED(api, BRIDGE_STRUCT, "ideatek:MaterialAnisotropy",
        ideatek_material_anisotropy_t);
    s->diffuse = TYPED(api, BRIDGE_STRUCT,
        "ideatek:MaterialDiffuseTransmission",
        ideatek_material_diffuse_transmission_t);
    s->unlit = BRIDGE_NATIVE_DECLARE_TAG(api, "ideatek:MaterialUnlit",
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
}

void game_declare_gltf(const bridge_api_t *api)
{
    game_gltf_slots_t *s = &game_gltf_slots;

    s->color = TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_color_t",
        ideatek_color_t);
    s->rgb = TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_rgb_t", ideatek_rgb_t);
    s->topology = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_topology_t",
        ideatek_topology_t);
    s->index_format = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_index_format_t",
        ideatek_index_format_t);
    s->slot = TYPED(api, BRIDGE_STRUCT, "ideatek:ideatek_texture_slot_t",
        ideatek_texture_slot_t);
    s->alpha = TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_alpha_mode_t",
        ideatek_alpha_mode_t);
    s->model = TYPED(api, BRIDGE_STRUCT, "ideatek:Model", ideatek_model_t);
    s->mesh = TYPED(api, BRIDGE_STRUCT, "ideatek:Mesh", ideatek_mesh_t);
    declare_materials(api);
    s->binds = TYPED(api, BRIDGE_STRUCT, "ideatek:SkinBinds",
        ideatek_skin_binds_t);
    s->clip = TYPED(api, BRIDGE_STRUCT, "ideatek:Clip", ideatek_clip_t);
}

static void name_of(const bridge_api_t *api, uint64_t id, char *out)
{
    const ideatek_resource_label_t *l = game_cell(api, id,
        game_resources_slots.label);
    const ideatek_resource_t *r = game_cell(api, id,
        game_resources_slots.resource);
    bridge_text_t t = l ? l->label : r ? r->path : (bridge_text_t){ 0, 0 };
    uint32_t len = t.len < 120 ? t.len : 120;

    memcpy(out, api->shared_base + t.at, len);
    out[len] = '\0';
}

static const uint8_t *bytes_of(const bridge_api_t *api, bridge_text_t t)
{
    return (api->shared_base + t.at);
}

static void show_model(const bridge_api_t *api, uint64_t id)
{
    const ideatek_model_t *m = game_cell(api, id, game_gltf_slots.model);
    const uint8_t *b = bytes_of(api, m->bytes);
    ideatek_model_node_t n;
    const char *names = (const char *)b + m->node_count * sizeof(n)
        + m->light_count * sizeof(ideatek_light_t) + m->material_count * 8
        + m->weight_count * 4 + m->camera_count
        * sizeof(ideatek_projection_t);
    char name[128];

    name_of(api, id, name);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u model [%s] nodes %u lights "
        "%u materials %u weights %u cameras %u joints %u rev %u len %u",
        game.round + 1, name, m->node_count, m->light_count,
        m->material_count, m->weight_count, m->camera_count, m->joint_count,
        m->revision, m->bytes.len);
    for (uint32_t i = 0; i < m->node_count; i++) {
        memcpy(&n, b + i * sizeof(n), sizeof(n));
        bridge_native_say(api, BRIDGE_LOG_INFO, "node %u [%.*s] parent %d "
            "mesh %d binds %d mats %u+%u weights %u+%u light %d camera %d "
            "joint %d pos %.2f %.2f %.2f rot %.2f %.2f %.2f %.2f scale %.2f "
            "%.2f %.2f", i, (int)n.name_length, names + n.name_offset,
            (int)n.parent, n.mesh.id != 0, n.binds.id != 0, n.material_first,
            n.material_count, n.weight_first, n.weight_count, (int)n.light,
            (int)n.camera, (int)n.joint, n.position.x, n.position.y,
            n.position.z, (double)n.rotation.x, (double)n.rotation.y,
            (double)n.rotation.z, (double)n.rotation.w, (double)n.scale.x,
            (double)n.scale.y, (double)n.scale.z);
    }
}

static void show_streams(const bridge_api_t *api, const ideatek_mesh_t *m)
{
    const uint8_t *b = bytes_of(api, m->bytes);
    ideatek_mesh_stream_t s;
    ideatek_submesh_t sub;
    float v[4] = { 0 };

    for (uint32_t k = 0; k < m->stream_count; k++) {
        memcpy(&s, b + k * sizeof(s), sizeof(s));
        memcpy(v, b + s.offset, s.stride < 16 ? s.stride : 16);
        bridge_native_say(api, BRIDGE_LOG_INFO, "stream %u sem %u set %u "
            "target %u format %u stride %u first %.2f %.2f %.2f %.2f", k,
            s.semantic, s.set, s.target, s.format, s.stride, (double)v[0],
            (double)v[1], (double)v[2], (double)v[3]);
    }
    for (uint32_t k = 0; k < m->submesh_count; k++) {
        memcpy(&sub, b + m->stream_count * 16 + k * sizeof(sub), sizeof(sub));
        bridge_native_say(api, BRIDGE_LOG_INFO, "submesh %u first %u count "
            "%u", k, sub.first, sub.count);
    }
}

static void show_indices(const bridge_api_t *api, const ideatek_mesh_t *m)
{
    const uint8_t *b = bytes_of(api, m->bytes) + m->index_offset;
    char text[200] = "";
    size_t at = 0;
    uint32_t v;
    uint16_t narrow;

    for (uint32_t i = 0; i < m->index_count && i < 24; i++) {
        memcpy(&narrow, b + i * 2, 2);
        v = narrow;
        if (m->index_format == IDEATEK_INDEX_FORMAT_U32)
            memcpy(&v, b + i * 4, 4);
        at += (size_t)snprintf(text + at, sizeof(text) - at, " %u", v);
    }
    bridge_native_say(api, BRIDGE_LOG_INFO, "indices%s", text);
}

static void show_mesh(const bridge_api_t *api, uint64_t id)
{
    const ideatek_mesh_t *m = game_cell(api, id, game_gltf_slots.mesh);
    char name[128];

    name_of(api, id, name);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u mesh [%s] vertices %u "
        "indices %u streams %u submeshes %u lods %u targets %u topology %u "
        "format %u rev %u", game.round + 1, name, m->vertex_count,
        m->index_count, m->stream_count, m->submesh_count, m->lod_count,
        m->target_count, m->topology, m->index_format, m->revision);
    show_streams(api, m);
    show_indices(api, m);
}

static void show_material(const bridge_api_t *api, uint64_t id)
{
    const ideatek_material_t *m = game_cell(api, id, game_gltf_slots.material);
    const game_gltf_slots_t *s = &game_gltf_slots;
    char name[128];

    name_of(api, id, name);
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u material [%s] color %.2f %.2f "
        "%.2f %.2f metallic %.2f roughness %.2f emissive %.2f strength %.2f "
        "normal %.2f occlusion %.2f cutoff %.2f alpha %u double %d texture %d "
        "texcoord %u offset %.2f scale %.2f rotation %.2f ext %d%d%d%d%d%d",
        game.round + 1, name, (double)m->base_color.r, (double)m->base_color.g,
        (double)m->base_color.b, (double)m->base_color.a, (double)m->metallic,
        (double)m->roughness, (double)m->emissive.r,
        (double)m->emissive_strength, (double)m->normal_scale,
        (double)m->occlusion_strength, (double)m->alpha_cutoff, m->alpha_mode,
        m->double_sided, m->base_color_texture.texture.id != 0,
        m->base_color_texture.texcoord, (double)m->base_color_texture.offset[0],
        (double)m->base_color_texture.scale[0],
        (double)m->base_color_texture.rotation,
        game_cell(api, id, s->clearcoat) != nullptr,
        game_cell(api, id, s->specular) != nullptr,
        game_cell(api, id, s->transmission) != nullptr,
        game_cell(api, id, s->anisotropy) != nullptr,
        game_cell(api, id, s->diffuse) != nullptr,
        bridge_native_find_value(api, id, s->unlit) != 0);
}

static void show_extensions(const bridge_api_t *api, uint64_t id)
{
    const ideatek_material_specular_t *s = game_cell(api, id,
        game_gltf_slots.specular);
    const ideatek_material_transmission_t *t = game_cell(api, id,
        game_gltf_slots.transmission);
    const ideatek_material_clearcoat_t *c = game_cell(api, id,
        game_gltf_slots.clearcoat);

    if (s)
        bridge_native_say(api, BRIDGE_LOG_INFO, "specular factor %.2f color "
            "%.2f ior %.2f", (double)s->factor, (double)s->color.r,
            (double)s->ior);
    if (t)
        bridge_native_say(api, BRIDGE_LOG_INFO, "transmission factor %.2f "
            "thickness %.2f distance %.2f", (double)t->factor,
            (double)t->thickness, (double)t->attenuation_distance);
    if (c)
        bridge_native_say(api, BRIDGE_LOG_INFO, "clearcoat factor %.2f "
            "roughness %.2f", (double)c->factor, (double)c->roughness);
}

static void show_motion(const bridge_api_t *api, uint64_t id)
{
    const ideatek_skin_binds_t *s = game_cell(api, id, game_gltf_slots.binds);
    const ideatek_clip_t *c = game_cell(api, id, game_gltf_slots.clip);
    ideatek_clip_channel_t ch;
    float m[16];
    char name[128];

    name_of(api, id, name);
    for (uint32_t r = 0; s && r < s->count; r++) {
        memcpy(m, bytes_of(api, s->matrices) + r * 64, 64);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u binds [%s] count %u rank "
            "%u diagonal %.2f %.2f %.2f %.2f move %.2f", game.round + 1, name,
            s->count, r, (double)m[0], (double)m[5], (double)m[10],
            (double)m[15], (double)m[12]);
    }
    for (uint32_t k = 0; c && k < c->channel_count; k++) {
        memcpy(&ch, bytes_of(api, c->bytes) + k * 24, 24);
        memcpy(m, bytes_of(api, c->bytes) + ch.values, 16);
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u clip [%s] duration %.2f "
            "channel %u target %llx keys %u property %u interpolation %u "
            "value %.2f %.2f %.2f %.2f", game.round + 1, name,
            (double)c->duration, k, (unsigned long long)ch.target,
            ch.key_count, ch.property, ch.interpolation, (double)m[0],
            (double)m[1], (double)m[2], (double)m[3]);
    }
}

#define SEEN(fn, list, show) \
    BRIDGE_NATIVE(fn) \
    { \
        const bridge_list_t *l = bridge_native_get_list(api, lists[list]); \
        const bridge_trigger_t *t = (const void *)(api->private_base + l->at); \
        for (uint32_t i = 0; i < l->count; i++) \
            show(api, t[i].id); \
        return (0); \
    }

static void show_material_all(const bridge_api_t *api, uint64_t id)
{
    show_material(api, id);
    show_extensions(api, id);
}

SEEN(seen_model, 0, show_model)
SEEN(seen_mesh, 1, show_mesh)
SEEN(seen_material, 2, show_material_all)
SEEN(seen_binds, 3, show_motion)
SEEN(seen_clip, 4, show_motion)

void game_watch_gltf(const bridge_api_t *api)
{
    const game_gltf_slots_t *s = &game_gltf_slots;
    const uint32_t slots[5] = { s->model, s->mesh, s->material, s->binds,
        s->clip };
    const char *const names[5] = { "seen_model", "seen_mesh", "seen_material",
        "seen_binds", "seen_clip" };
    bridge_term_t term[1];
    bridge_observer_statement_t statement;

    for (uint32_t i = 0; i < 5; i++) {
        term[0] = (bridge_term_t){ slots[i], BRIDGE_READ, BRIDGE_AND, 0 };
        statement = (bridge_observer_statement_t){
            .name = bridge_native_put(api, names[i],
                (uint32_t)strlen(names[i])),
            .name_len = (uint32_t)strlen(names[i]),
            .events = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH,
            .terms = bridge_native_put(api, term, sizeof(term)),
            .term_count = 1 };
        lists[i] = api->observe(api, bridge_native_put(api, &statement,
            sizeof(statement)));
    }
}

#endif
