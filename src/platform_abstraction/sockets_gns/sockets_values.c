/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gns_flat.h"
#include "sockets_values.h"

const char *const ALLOW_WORDS[ALLOW_COUNT] = { "refuse", "warn", "allow" };
const char *const UNENCRYPTED_WORDS[UNENCRYPTED_COUNT] = { "never", "allowed",
    "preferred", "required" };
const char *const ICE_WORDS[ICE_COUNT] = { "auto", "native", "webrtc" };
const char *const LOG_WORDS[LOG_COUNT] = { "none", "bug", "error", "important",
    "warning", "msg", "verbose", "debug", "everything" };

static const char *const ROUTE_WORDS[] = { "relay", "private", "public" };

uint32_t word_number(const char *text, const char *const *words,
    uint32_t count)
{
    uint32_t found = count;
    bool hit;

    for (uint32_t i = 0; i < count; i++) {
        hit = !strcmp(text, words[i]);
        found = hit * i + !hit * found;
    }
    return (found);
}

static int32_t route_bit(const char *word, size_t len)
{
    int32_t bit = -1;
    bool hit;

    for (uint32_t i = 0; i < 3; i++) {
        hit = strlen(ROUTE_WORDS[i]) == len
            && !memcmp(word, ROUTE_WORDS[i], len);
        bit = hit * (1 << i) + !hit * bit;
    }
    return (bit);
}

int32_t ice_mask(const char *text)
{
    int32_t mask = 0;
    int32_t bit;
    const char *comma;
    size_t len;

    if (!*text)
        return (0);
    while (mask >= 0) {
        comma = strchr(text, ',');
        len = comma ? (size_t)(comma - text) : strlen(text);
        bit = route_bit(text, len);
        mask = bit < 0 ? -1 : (mask | bit);
        if (!comma)
            break;
        text = comma + 1;
    }
    return (mask);
}

ideatek_connection_status_t status_of(int32_t state)
{
    static const ideatek_connection_status_t LIVE[] = {
        IDEATEK_CONNECTION_STATUS_PROBLEM,
        IDEATEK_CONNECTION_STATUS_CONNECTING,
        IDEATEK_CONNECTION_STATUS_FINDING_ROUTE,
        IDEATEK_CONNECTION_STATUS_CONNECTED,
        IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER,
        IDEATEK_CONNECTION_STATUS_PROBLEM,
    };
    bool live = (state >= GNS_STATE_NONE) & (state <= GNS_STATE_PROBLEM);

    return (LIVE[live * state]);
}
