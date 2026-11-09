/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

static uint32_t kind_of(int level)
{
    bool warning = (level >= GNS_LOG_BUG) & (level <= GNS_LOG_WARNING);

    return (warning * BRIDGE_LOG_WARN + !warning * BRIDGE_LOG_INFO);
}

void on_output(int level, const char *text)
{
    const bridge_api_t *api = sockets_module.api;
    uint8_t line[SOCKETS_LINE + 1];
    size_t len = strlen(text);

    len -= (len > SOCKETS_LINE) * (len - SOCKETS_LINE);
    line[0] = (uint8_t)kind_of(level);
    memcpy(line + 1, text, len);
    ring_put(api->shared_base, &sockets_module.lines, line,
        (uint32_t)len + 1);
}

void say_lines(const bridge_api_t *api)
{
    const uint8_t *bytes;
    uint32_t len;
    char text[SOCKETS_LINE + 1];

    len = ring_take(api->shared_base, &sockets_module.lines, &bytes);
    while (len) {
        memcpy(text, bytes + 1, len - 1);
        text[len - 1] = '\0';
        bridge_native_say(api, bytes[0], "gns: %s", text);
        len = ring_take(api->shared_base, &sockets_module.lines, &bytes);
    }
}

void say_request(const bridge_api_t *api, uint64_t entity, const char *field,
    const char *outside)
{
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the request of %llu: %s is outside %s", (unsigned long long)entity,
        field, outside);
}

void say_bound(const bridge_api_t *api, const char *what, uint32_t max,
    uint64_t entity)
{
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "more than %u %s: the request of %llu fails", max, what,
        (unsigned long long)entity);
}
