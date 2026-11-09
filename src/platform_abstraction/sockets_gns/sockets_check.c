/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <string.h>

#include "sockets_inside.h"

static const struct {
    uint32_t offset;
    const char *name;
} TEXTS[SOCKETS_TEXTS] = {
    { offsetof(sockets_settings_t, allow_without_auth), "allow_without_auth" },
    { offsetof(sockets_settings_t, allow_localhost_without_auth),
        "allow_localhost_without_auth" },
    { offsetof(sockets_settings_t, unencrypted), "unencrypted" },
    { offsetof(sockets_settings_t, stun_servers), "stun_servers" },
    { offsetof(sockets_settings_t, turn_servers), "turn_servers" },
    { offsetof(sockets_settings_t, turn_users), "turn_users" },
    { offsetof(sockets_settings_t, turn_passwords), "turn_passwords" },
    { offsetof(sockets_settings_t, ice_transports), "ice_transports" },
    { offsetof(sockets_settings_t, ice_implementation), "ice_implementation" },
};

bool check_numbers(const bridge_api_t *api, const sockets_settings_t *s)
{
    const sockets_number_t *n;
    uint32_t value;
    bool ok = true;
    bool good;

    for (uint32_t i = 0; i < SOCKETS_NUMBER_COUNT; i++) {
        n = &SOCKETS_NUMBERS[i];
        value = *(const uint32_t *)((const char *)s + n->offset);
        good = (value >= n->low) & (value <= n->high);
        good &= (n->config != -1) | (i != 0) | (value % 2 == 0);
        good &= (n->config != GNS_CONFIG_RECV_BUFFER)
            | (value > s->recv_max_message);
        if (!good)
            bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %s",
                n->name, value, n->range);
        ok &= good;
    }
    return (ok);
}

static bool copy_text(const bridge_api_t *api, uint32_t i,
    const sockets_settings_t *s)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    bridge_text_t text;
    bool inside;

    memcpy(&text, (const char *)s + TEXTS[i].offset, sizeof(text));
    inside = (text.at >= h->arena_lo) & (text.at <= h->arena_hi)
        & (text.len <= h->arena_hi - text.at) & (text.len < SOCKETS_TEXT);
    if (!inside) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "%s: a text of %u bytes is outside 0 to %u", TEXTS[i].name,
            text.len, SOCKETS_TEXT - 1);
        return (false);
    }
    memcpy(sockets_module.texts[i], api->shared_base + text.at, text.len);
    sockets_module.texts[i][text.len] = '\0';
    return (true);
}

bool copy_texts(const bridge_api_t *api, const sockets_settings_t *s)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    bridge_text_t d = s->debug_output;
    bool ok = true;
    bool inside = (d.at >= h->arena_lo) & (d.at <= h->arena_hi)
        & (d.len <= h->arena_hi - d.at) & (d.len < SOCKETS_TEXT);

    for (uint32_t i = 0; i < SOCKETS_TEXTS; i++)
        ok &= copy_text(api, i, s);
    if (!inside)
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "debug_output: a text of %u bytes is outside 0 to %u", d.len,
            SOCKETS_TEXT - 1);
    memcpy(sockets_module.debug_level, api->shared_base + d.at,
        inside * d.len);
    sockets_module.debug_level[inside * d.len] = '\0';
    return (ok & inside);
}

static bool check_word(const bridge_api_t *api, uint32_t i,
    const char *const *words, uint32_t count)
{
    static const char *const LISTS[] = { "refuse, warn or allow",
        "never, allowed, preferred or required", "auto, native or webrtc",
        ("none, bug, error, important, warning, msg, verbose, debug"
        " or everything")
    };
    const char *text = i < SOCKETS_TEXTS ? sockets_module.texts[i]
        : sockets_module.debug_level;
    uint32_t list = (count == UNENCRYPTED_COUNT) + 2 * (words == ICE_WORDS)
        + 3 * (words == LOG_WORDS);
    bool ok = word_number(text, words, count) < count;

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %s is outside %s",
            i < SOCKETS_TEXTS ? TEXTS[i].name : "debug_output", text,
            LISTS[list]);
    return (ok);
}

bool check_words(const bridge_api_t *api)
{
    bool ok = check_word(api, TEXT_ALLOW, ALLOW_WORDS, ALLOW_COUNT);

    ok &= check_word(api, TEXT_ALLOW_LOCALHOST, ALLOW_WORDS, ALLOW_COUNT);
    ok &= check_word(api, TEXT_UNENCRYPTED, UNENCRYPTED_WORDS,
        UNENCRYPTED_COUNT);
    ok &= check_word(api, TEXT_ICE_IMPLEMENTATION, ICE_WORDS, ICE_COUNT);
    ok &= check_word(api, SOCKETS_TEXTS, LOG_WORDS, LOG_COUNT);
    if (ice_mask(sockets_module.texts[TEXT_ICE]) < 0) {
        bridge_native_say(api, BRIDGE_LOG_ERR,
            "ice_transports: %s is outside relay, private and public",
            sockets_module.texts[TEXT_ICE]);
        ok = false;
    }
    return (ok);
}
