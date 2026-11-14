/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <string.h>

#include "render_inside.h"

static const render_number_t NUMBERS[] = {
    { "lod_threshold", RENDER_AT(lod_threshold), 0, 3.4e38, true, false,
        "above 0" },
    { "origin_distance", RENDER_AT(origin_distance), 1, 1e15, false, false,
        "1 to 1e15" },
    { "max_instances", RENDER_AT(max_instances), 1, 16777216, false, false,
        "1 to 16777216" },
    { "max_lights", RENDER_AT(max_lights), 1, 65536, false, false,
        "1 to 65536" },
    { "cluster_x", RENDER_AT(cluster_x), 1, 128, false, false, "1 to 128" },
    { "cluster_y", RENDER_AT(cluster_y), 1, 128, false, false, "1 to 128" },
    { "cluster_z", RENDER_AT(cluster_z), 1, 128, false, false, "1 to 128" },
    { "lights_per_cell", RENDER_AT(lights_per_cell), 1, 65536, false, false,
        "1 to 65536" },
    { "geometry_bytes", RENDER_AT(geometry_bytes), 1048576, 17179869184.0,
        false, false, "1048576 to 17179869184" },
    { "texture_bytes", RENDER_AT(texture_bytes), 1048576, 17179869184.0,
        false, false, "1048576 to 17179869184" },
    { "capture_bytes", RENDER_AT(capture_bytes), 0, 1073741824, false, true,
        "0 to 1073741824, an even number" },
};

static const render_words_t WORDS[4] = {
    { "diffuse", RENDER_AT(diffuse), { "lambert", "burley" } },
    { "specular", RENDER_AT(specular), { "ggx", "none" } },
    { "shading", RENDER_AT(shading), { "pixel", "vertex" } },
    { "world_precision", RENDER_AT(world_precision),
        { "double_float", "float" } },
};

static bool check_numbers(const bridge_api_t *api, const uint8_t *s)
{
    const render_number_t *n;
    bool all = true;
    bool ok;
    double v;

    for (size_t i = 0; i < sizeof(NUMBERS) / sizeof(NUMBERS[0]); i++) {
        n = &NUMBERS[i];
        v = *(const double *)(s + n->at);
        ok = (v >= n->low) & (v <= n->high) & (!n->open | (v > n->low))
            & (!n->even | (fmod(v, 2.0) == 0.0));
        if (!ok)
            bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %.17g is outside %s",
                n->name, v, n->range);
        all &= ok;
    }
    return (all);
}

static uint32_t find_word(const render_words_t *w, const char *text,
    uint32_t len)
{
    uint32_t found = 2;

    for (uint32_t k = 0; k < 2; k++)
        if (strlen(w->words[k]) == len && !memcmp(text, w->words[k], len))
            found = k;
    return (found);
}

static bool check_words(const bridge_api_t *api, const uint8_t *s)
{
    const bridge_text_t *t;
    const char *text;
    bool all = true;

    for (uint32_t i = 0; i < 4; i++) {
        t = (const bridge_text_t *)(s + WORDS[i].at);
        text = (const char *)(api->shared_base + t->at);
        render_module.choices[i] = find_word(&WORDS[i], text, t->len);
        if (render_module.choices[i] == 2)
            bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %.*s is outside %s "
                "or %s", WORDS[i].name, (int)t->len, text,
                WORDS[i].words[0], WORDS[i].words[1]);
        all &= render_module.choices[i] != 2;
    }
    return (all);
}

bool start_render(const bridge_api_t *api)
{
    const render_settings_t *s = bridge_native_get_single(api,
        render_module.slots[SETTINGS]);
    bool ok = check_numbers(api, (const uint8_t *)s);
    uint32_t half;

    ok &= check_words(api, (const uint8_t *)s);
    half = ok * (uint32_t)(s->capture_bytes / 2);
    render_module.started = true;
    render_module.broken = !ok;
    render_module.settings = *s;
    if (half)
        render_module.halves[0].at = api->alloc(api, 2 * half);
    render_module.halves[1].at = render_module.halves[0].at + half;
    render_module.halves[0].len = half;
    render_module.halves[1].len = half;
    return (ok);
}
