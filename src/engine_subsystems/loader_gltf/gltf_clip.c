/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <string.h>

#include "gltf_inside.h"

static const uint8_t INTERPOLATIONS[] = { IDEATEK_INTERPOLATION_LINEAR,
    IDEATEK_INTERPOLATION_STEP, IDEATEK_INTERPOLATION_CUBIC_SPLINE };

static bool is_kept(const cgltf_animation_channel *c)
{
    return (c->target_node
        && c->target_path != cgltf_animation_path_type_invalid);
}

static uint32_t count_kept(gltf_build_t *b, size_t clip)
{
    const cgltf_animation *a = &b->data->animations[clip];
    uint32_t kept = 0;

    for (size_t i = 0; i < a->channels_count; i++)
        kept += is_kept(&a->channels[i]);
    if (!b->out.base && kept < a->channels_count)
        say_index(b->api, b->file->entity, ": a channel of the clip %zu has "
            "no node, and is skipped", clip);
    return (kept);
}

static void write_channel(gltf_build_t *b, const cgltf_animation_channel *c,
    uint32_t start, uint8_t *record)
{
    const cgltf_accessor *in = c->sampler->input;
    const cgltf_accessor *out = c->sampler->output;
    size_t values = out->count * cgltf_num_components(out->type);
    ideatek_clip_channel_t r = { .target = target_of(b->data,
        c->target_node), .key_count = (uint32_t)in->count,
        .property = (uint8_t)(c->target_path - 1),
        .interpolation = INTERPOLATIONS[c->sampler->interpolation] };
    float *times;
    float *v;

    r.times = b->out.at - start;
    times = out_take(&b->out, (uint32_t)in->count * 4);
    r.values = b->out.at - start;
    v = out_take(&b->out, (uint32_t)values * 4);
    if (!times)
        return;
    cgltf_accessor_unpack_floats(in, times, in->count);
    cgltf_accessor_unpack_floats(out, v, values);
    memcpy(record, &r, sizeof(r));
}

static float last_time(const cgltf_animation_channel *c)
{
    float t = 0.0f;
    const cgltf_accessor *in = c->sampler->input;

    if (in->count)
        cgltf_accessor_read_float(in, in->count - 1, &t, 1);
    return (t);
}

void write_clip(gltf_build_t *b, size_t clip)
{
    const cgltf_animation *a = &b->data->animations[clip];
    uint32_t kept = count_kept(b, clip);
    uint32_t start;
    uint8_t *records;
    uint32_t k = 0;

    out_align(&b->out, 8);
    start = b->out.at;
    records = out_take(&b->out, kept * 24);
    b->channels[clip] = kept;
    b->durations[clip] = 0.0f;
    for (size_t i = 0; i < a->channels_count; i++) {
        if (!is_kept(&a->channels[i]))
            continue;
        write_channel(b, &a->channels[i], start, records ? records + 24
            * k++ : nullptr);
        b->durations[clip] = fmaxf(b->durations[clip],
            last_time(&a->channels[i]));
    }
    b->spans[KIND_CLIP][clip] = (gltf_span_t){ start, b->out.at - start };
}
