/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const char *const CHECKS[3] = { "always", "never", "paths" };
static const char *const MODES[3] = { "unprocessed", "processed", "" };

static const struct {
    const char *name;
    uint32_t range[2];
} RULES[] = {
    { "reload_delay_ms", { 0, 60000 } }, { "load_bytes", { 1, 1073741824 } },
    { "max_resources", { 1, 1048576 } }, { "max_uses", { 1, 4194304 } },
    { "max_loaders", { 1, 4096 } }, { "max_sources", { 1, 1024 } },
    { "max_variables", { 1, 4096 } }, { "max_reads", { 1, 65536 } },
    { "max_labels", { 1, 1048576 } }, { "path_bytes", { 16, 4096 } },
    { "settings_bytes", { 0, 65536 } },
};

constexpr uint32_t RES_TEXT = 4095;

static bool check_count(const bridge_api_t *api, const char *name,
    const uint32_t value, const uint32_t range[2])
{
    bool ok = (value >= range[0]) & (value <= range[1]);

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %u to %u",
            name, value, range[0], range[1]);
    return (ok);
}

static int pick_choice(const bridge_api_t *api, const char *name,
    bridge_text_t text, const char *const words[3])
{
    res_text_t value = res_text(api, text);
    res_text_t word;
    int pick = -1;
    int shown = (int)(text.len < 64 ? text.len : 64);
    const char *third = words[2][0] ? " or " : "";

    for (int k = 0; k < 3; k++) {
        word.at = words[k];
        word.len = (uint32_t)strlen(words[k]);
        pick = word.len && same_text(&value, &word) ? k : pick;
    }
    if (pick < 0)
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "%s: %.*s is outside %s, %s%s%s", name, shown, value.at,
            words[0], words[1], third, words[2]);
    return (pick);
}

static bool check_text(const bridge_api_t *api, const char *name,
    bridge_text_t text)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    bool inside = (text.at >= h->arena_lo) & (text.at <= h->arena_hi)
        & (text.len <= h->arena_hi - text.at);
    bool ok = inside & (text.len <= RES_TEXT);

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "%s: a text of %u bytes is outside 0 to %u bytes", name, text.len,
            RES_TEXT);
    return (ok);
}

static bool check_counts(const bridge_api_t *api, const res_settings_t *s)
{
    const uint32_t values[] = { s->reload_delay_ms, s->load_bytes,
        s->max_resources, s->max_uses, s->max_loaders, s->max_sources,
        s->max_variables, s->max_reads, s->max_labels, s->path_bytes,
        s->settings_bytes };
    const uint32_t head[2] = { 1, s->load_bytes };
    bool ok = check_count(api, "head_bytes", s->head_bytes, head);

    for (size_t k = 0; k < sizeof(values) / sizeof(values[0]); k++)
        ok &= check_count(api, RULES[k].name, values[k], RULES[k].range);
    return (ok);
}

bool read_settings(const bridge_api_t *api)
{
    const res_settings_t *s = bridge_native_get_single(api, SETTINGS);
    int check = pick_choice(api, "meta_check", s->meta_check, CHECKS);
    int mode = pick_choice(api, "mode", s->mode, MODES);
    bool ok = check_counts(api, s);

    ok &= check_text(api, "meta_paths", s->meta_paths);
    ok &= check_text(api, "processed_folder", s->processed_folder);
    ok &= check_text(api, "sources", s->sources);
    res_module.s = *s;
    res_module.meta_check = (uint32_t)(check < 0 ? 0 : check);
    res_module.processed = mode == 1;
    return (ok & (check >= 0) & (mode >= 0));
}
